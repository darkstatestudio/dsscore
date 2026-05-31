// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameLanguageSettingsAsset.h"
#include "DssLocalizationSettings.generated.h"

UCLASS(Config=Game, DefaultConfig)
class DSSCORE_API UDssLocalizationSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDssLocalizationSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
	{
		CategoryName = "DssCore";
		SectionName = "Localization";
	}

#if WITH_EDITOR
	virtual FText GetSectionText() const override
	{
		return FText::FromString("Localization");
	}
#endif

	UPROPERTY(Config, EditAnywhere)
	FName FallbackLanguageId = "en";

	UPROPERTY(Config, EditAnywhere)
	TArray<FGameLanguageDef> Languages;
};
