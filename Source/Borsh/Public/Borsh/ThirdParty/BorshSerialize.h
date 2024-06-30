//
// Copyright 2023 Daniel Povolotski (dannypovolotski at gmail dot com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef BORSHUNREAL_CPP20_H
#define BORSHUNREAL_CPP20_H

namespace borsh
{

	auto serialize(UEScalarTypeOnly auto& value, Serializer& serializer)
	{
		return serializer(value);
	}
	
	template <typename T>
	requires UEScalarTypeOnly<T> || UEScalarArrayType<T>
		std::vector<uint8_t> serialize(const T& Value)
	{
		std::vector<uint8_t> Buffer;
		to_bytes(Value, Buffer);
		return Buffer;
	}
	
	template <UESerializableArray T> std::vector<typename T::ElementType> ue_array_to_std_vector(T& Object)
	{
		// TODO: investigate a better way to serialize (perhaps see if one can simply construct a copy-free std::vector instance?
		size_t Length = Object.Num();
		std::vector<typename T::ElementType> Vector(Length);
		for (int32 i = 0; i < Length; ++i)
		{
			Vector[i] = Object[i];
		}
		return Vector;
	}
	
	template <UESerializableArray T> std::vector<uint8_t> serialize(T& Object)
	{
		auto StdVector = ue_array_to_std_vector(Object);
		return serialize(StdVector);
	}
	
	template <typename T>
		requires UEScalarTypeOnly<T>
	T deserialize(std::vector<uint8_t>& Buffer)
	{
		const uint8_t* Data = Buffer.data();
		T              Value;
		from_bytes(Value, Data);
		return Value;
	}
	
	template <typename T, std::size_t N>
		requires UEScalarTypeOnly<T>
	void deserialize(T (&Value)[N], std::vector<uint8_t>& Buffer)
	{
		const uint8_t* Data = Buffer.data();
		from_bytes(Value, Data);
	}
	
	template <UESerializableArray T> T deserialize(std::vector<uint8_t>& Buffer)
	{
		std::vector<typename T::ElementType> Vector = deserialize<std::vector<typename T::ElementType>>(Buffer);
		return T(Vector.data(), Vector.size());
	}

}


template <typename T>
requires borsh::UEScalarType<T> || borsh::UEScalarArrayType<T>
	TArray<uint8_t> BorshSerialize(const T& Value)
{
	const std::vector<uint8_t> Serialized = borsh::serialize(Value);
	return TArray(Serialized.data(), Serialized.size());
}
	
template <borsh::UESerializableArray T> TArray<uint8_t> BorshSerialize(T& Object)
{
	const std::vector<uint8_t> Serialized = borsh::serialize(Object);
	return TArray(Serialized.data(), Serialized.size());
}

template <borsh::UESerializableNonScalar T> TArray<uint8_t> BorshSerialize(T& Object)
{
	const std::vector<uint8_t> Serialized = borsh::serialize(Object);
	return TArray(Serialized.data(), Serialized.size());
}

template <typename T>
	requires borsh::UEStringType<T>
T BorshDeserialize(TArray<uint8_t>& BufferArray)
{
	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
	return borsh::deserialize<FString>(BufferVector);
}
	
template <typename T>
	requires borsh::UEScalarType<T>
T BorshDeserialize(TArray<uint8_t>& BufferArray)
{
	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
	return borsh::deserialize<T>(BufferVector);
}

template <typename T, std::size_t N>
	requires borsh::UEScalarType<T>
void BorshDeserialize(T (&Value)[N], TArray<uint8_t>& BufferArray)
{
	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
	deserialize(Value, BufferVector);
}
	
template <borsh::UESerializableArray T> T BorshDeserialize(TArray<uint8_t>& BufferArray)
{
	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
	std::vector<typename T::ElementType> Vector = borsh::deserialize<std::vector<typename T::ElementType>>(BufferVector);
	return T(Vector.data(), Vector.size());
}

template <borsh::UESerializableNonScalar T> T BorshDeserialize(TArray<uint8_t>& BufferArray)
{
	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
	return borsh::deserialize<T>(BufferVector);
}


#endif