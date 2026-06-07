// Copyright Dark State Studio.All rights reserved.


#include "Util/Data/Variant/DssScalarVariant.h"

FDssScalarVariant FDssScalarVariant::FromBool(const bool Value)
{
	FDssScalarVariant Result;
	Result.Type = EDssScalarVariantType::Bool;
	Result.Value.Set<bool>(Value);
	return Result;
}

FDssScalarVariant FDssScalarVariant::FromInt(const int32 Value)
{
	FDssScalarVariant Result;
	Result.Type = EDssScalarVariantType::Int;
	Result.Value.Set<int32>(Value);
	return Result;
}

FDssScalarVariant FDssScalarVariant::FromFloat(const float Value)
{
	FDssScalarVariant Result;
	Result.Type = EDssScalarVariantType::Float;
	Result.Value.Set<float>(Value);
	return Result;
}

FDssScalarVariant FDssScalarVariant::FromString(const FString& Value)
{
	FDssScalarVariant Result;
	Result.Type = EDssScalarVariantType::String;
	Result.Value.Set<FString>(Value);
	return Result;
}

FDssScalarVariant FDssScalarVariant::FromName(FName Value)
{
	FDssScalarVariant Result;
	Result.Type = EDssScalarVariantType::Name;
	Result.Value.Set<FName>(Value);
	return Result;
}

FDssScalarVariant FDssScalarVariant::FromTag(FGameplayTag Value)
{
	FDssScalarVariant Result;
	Result.Type = EDssScalarVariantType::Tag;
	Result.Value.Set<FGameplayTag>(Value);
	return Result;
}


bool FDssScalarVariant::TryGet(bool& OutValue) const
{
	if (Type != EDssScalarVariantType::Bool)
	{
		return false;
	}

	OutValue = Value.Get<bool>();
	return true;
}

bool FDssScalarVariant::TryGet(int32& OutValue) const
{
	if (Type != EDssScalarVariantType::Int)
	{
		return false;
	}

	OutValue = Value.Get<int32>();
	return true;
}

bool FDssScalarVariant::TryGet(float& OutValue) const
{
	if (Type != EDssScalarVariantType::Float)
	{
		return false;
	}

	OutValue = Value.Get<float>();
	return true;
}

bool FDssScalarVariant::TryGet(FString& OutValue) const
{
	if (Type != EDssScalarVariantType::String)
	{
		return false;
	}

	OutValue = Value.Get<FString>();
	return true;
}

bool FDssScalarVariant::TryGet(FName& OutValue) const
{
	if (Type != EDssScalarVariantType::Name)
	{
		return false;
	}

	OutValue = Value.Get<FName>();
	return true;
}

bool FDssScalarVariant::TryGet(FGameplayTag& OutValue) const
{
	if (Type != EDssScalarVariantType::Tag)
	{
		return false;
	}

	OutValue = Value.Get<FGameplayTag>();
	return true;
}

TSharedPtr<FJsonObject> FDssScalarVariant::ToJson() const
{
	auto Json = MakeShared<FJsonObject>();

	Json->SetStringField(TEXT("t"), DssCore::EnumToNameString(Type));

	switch (Type)
	{
	case EDssScalarVariantType::Bool:
		Json->SetBoolField(TEXT("v"), Value.Get<bool>());
		break;
	case EDssScalarVariantType::Int:
		Json->SetNumberField(TEXT("v"), Value.Get<int32>());
		break;
	case EDssScalarVariantType::Float:
		Json->SetNumberField(TEXT("v"), Value.Get<float>());
		break;
	case EDssScalarVariantType::String:
		Json->SetStringField(TEXT("v"), Value.Get<FString>());
		break;
	case EDssScalarVariantType::Name:
		Json->SetStringField(TEXT("v"), Value.Get<FName>().ToString());
		break;
	case EDssScalarVariantType::Tag:
		Json->SetStringField(TEXT("v"), Value.Get<FGameplayTag>().ToString());
		break;
	}

	return Json;
}

