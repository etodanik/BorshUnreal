#include "Borsh/Automation/BorshAutomation.h"
#include "Borsh/Borsh.h"

BORSH_TEST("Borsh.TArray")
{
	static_assert(BorshSerializable<TArray<int32_t>>);
	static_assert(BorshSerializable<const TArray<int32_t>>);

	const TArray<int32_t> Array = { 15, -20, 10, 3435, -4011 };
	const TArray<uint8_t> ExpectedData = TArray<uint8_t>{
		0b00000101,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00001111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b11101100,
		0b11111111,
		0b11111111,
		0b11111111,
		0b00001010,
		0b00000000,
		0b00000000,
		0b00000000,
		0b01101011,
		0b00001101,
		0b00000000,
		0b00000000,
		0b01010101,
		0b11110000,
		0b11111111,
		0b11111111,
	};
	
	TArray<uint8_t> SerializedArray = BorshSerialize(Array);
	auto SerializedSize = SerializedArray.Num();
	UTEST_EQUAL("serialized size", SerializedSize, sizeof(int32_t) * 6);
	UTEST_EQUAL("serialized data", SerializedArray, ExpectedData);

	TArray<int32_t> DeserializedArray = BorshDeserialize<TArray<int32_t>>(SerializedArray);
	UTEST_EQUAL("deserialized array 0", DeserializedArray[0], Array[0]);
	UTEST_EQUAL("deserialized array 1", DeserializedArray[1], Array[1]);
	UTEST_EQUAL("deserialized array 2", DeserializedArray[2], Array[2]);
	UTEST_EQUAL("deserialized array 3", DeserializedArray[3], Array[3]);
	UTEST_EQUAL("deserialized array 4", DeserializedArray[4], Array[4]);
	
	return true;
}
