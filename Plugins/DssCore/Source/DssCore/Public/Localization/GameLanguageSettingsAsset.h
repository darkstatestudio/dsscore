// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameLanguageSettingsAsset.generated.h"

USTRUCT(BlueprintType)
struct FGameLanguageDef
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LanguageId; // "pl", "en", "de", "fr"

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Culture; // "pl-PL", "en-US", "de-DE"

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName; // Polski, English, Deutsch

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsSupported = true;
};

UCLASS(BlueprintType)
class DSSCORE_API UGameLanguageSettingsAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameLanguageDef> Languages;
};
