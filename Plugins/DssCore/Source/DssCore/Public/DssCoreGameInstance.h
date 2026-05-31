// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Localization/DssLanguageSubsystem.h"
#include "DssCoreGameInstance.generated.h"

UCLASS()
class DSSCORE_API UDssCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 RandomSeed = 0;

	UFUNCTION(BlueprintCallable)
	FRandomStream GetRandomStream() const
	{
		return FRandomStream(RandomSeed);
	}

	UDssLanguageSubsystem* GetLanguageSubsystem() const
	{
		return GetSubsystem<UDssLanguageSubsystem>();
	}
};
