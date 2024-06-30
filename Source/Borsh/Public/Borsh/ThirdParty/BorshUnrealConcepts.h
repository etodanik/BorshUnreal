#pragma once

namespace borsh
{
	
template <typename T>
concept UEStringType = std::is_same_v<T, FString>;

template <>
struct IsScalar<FString> : std::true_type {};

template <typename T>
concept UESerializableArray = requires(T t) {
	requires std::same_as<std::remove_cv_t<T>, TArray<typename T::ElementType>>;
	requires SerializableElement<std::remove_cv_t<typename T::ElementType>> || SerializableArray<std::remove_cv_t<typename T::ElementType>>;
};

template <typename T>
concept UESerializableArrayArray = requires(T t) {
	requires std::same_as<std::remove_cv_t<T>, TArray<typename T::ElementType>>;
	requires UESerializableArray<std::remove_cv_t<typename T::ElementType>>;
};

template <typename T>
concept UEScalarType = ScalarType<T> || UEStringType<T>;

template <typename T>
concept UEScalarTypeOnly = UEScalarType<T> && !ScalarType<T>;

template <typename T>
concept UEScalarArrayType = is_bounded_array_v<T> && UEScalarType<remove_extent_and_cv_t<T>>;

template <typename T>
concept UESerializable = Serializable<T> || UESerializableArray<T> || UESerializableArrayArray<T>;

template <typename T>
concept UESerializableNonScalar = SerializableElement<T> && !UEScalarType<T>;

template <typename T>
concept UESerializableNonScalarArray = UESerializableArray<T> && !UEScalarType<remove_extent_and_cv_t<T>>;

template <typename T>
concept UESerializableScalar = SerializableElement<T> && UEScalarType<T>;
	
}

template <typename T>
concept BorshSerializable = borsh::UESerializable<T>;