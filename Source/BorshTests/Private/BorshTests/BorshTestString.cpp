#include "Borsh/Automation/BorshAutomation.h"
#include "Borsh/Borsh.h"

BORSH_TEST("Borsh.FString")
{
	static_assert(BorshSerializable<FString>);

	FString String = FString(TEXT("hello 🚀"));

	TArray<uint8_t> StringData = TArray<uint8_t>{ //
		// int32_t representation of the string length (little endian)
		0b00001010, 0b00000000, 0b00000000, 0b00000000,
		// utf-8 string
		0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0xf0, 0x9f, 0x9a, 0x80
	};
	
	TArray<uint8_t> SerializedString = BorshSerialize(String);
	UTEST_EQUAL("serialized string size", SerializedString.Num(), 14);
	UTEST_EQUAL("serialized string data", SerializedString, StringData);

	FString DeserializedString = BorshDeserialize<FString>(SerializedString);
	UTEST_EQUAL("deserialized string", DeserializedString, String);
	
	return true;
}
