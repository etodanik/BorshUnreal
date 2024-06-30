#include "Borsh/Automation/BorshAutomation.h"
#include "Borsh/Borsh.h"

BORSH_TEST("Borsh.Scalar")
{
	const int8_t Max8 = INT8_MAX;
	const int8_t Min8 = INT8_MIN;
	
	TArray<uint8_t> SerializedMax8 = BorshSerialize(Max8);
	UTEST_EQUAL("serialized INT8_MAX length", SerializedMax8.Num(), sizeof(int8_t));
	UTEST_EQUAL("serialized INT8_MAX data", SerializedMax8, TArray<uint8_t>{ 0b01111111 });
	UTEST_EQUAL("deserialized INT8_MAX data", BorshDeserialize<int8_t>(SerializedMax8), INT8_MAX);

	TArray<uint8_t> SerializedMin8 = BorshSerialize(Min8);
	UTEST_EQUAL("serialized INT8_MIN length", SerializedMin8.Num(), sizeof(int8_t));
	UTEST_EQUAL("serialized INT8_MIN data", SerializedMin8, TArray<uint8_t>{ 0b10000000 });
	UTEST_EQUAL("deserialized INT8_MIN data", BorshDeserialize<int8_t>(SerializedMin8), INT8_MIN);
	
	return true;
}
