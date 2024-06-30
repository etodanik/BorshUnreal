#include "BorshTests/BorshTestStruct.h"
#include "Borsh/Automation/BorshAutomation.h"

BORSH_TEST("Borsh.UStruct")
{
	FBorshTestVector2D Point{ 10, 20 };
	TArray<uint8_t> Buffer = BorshSerialize(Point);
	UTEST_EQUAL("buffer size", Buffer.Num(), sizeof(int32_t) * 2);
	FBorshTestVector2D Deserialized = BorshDeserialize<FBorshTestVector2D>(Buffer);
	UTEST_EQUAL("Point.x", Deserialized.x, 10);
	UTEST_EQUAL("Point.y", Deserialized.y, 20);

	return true;
}

BORSH_TEST("Borsh.UStruct (Nested)")
{
	static_assert(BorshSerializable<FBorshTestLine>);

	FBorshTestLine Line{ { 5, 10 }, { 15, 25 }, "my line" };

	TArray<uint8_t> Buffer = BorshSerialize(Line);
	UTEST_EQUAL("serialized length", Buffer.Num(), 27);
	FBorshTestLine Deserialized = BorshDeserialize<FBorshTestLine>(Buffer);
	UTEST_EQUAL("Deserialized.a.x", Deserialized.a.x, 5);
	UTEST_EQUAL("Deserialized.a.y", Deserialized.a.y, 10);
	UTEST_EQUAL("Deserialized.b.x", Deserialized.b.x, 15);
	UTEST_EQUAL("Deserialized.b.y", Deserialized.b.y, 25);
	UTEST_EQUAL("Deserialized.name", Deserialized.name, FString("my line"));
		
	return true;
}
