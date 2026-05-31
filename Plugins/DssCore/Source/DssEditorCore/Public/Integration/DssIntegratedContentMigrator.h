// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DssIntegratedContentMigrator.generated.h"

USTRUCT()
struct FDssContentMigrationStep
{
	GENERATED_BODY()

	UPROPERTY()
	FString SourceRoot;

	UPROPERTY()
	FString TargetRoot;

	UPROPERTY()
	bool bSkipIfTargetExists = false;
};

UCLASS()
class DSSEDITORCORE_API UDssIntegratedContentMigrator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor, Category="DSS|Integration")
	static bool MigrateToIntegratedPlugin();

private:
	static bool RunMigration(const TArray<FDssContentMigrationStep>& Steps);
	static bool DuplicateRoot(
		const FDssContentMigrationStep& Step,
		TMap<TObjectPtr<UObject>, TObjectPtr<UObject>>& OutReplacementMap
	);

	static void GetAssetsUnderPath(const FString& Root, TArray<FAssetData>& OutAssets);
	static FString MakeTargetPackageName(const FString& SourcePackageName, const FString& SourceRoot, const FString& TargetRoot);
	static UObject* LoadAssetByPackageAndName(const FString& PackageName, const FString& AssetName);
	static bool DuplicateOrMapExistingAsset(
		const FAssetData& SourceAsset,
		const FString& SourceRoot,
		const FString& TargetRoot,
		TMap<TObjectPtr<UObject>, TObjectPtr<UObject>>& OutReplacementMap
	);

	static void ReplaceReferencesInTargetRoots(
		const TArray<FString>& TargetRoots,
		const TMap<TObjectPtr<UObject>, TObjectPtr<UObject>>& ReplacementMap
	);

	static void SaveTargetRoots(const TArray<FString>& TargetRoots);
	static bool ValidateNoOldReferences(const TArray<FString>& TargetRoots, const TArray<FString>& ForbiddenRoots);
};
