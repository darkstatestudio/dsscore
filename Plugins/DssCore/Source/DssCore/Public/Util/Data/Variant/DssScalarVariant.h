// Copyright Dark State Studio.All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Util/DssTypes.h"
#include "DssScalarVariant.generated.h"

struct FDssScalarVariant;

UENUM(BlueprintType)
enum class EDssScalarVariantType : uint8
{
	Bool,
	Int,
	Float,
	String,
	Name,
	Tag
};

using FDssScalarVariantValue = TVariant<bool, float, int32, FString, FName, FGameplayTag>;

template <typename TKey>
using TDssScalarVariantMap = TMap<TKey, FDssScalarVariant>;

// Lightweight reflected scalar variant for config/profile/gameplay settings.
// Not intended to replace UObject serialization or arbitrary JSON.
USTRUCT()
struct DSSCORE_API FDssScalarVariant
{
	GENERATED_BODY()

	FDssScalarVariant()
	{
		Type = EDssScalarVariantType::Bool;
		Value.Set<bool>(false);
	}

	// -- FACTORIES --
	static FDssScalarVariant FromBool(const bool Value);
	static FDssScalarVariant FromInt(const int32 Value);
	static FDssScalarVariant FromFloat(const float Value);
	static FDssScalarVariant FromString(const FString& Value);
	static FDssScalarVariant FromName(FName Value);
	static FDssScalarVariant FromTag(FGameplayTag Value);

	FORCEINLINE EDssScalarVariantType GetType() const { return Type; }

	// -- Value Getters --
	FORCEINLINE bool TryGet(bool& OutValue) const;
	FORCEINLINE bool TryGet(int32& OutValue) const;
	FORCEINLINE bool TryGet(float& OutValue) const;
	FORCEINLINE bool TryGet(FString& OutValue) const;
	FORCEINLINE bool TryGet(FName& OutValue) const;
	FORCEINLINE bool TryGet(FGameplayTag& OutValue) const;

	// -- JSON --
	TSharedPtr<FJsonObject> ToJson() const;
	bool FromJson(const TSharedPtr<FJsonValue>& Json);

	template <typename TKey>
	static TSharedRef<FJsonObject> MapToJson(const TDssScalarVariantMap<TKey>& Map, DssCore::TKeyToStringFn<TKey> KeyToString);

	static TSharedRef<FJsonObject> MapToJson(const TDssScalarVariantMap<FGameplayTag>& Map);


	template <typename TKey>
	static bool MapFromJson(const TSharedPtr<FJsonObject>& Json, DssCore::TStringToKeyFn<TKey> KeyParser, TDssScalarVariantMap<TKey>& OutMap,
	                        FDssErrors* Errors = nullptr);

	static bool MapFromJson(const TSharedPtr<FJsonObject>& Json, TDssScalarVariantMap<FGameplayTag>& OutMap, FDssErrors* Errors = nullptr);

protected:
	EDssScalarVariantType Type = EDssScalarVariantType::Bool;
	FDssScalarVariantValue Value;
};
