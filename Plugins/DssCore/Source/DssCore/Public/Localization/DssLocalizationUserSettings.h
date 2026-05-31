// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "DssLocalizationUserSettings.generated.h"

UCLASS(Config=GameUserSettings)
class DSSCORE_API UDssLocalizationUserSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config)
	FName CurrentLanguageId = "en";

	void Save()
	{
		SaveConfig();
	}
};
