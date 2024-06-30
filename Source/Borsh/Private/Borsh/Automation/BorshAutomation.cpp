#include "Borsh/Automation/BorshAutomation.h"
#include "Borsh/BorshTypes.h"
#include "HAL/FeedbackContextAnsi.h"
#include "HAL/ExceptionHandling.h"

struct FBorshAutomationFeedbackContext : public FFeedbackContextAnsi
{
	void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override
	{
		if (Category == FName(TEXT("LogBorsh"))
			&& Verbosity == ELogVerbosity::Display)
		{
			LocalPrint(V);
			LocalPrint(TEXT("\n"));
		}
		else if (Category == FName(TEXT("LogBorsh"))
			&& Verbosity == ELogVerbosity::NoLogging)
		{
			//	pass
		}
		else
		{
			FFeedbackContextAnsi::Serialize(V, Verbosity, Category);
		}
	}
};

FString FBorshAutomationBase::CheckUniqueName(const FString& InName)
{
	check(!FAutomationTestFramework::Get().ContainsTest(InName));
	return InName;
}

uint32 FBorshAutomationBase::GetTestFlags() const
{
	return FLAGS;
}

uint32 FBorshAutomationBase::GetRequiredDeviceNum() const
{
	return 1;
}

bool FBorshAutomationBase::RunTest(const FString& Parameters)
{
	bool bRet = BorshRunTestBody(Parameters);
	// BorshEnv().FlushDiags();
	return bRet;
}

#if PLATFORM_WINDOWS && !PLATFORM_SEH_EXCEPTIONS_DISABLED
static int32 _Win32DumpStackAndExit(Windows::LPEXCEPTION_POINTERS ExceptionInfo)
{
	const SIZE_T StackTraceSize = 65535;
	ANSICHAR* StackTrace = (ANSICHAR*)FMemory::SystemMalloc(StackTraceSize);

	StackTrace[0] = 0;
	FPlatformStackWalk::StackWalkAndDumpEx(
		StackTrace,
		StackTraceSize,
		7,
		FGenericPlatformStackWalk::EStackWalkFlags::FlagsUsedWhenHandlingEnsure);

	puts(StackTrace);
	FMemory::SystemFree(StackTrace);
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

FBorshAutomationConsoleRunner::FArgs FBorshAutomationConsoleRunner::FromCommandlineTokens(const TArray<FString>& Tokens)
{
	FBorshAutomationConsoleRunner::FArgs Args;
	if (Tokens.Num() == 0)
	{
		//	default filter
		//	ends with `.` to filter out DataConfigBenchmark
		Args.Filters.Add(TEXT("DataConfig."));
	}

	int ParametersSplitIx = -1;
	//	note that we look for "--", but FCommandLine::Parse eats one '-'
	bool bHasParametersSplit = Tokens.Find(TEXT("-"), ParametersSplitIx);
	if (bHasParametersSplit)
	{
		for (int Ix = 0; Ix < ParametersSplitIx; Ix++)
			Args.Filters.Add(Tokens[Ix]);

		for (int Ix = ParametersSplitIx+1; Ix < Tokens.Num(); Ix++)
			Args.Parameters.Add(Tokens[Ix]);
	}
	else
	{
		for (const FString& Token : Tokens)
			Args.Filters.Add(Token);
	}

	Args.RequestedTestFilter = FBorshAutomationBase::FLAGS;

	return Args;
}

void FBorshAutomationConsoleRunner::Prepare(const FArgs& Args)
{
	UE_SET_LOG_VERBOSITY(LogBorsh, Display);

	FAutomationTestFramework& Framework = FAutomationTestFramework::Get();

	UE_LOG(LogBorsh, Display, TEXT("Filters: %s"), *FString::Join(Args.Filters, TEXT(", ")));

	Framework.SetRequestedTestFilter(Args.RequestedTestFilter);

	TArray<FAutomationTestInfo> TestInfos;
	Framework.GetValidTestNames(TestInfos);

	TestInfos.RemoveAllSwap([&Args](FAutomationTestInfo& TestInfo) {
		for (const FString& Filter : Args.Filters)
			if (!TestInfo.GetDisplayName().Contains(Filter, ESearchCase::IgnoreCase))
				return true;

		return false;
	});

	SelectedTests = MoveTemp(TestInfos);
	ParametersJoined = FString::Join(Args.Parameters, TEXT(" "));
}

namespace BorshAutomationDetails
{

int32 RunTestsBody(FBorshAutomationConsoleRunner* Self)
{
	FAutomationTestFramework& Framework = FAutomationTestFramework::Get();

	FBorshAutomationFeedbackContext BorshAutomationLog;
	TBorshStoreThenReset<FFeedbackContext*> OverrideGWarn(GWarn, &BorshAutomationLog);

	bool bAllSuccessful = true;

	{
		FAutomationTestFramework::Get().SetCaptureStack(true);

		int RunCount = 0;
		int SuccessCount = 0;
		int FailCount = 0;

		{
			//	shuffle tests for random execution order
			int32 LastIndex = Self->SelectedTests.Num() - 1;
			for (int32 Ix = 0; Ix < LastIndex; Ix++)
			{
				int32 SwapIx = FMath::RandRange(Ix, LastIndex);
				Self->SelectedTests.Swap(Ix, SwapIx);
			}
		}

		for (const FAutomationTestInfo& TestInfo: Self->SelectedTests)
		{
			FString TestCommand = FString::Printf(TEXT("%s %s"), *TestInfo.GetTestName(), *Self->ParametersJoined);
			FAutomationTestExecutionInfo CurExecutionInfo;

			int32 RoleIndex = 0;
			Framework.StartTestByName(TestCommand, RoleIndex);
			const bool CurTestSuccessful = Framework.StopTest(CurExecutionInfo);

			bAllSuccessful = bAllSuccessful && CurTestSuccessful;
			++RunCount;
			if (CurTestSuccessful) ++SuccessCount;
			else ++ FailCount;

			{
				UE_LOG(LogBorsh, Display, TEXT("- %4s | %s "),
					CurTestSuccessful ? TEXT("OK") : TEXT("FAIL"),
					*TestCommand
				);

				for (const FAutomationExecutionEntry& Entry: CurExecutionInfo.GetEntries())
				{
					UE_LOG(LogBorsh, Display, TEXT("* %s %s"),
						*Entry.Event.Message,
						*Entry.Event.Context
					);
					UE_LOG(LogBorsh, Display, TEXT("  | %s:%d"),
						*Entry.Filename,
						Entry.LineNumber
					);
				}
			}
		}

		UE_LOG(LogBorsh, Display, TEXT("Run: %4d, Success: %4d, Fail: %4d"),
			RunCount, SuccessCount, FailCount
		);
	}

	return bAllSuccessful ? 0 : -1;
}


}	// namespace BorshAutomationDetails


int32 FBorshAutomationConsoleRunner::RunTests()
{
	if (FPlatformMisc::IsDebuggerPresent())
	{
		return BorshAutomationDetails::RunTestsBody(this);
	}
	else
	{

#if PLATFORM_WINDOWS && !PLATFORM_SEH_EXCEPTIONS_DISABLED
		__try
#endif
		{
			return BorshAutomationDetails::RunTestsBody(this);
		}
#if PLATFORM_WINDOWS && !PLATFORM_SEH_EXCEPTIONS_DISABLED
		__except (_Win32DumpStackAndExit(GetExceptionInformation()))
		{
			FPlatformMisc::RequestExit(true);
			return -1;
		}
#endif
	}
}
