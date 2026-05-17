#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TextDataAsset.generated.h"


UCLASS(BlueprintType)
class DSSCORE_API UTextDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data")
	FString Text;

	FPrimaryAssetId virtual GetPrimaryAssetId() const override { return FPrimaryAssetId("AssetText", GetFName()); }
};
