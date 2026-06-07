#pragma once
#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

using FJsonValueCallback = TFunction<void(const TSharedPtr<FJsonValue>& V)>;

class FJsonHelper
{
public:
	static bool TryProcessArrayField(
		const TSharedPtr<FJsonObject>& Obj,
		const FStringView FieldName,
		const FJsonValueCallback& OnItem
	)
	{
		const TArray<TSharedPtr<FJsonValue>>* TmpArray;
		if (Obj->TryGetArrayField(FieldName, TmpArray))
		{
			for (auto& V : *TmpArray)
			{
				OnItem(V);
			}

			return true;
		}

		return false;
	}

	static TSharedPtr<FJsonObject> LoadJsonFromString(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> Out;
		const auto Reader = TJsonReaderFactory<>::Create(JsonString);
		FJsonSerializer::Deserialize(Reader, Out);
		return Out;
	}

	static TSharedPtr<FJsonObject> LoadJsonFromString(const FString& JsonString, FString& OutErrorMessage)
	{
		TSharedPtr<FJsonObject> Out;
		const auto Reader = TJsonReaderFactory<>::Create(JsonString);
		if (!FJsonSerializer::Deserialize(Reader, Out))
		{
			OutErrorMessage = Reader->GetErrorMessage();
		}

		return Out;
	}

	static TSharedPtr<FJsonObject> NormalizeKeysToPascalCase(const TSharedPtr<FJsonObject>& InObj)
	{
		TSharedPtr<FJsonObject> OutObj = MakeShared<FJsonObject>();

		for (const auto& Pair : InObj->Values)
		{
			FString Key = Pair.Key;
			if (Key.Len() > 0)
			{
				Key[0] = FChar::ToUpper(Key[0]); // camelCase -> PascalCase
			}

			OutObj->SetField(Key, Pair.Value);
		}

		return OutObj;
	}

	FORCEINLINE static FString JsonToCompactString(const TSharedPtr<FJsonObject>& Json)
	{
		FString OutString;
		if (!Json.IsValid()) { return OutString; }

		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutString);
		FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

		return OutString;
	}
};
