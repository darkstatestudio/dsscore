#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetUserData.h"
#include "AssetSourceInfo.generated.h"

UCLASS(BlueprintType)
class UAssetSourceInfo : public UAssetUserData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString SourcePack;

    UPROPERTY(EditAnywhere)
    FString SourceURL;

    UPROPERTY(EditAnywhere)
    FString Notes;

    UPROPERTY(EditAnywhere)
    TMap<FString, FString> Tags;
};
