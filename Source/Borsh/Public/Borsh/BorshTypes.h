#pragma once

BORSH_API DECLARE_LOG_CATEGORY_EXTERN(LogBorsh, Log, All);

template<typename T>
struct TBorshStoreThenReset
{
	TBorshStoreThenReset(T& InRef, const T& NewValue)
		: Ref(InRef)
	{
		RestoreValue = Ref;
		Ref = NewValue;
	}

	TBorshStoreThenReset(T& InRef)
		: Ref(InRef)
	{
		RestoreValue = Ref;
	}

	~TBorshStoreThenReset()
	{
		Ref = RestoreValue;
	}

	T& Ref;
	T RestoreValue;
};