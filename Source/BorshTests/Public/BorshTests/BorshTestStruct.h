#pragma once

#include "CoreMinimal.h"
#include "Borsh/Borsh.h"
#include "BorshTestStruct.generated.h"

USTRUCT(BlueprintType)
struct FBorshTestVector2D
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int x;
	UPROPERTY(BlueprintReadWrite)
	int y;
};

inline auto serialize(FBorshTestVector2D& data, borsh::Serializer& serializer)
{
	return serializer(data.x, data.y);
}

USTRUCT(BlueprintType)
struct FBorshTestLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FBorshTestVector2D    a;
	UPROPERTY(BlueprintReadWrite)
	FBorshTestVector2D    b;
	UPROPERTY(BlueprintReadWrite)
	FString name;
};

inline auto serialize(FBorshTestLine& data, borsh::Serializer& serializer)
{
	return serializer(data.a, data.b, data.name);
}