// Copyright Dark State Studio.All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "DssTypes.generated.h"

namespace DssCore
{
	template <typename TKey>
	using TKeyToStringFn = TFunctionRef<FString(const TKey&)>;

	FORCEINLINE FString GameplayTagToString(const FGameplayTag& Value)
	{
		return Value.ToString();
	}

	template <typename TKey>
	using TStringToKeyFn = TFunctionRef<bool(const FString&, TKey&)>;

	FORCEINLINE bool StringToGameplayTag(const FString& Value, FGameplayTag& Key)
	{
		Key = FGameplayTag::RequestGameplayTag(FName(*Value), false);
		return Key.IsValid();
	}

	template<typename TEnum>FString EnumToNameString(TEnum Value)
	{
		return StaticEnum<TEnum>()->GetNameStringByValue(static_cast<int64>(Value));
	}

	template<typename TEnum>
	bool TryNameStringToEnum(const FString& StringValue, TEnum& OutValue)
	{
		const UEnum* Enum = StaticEnum<TEnum>();
		if (!Enum)
		{
			return false;
		}

		const int64 Value = Enum->GetValueByNameString(StringValue);

		if (Value == INDEX_NONE)
		{
			return false;
		}

		OutValue = static_cast<TEnum>(Value);
		return true;
	}
}


UENUM(BlueprintType)
enum class EDssErrorSeverity : uint8
{
	Warning,
	Error
};

USTRUCT(BlueprintType)
struct DSSCORE_API FDssError
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName Code;

	UPROPERTY(BlueprintReadOnly)
	FString Message;

	UPROPERTY(BlueprintReadOnly)
	EDssErrorSeverity Severity = EDssErrorSeverity::Error;
};

USTRUCT(BlueprintType)
struct DSSCORE_API FDssErrors
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FDssError> Items;

	void Add(const FName& Code, const FString& Message, const EDssErrorSeverity Severity = EDssErrorSeverity::Error)
	{
		Items.Add({Code, Message, Severity});
	}

	static void AddTo(FDssErrors* Errors, const FName& Code, const FString& Message, const EDssErrorSeverity Severity = EDssErrorSeverity::Error)
	{
		if (!Errors)
		{
			return;
		}

		Errors->Add(Code, Message, Severity);
	}

	bool HasAny() const
	{
		return Items.Num() > 0;
	}
};
