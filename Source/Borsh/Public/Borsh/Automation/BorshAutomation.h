#pragma once

#include "CoreTypes.h"
#include "Misc/AutomationTest.h"
#include "Misc/AssertionMacros.h"
#include "Containers/UnrealString.h"

#define BORSH_JOIN(a, b) BORSH_JOIN2(a, b)
#define BORSH_JOIN2(a, b) a##b
#define BORSH_UNIQUE(a) BORSH_JOIN(a, __LINE__)

class BORSH_API FBorshAutomationBase : public FAutomationTestBase
{
public:
	static FString CheckUniqueName(const FString& InName);

	constexpr static uint32 FLAGS = EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::EngineFilter;

	FBorshAutomationBase( const FString& InName, const bool bInComplexTask )
		: FAutomationTestBase(InName, bInComplexTask)
	{
		//	suppress logs here as `BorshAutomationDetails::RunTestsBody` handles logs
		bSuppressLogs = true;
	}

	uint32 GetTestFlags() const override;
	uint32 GetRequiredDeviceNum() const override;
	
	bool RunTest(const FString& Parameters) override;

	virtual bool BorshRunTestBody(const FString& Parameters) = 0;
};

///	Example:
///
///		BORSH_TEST("Borsh.Foo.Bar")
///		{
///			// test body
///			return true;
///		}

#define BORSH_TEST_IMPL(ID, PrettyName) \
	namespace { \
	class BORSH_JOIN(FBorshAutomationTest, ID) : public FBorshAutomationBase \
	{ \
	public: \
		BORSH_JOIN(FBorshAutomationTest, ID)(const FString& InName) \
			: FBorshAutomationBase(InName, false) {} \
		virtual FString GetTestSourceFileName() const override { return __FILE__; } \
		virtual int32 GetTestSourceFileLine() const override { return __LINE__; } \
	protected: \
		virtual void GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const override \
		{ \
			OutBeautifiedNames.Add(PrettyName); \
			OutTestCommands.Add(FString()); \
		} \
		virtual bool BorshRunTestBody(const FString& Parameters) override; \
		virtual FString GetBeautifiedTestName() const override { return PrettyName; } \
	}; \
	BORSH_JOIN(FBorshAutomationTest, ID) BORSH_JOIN(_BorshAutomationInstance, ID)(FBorshAutomationBase::CheckUniqueName(TEXT(PrettyName))); \
	} \
	bool BORSH_JOIN(FBorshAutomationTest, ID)::BorshRunTestBody(const FString& Parameters)

#define BORSH_TEST(PrettyName) BORSH_TEST_IMPL(__COUNTER__, PrettyName)

struct BORSH_API FBorshAutomationConsoleRunner
{
	struct FArgs
	{
		TArray<FString> Filters;
		TArray<FString> Parameters;
		uint32 RequestedTestFilter;
	};

	static FArgs FromCommandlineTokens(const TArray<FString>& Tokens);

	void Prepare(const FArgs& Args);
	int32 RunTests();

	TArray<FAutomationTestInfo> SelectedTests;
	FString ParametersJoined;
};

