#pragma once

namespace borsh
{
	
	void to_bytes(UEStringType auto const& Value, std::vector<uint8_t>& Buffer)
	{
		// this is a very lazy and definitely not the most performant way to do this because it involves a conversion
		// TODO: properly serialize the underlying structure of FString
		return to_bytes(std::string(TCHAR_TO_UTF8(*Value)), Buffer);
	}

	template <UEStringType T> void from_bytes(T& Value, const uint8_t*& Buffer)
	{
		std::string StdString;
		// this is a very lazy and definitely not the most performant way to do this because it involves a conversion
		// TODO: properly deserialize the underlying structure of FString
		from_bytes<std::string>(StdString, Buffer);
		Value = FString(UTF8_TO_TCHAR(StdString.c_str()));
	}
	
}