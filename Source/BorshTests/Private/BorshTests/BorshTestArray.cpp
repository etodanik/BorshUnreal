#include "Borsh/Automation/BorshAutomation.h"
#include "BorshTests/BorshTestStruct.h"
#include "Borsh/Borsh.h"

BORSH_TEST("Borsh.TArray<int32_t>")
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

BORSH_TEST("Borsh.TArray<UStruct>")
{
	static_assert(BorshSerializable<TArray<FBorshTestLine>>);

	const TArray<FBorshTestLine> Array = { { { 5, 10 }, { 15, 25 }, "hello 🚀" }, { { 25, 30 }, { 45, 75 }, "olleh 🚀" } };

	TArray<uint8_t> SerializedVector = BorshSerialize(Array);
	UTEST_EQUAL("", SerializedVector.Num(),
		// the result should be the raw length of the types with prepended length
		sizeof(uint32_t)          // length
		+ sizeof(FBorshTestVector2D) * 2    // two Vector structs
		+ static_cast<size_t>(14) // string
		+ sizeof(FBorshTestVector2D) * 2    // two Vector structs
		+ static_cast<size_t>(14) // string
	);

	auto DeserializedVector = BorshDeserialize<TArray<FBorshTestLine>>(SerializedVector);

	UTEST_EQUAL("DeserializedVector[0].a.x", DeserializedVector[0].a.x, 5);
	UTEST_EQUAL("DeserializedVector[0].a.y", DeserializedVector[0].a.y, 10);
	UTEST_EQUAL("DeserializedVector[0].b.x", DeserializedVector[0].b.x, 15);
	UTEST_EQUAL("DeserializedVector[0].b.y", DeserializedVector[0].b.y, 25);
	UTEST_EQUAL("DeserializedVector[0].name", DeserializedVector[0].name, FString(TEXT("hello 🚀")));
	UTEST_EQUAL("DeserializedVector[1].a.x", DeserializedVector[1].a.x, 25);
	UTEST_EQUAL("DeserializedVector[1].a.y", DeserializedVector[1].a.y, 30);
	UTEST_EQUAL("DeserializedVector[1].b.x", DeserializedVector[1].b.x, 45)
	UTEST_EQUAL("DeserializedVector[1].b.y", DeserializedVector[1].b.y, 75);
	UTEST_EQUAL("DeserializedVector[1].name", DeserializedVector[1].name, FString(TEXT("olleh 🚀")));
	
	return true;
}
