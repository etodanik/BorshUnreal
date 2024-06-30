#pragma once
#include "Borsh/Borsh.h"
#include "Containers/UnrealString.h"
//
// namespace borsh
// {
// 	template <>
// 	struct IsScalar<FString> : std::true_type {};
// 	
// 	template <typename T>
// 	concept UEStringType = std::is_same_v<T, FString>;
//
// 	template <typename T>
// 	concept UESerializableArray = requires(T t) {
// 		requires std::same_as<std::remove_cv_t<T>, TArray<typename T::ElementType>>;
// 		requires SerializableElement<std::remove_cv_t<typename T::ElementType>> || SerializableArray<std::remove_cv_t<typename T::ElementType>>;
// 	};
// 	
// 	template <typename T>
// 	concept UESerializableArrayArray = requires(T t) {
// 		requires std::same_as<std::remove_cv_t<T>, TArray<typename T::ElementType>>;
// 		requires UESerializableArray<std::remove_cv_t<typename T::ElementType>>;
// 	};
// 	
// 	template <typename T>
// 	concept UEScalarType = ScalarType<T> || UEStringType<T>;
//
// 	template <typename T>
// 	concept UEScalarTypeOnly = UEScalarType<T> && !ScalarType<T>;
// 	
// 	template <typename T>
// 	concept UEScalarArrayType = is_bounded_array_v<T> && UEScalarType<remove_extent_and_cv_t<T>>;
// 	
// 	template <typename T>
// 	concept UESerializable = Serializable<T> || UESerializableArray<T> || UESerializableArrayArray<T>;
// 	
// 	template <typename T>
// 	concept UESerializableNonScalar = SerializableElement<T> && !UEScalarType<T>;
//
// 	template <typename T>
// 	concept UESerializableNonScalarArray = UESerializableArray<T> && !UEScalarType<remove_extent_and_cv_t<T>>;
//
// 	template <typename T>
// 	concept UESerializableScalar = SerializableElement<T> && UEScalarType<T>;
// 		
// 	void to_bytes(UEStringType auto const& Value, std::vector<uint8_t>& Buffer)
// 	{
// 		// this is a very lazy and definitely not the most performant way to do this because it involves a conversion
// 		// TODO: properly serialize the underlying structure of FString
// 		return to_bytes(std::string(TCHAR_TO_UTF8(*Value)), Buffer);
// 	}
//
// 	template <UEStringType T> void from_bytes(T& Value, const uint8_t*& Buffer)
// 	{
// 		std::string StdString;
// 		// this is a very lazy and definitely not the most performant way to do this because it involves a conversion
// 		// TODO: properly deserialize the underlying structure of FString
// 		from_bytes<std::string>(StdString, Buffer);
// 		Value = FString(UTF8_TO_TCHAR(StdString.c_str()));
// 	}
//
// 	auto serialize(UEScalarTypeOnly auto& value, Serializer& serializer)
// 	{
// 		return serializer(value);
// 	}
// 	
// 	template <typename T>
// 	requires UEScalarTypeOnly<T> || UEScalarArrayType<T>
// 		std::vector<uint8_t> serialize(const T& Value)
// 	{
// 		std::vector<uint8_t> Buffer;
// 		to_bytes(Value, Buffer);
// 		return Buffer;
// 	}
// 	
// 	template <UESerializableArray T> std::vector<typename T::ElementType> ue_array_to_std_vector(T& Object)
// 	{
// 		// TODO: investigate a better way to serialize (perhaps see if one can simply construct a copy-free std::vector instance?
// 		size_t Length = Object.Num();
// 		std::vector<typename T::ElementType> Vector(Length);
// 		for (int32 i = 0; i < Length; ++i)
// 		{
// 			Vector[i] = Object[i];
// 		}
// 		return Vector;
// 	}
// 	
// 	template <UESerializableArray T> std::vector<uint8_t> serialize(T& Object)
// 	{
// 		auto StdVector = ue_array_to_std_vector(Object);
// 		return serialize(StdVector);
// 	}
// 	
// 	template <typename T>
// 		requires UEScalarTypeOnly<T>
// 	T deserialize(std::vector<uint8_t>& Buffer)
// 	{
// 		const uint8_t* Data = Buffer.data();
// 		T              Value;
// 		from_bytes(Value, Data);
// 		return Value;
// 	}
// 	
// 	template <typename T, std::size_t N>
// 		requires UEScalarTypeOnly<T>
// 	void deserialize(T (&Value)[N], std::vector<uint8_t>& Buffer)
// 	{
// 		const uint8_t* Data = Buffer.data();
// 		from_bytes(Value, Data);
// 	}
// 	
// 	template <UESerializableArray T> T deserialize(std::vector<uint8_t>& Buffer)
// 	{
// 		std::vector<typename T::ElementType> Vector = deserialize<std::vector<typename T::ElementType>>(Buffer);
// 		return T(Vector.data(), Vector.size());
// 	}
//
// }
//
// template <typename T>
// concept BorshSerializable = borsh::UESerializable<T>;
//
// template <typename T>
// requires borsh::UEScalarType<T> || borsh::UEScalarArrayType<T>
// 	TArray<uint8_t> BorshSerialize(const T& Value)
// {
// 	const std::vector<uint8_t> Serialized = borsh::serialize(Value);
// 	return TArray(Serialized.data(), Serialized.size());
// }
// 	
// template <borsh::UESerializableArray T> TArray<uint8_t> BorshSerialize(T& Object)
// {
// 	const std::vector<uint8_t> Serialized = borsh::serialize(Object);
// 	return TArray(Serialized.data(), Serialized.size());
// }
//
// template <borsh::UESerializableNonScalar T> TArray<uint8_t> BorshSerialize(T& Object)
// {
// 	const std::vector<uint8_t> Serialized = borsh::serialize(Object);
// 	return TArray(Serialized.data(), Serialized.size());
// }
//
// template <typename T>
// 	requires borsh::UEStringType<T>
// T BorshDeserialize(TArray<uint8_t>& BufferArray)
// {
// 	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
// 	return borsh::deserialize<FString>(BufferVector);
// }
// 	
// template <typename T>
// 	requires borsh::UEScalarType<T>
// T BorshDeserialize(TArray<uint8_t>& BufferArray)
// {
// 	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
// 	return borsh::deserialize<T>(BufferVector);
// }
//
// template <typename T, std::size_t N>
// 	requires borsh::UEScalarType<T>
// void BorshDeserialize(T (&Value)[N], TArray<uint8_t>& BufferArray)
// {
// 	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
// 	deserialize(Value, BufferVector);
// }
// 	
// template <borsh::UESerializableArray T> T BorshDeserialize(TArray<uint8_t>& BufferArray)
// {
// 	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
// 	std::vector<typename T::ElementType> Vector = borsh::deserialize<std::vector<typename T::ElementType>>(BufferVector);
// 	return T(Vector.data(), Vector.size());
// }
//
// template <borsh::UESerializableNonScalar T> T BorshDeserialize(TArray<uint8_t>& BufferArray)
// {
// 	std::vector<uint8_t> BufferVector = borsh::ue_array_to_std_vector(BufferArray);
// 	return borsh::deserialize<T>(BufferVector);
// }
//