bool FDssScalarVariant::FromJson(const TSharedPtr<FJsonValue>& Json)
{
	if (!Json.IsValid() || Json->Type != EJson::Object) { return false; }

	const auto JsonObject = Json->AsObject();

	FString TypeString;
	if (!JsonObject->TryGetStringField(TEXT("t"), TypeString)) { return false; }

	if (!DssCore::TryNameStringToEnum<EDssScalarVariantType>(TypeString, Type)) { return false; }

	FString ValueString;
	switch (Type)
	{
	case EDssScalarVariantType::Bool:
		bool ValueRaw;
		if (!JsonObject->TryGetBoolField(TEXT("v"), ValueRaw)) { return false; }
		Value.Set<bool>(ValueRaw);
		break;
	case EDssScalarVariantType::Int:
		int32 ValueInt;
		if (!JsonObject->TryGetNumberField(TEXT("v"), ValueInt)) { return false; }
		Value.Set<int32>(ValueInt);
		break;
	case EDssScalarVariantType::Float:
		float ValueFloat;
		if (!JsonObject->TryGetNumberField(TEXT("v"), ValueFloat)) { return false; }
		Value.Set<float>(ValueFloat);
		break;
	case EDssScalarVariantType::String:
		if (!JsonObject->TryGetStringField(TEXT("v"), ValueString)) { return false; }
		Value.Set<FString>(ValueString);
		break;
	case EDssScalarVariantType::Name:
		if (!JsonObject->TryGetStringField(TEXT("v"), ValueString)) { return false; }
		Value.Set<FName>(FName(*ValueString));
		break;
	case EDssScalarVariantType::Tag:
		if (!JsonObject->TryGetStringField(TEXT("v"), ValueString)) { return false; }
		const FGameplayTag ParsedTag = FGameplayTag::RequestGameplayTag(*ValueString, false);
		if (!ParsedTag.IsValid()) { return false; }
		Value.Set<FGameplayTag>(ParsedTag);
		break;
	}

	return true;
}

template <typename TKey>
TSharedRef<FJsonObject> FDssScalarVariant::MapToJson(const TDssScalarVariantMap<TKey>& Map, DssCore::TKeyToStringFn<TKey> KeyToString)
{
	auto Json = MakeShared<FJsonObject>();

	for (const auto& Pair : Map)
	{
		const auto ValueJson = Pair.Value.ToJson();
		Json->SetObjectField(KeyToString(Pair.Key), ValueJson);
	}

	return Json;
}

TSharedRef<FJsonObject> FDssScalarVariant::MapToJson(const TDssScalarVariantMap<FGameplayTag>& Map)
{
	return MapToJson<FGameplayTag>(Map, DssCore::GameplayTagToString);
}

template <typename TKey>
bool FDssScalarVariant::MapFromJson(const TSharedPtr<FJsonObject>& Json, DssCore::TStringToKeyFn<TKey> KeyParser,
                                    TDssScalarVariantMap<TKey>& OutMap, FDssErrors* Errors
)
{
	if (!Json.IsValid()) { return false; }

	OutMap.Reset();

	for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : Json->Values)
	{
		TKey Key;
		if (!KeyParser(Pair.Key, Key))
		{
			FDssErrors::AddTo(Errors, TEXT("DssCore.StringToKeyParse"), FString::Printf(TEXT("Failed to parse key '%s'"), *Pair.Key));
			continue;
		}

		if (!Pair.Value.IsValid()) { continue; }
		FDssScalarVariant Value;
		if (!Value.FromJson(Pair.Value))
		{
			continue;
		}

		OutMap.Add(Key, Value);
	}

	return true;
}

bool FDssScalarVariant::MapFromJson(const TSharedPtr<FJsonObject>& Json, TDssScalarVariantMap<FGameplayTag>& OutMap, FDssErrors* Errors)
{
	return MapFromJson<FGameplayTag>(Json, DssCore::StringToGameplayTag, OutMap, Errors);
}
