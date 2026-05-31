// Copyright Dark State Studio.All rights reserved.

#include "Integration/DssIntegratedContentMigrator.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Editor.h"
#include "FileHelpers.h"
#include "ObjectTools.h"
#include "Serialization/ArchiveReplaceObjectRef.h"
#include "UObject/Package.h"
#include "UObject/UObjectIterator.h"

bool UDssIntegratedContentMigrator::MigrateToIntegratedPlugin()
{
	// WAŻNE:
	// DssIntegrated plugin musi już istnieć i być enabled,
	// żeby mount point /DssIntegrated był dostępny.

	TArray<FDssContentMigrationStep> Steps;

	Steps.Add({TEXT("/DssVR/DssCore"), TEXT("/DssIntegrated/DssCore"), true});

	// Plugin-specific content.
	Steps.Add({TEXT("/DssVR/DssVR"), TEXT("/DssIntegrated/DssVR"), false});

	return RunMigration(Steps);
}

bool UDssIntegratedContentMigrator::RunMigration(const TArray<FDssContentMigrationStep>& Steps)
{
	auto& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	auto& AssetRegistry = AssetRegistryModule.Get();

	// TODO: need scan ?
	AssetRegistryModule.Get().WaitForCompletion();

	TMap<TObjectPtr<UObject>, TObjectPtr<UObject>> ReplacementMap;
	TArray<FString> TargetRoots;
	TArray<FString> ForbiddenRoots;

	for (const FDssContentMigrationStep& Step : Steps)
	{
		TargetRoots.AddUnique(Step.TargetRoot);
		ForbiddenRoots.AddUnique(Step.SourceRoot);

		if (!DuplicateRoot(Step, ReplacementMap))
		{
			UE_LOG(LogTemp, Error, TEXT("DSS migration failed: %s -> %s"), *Step.SourceRoot, *Step.TargetRoot);
			return false;
		}
	}

	ReplaceReferencesInTargetRoots(TargetRoots, ReplacementMap);
	SaveTargetRoots(TargetRoots);

	const bool bValid = ValidateNoOldReferences(TargetRoots, ForbiddenRoots);

	if (!bValid)
	{
		UE_LOG(LogTemp, Error, TEXT("DSS migration finished with forbidden references. Check logs."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("DSS migration finished successfully."));
	return true;
}

bool UDssIntegratedContentMigrator::DuplicateRoot(
	const FDssContentMigrationStep& Step,
	TMap<TObjectPtr<UObject>, TObjectPtr<UObject>>& OutReplacementMap)
{
	TArray<FAssetData> Assets;
	GetAssetsUnderPath(Step.SourceRoot, Assets);

	if (Assets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DSS migration: no assets under %s"), *Step.SourceRoot);
		return true;
	}

	UE_LOG(LogTemp, Display, TEXT("DSS migration: duplicating %d assets from %s to %s"),
	       Assets.Num(),
	       *Step.SourceRoot,
	       *Step.TargetRoot
	);

	bool bOk = true;

	for (const FAssetData& Asset : Assets)
	{
		if (!DuplicateOrMapExistingAsset(Asset, Step.SourceRoot, Step.TargetRoot, OutReplacementMap))
		{
			bOk = false;
		}
	}

	return bOk;
}

void UDssIntegratedContentMigrator::GetAssetsUnderPath(const FString& Root, TArray<FAssetData>& OutAssets)
{
	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	FARFilter Filter;
	Filter.PackagePaths.Add(*Root);
	Filter.bRecursivePaths = true;
	Filter.bIncludeOnlyOnDiskAssets = false;

	AssetRegistryModule.Get().GetAssets(Filter, OutAssets);
}

FString UDssIntegratedContentMigrator::MakeTargetPackageName(
	const FString& SourcePackageName,
	const FString& SourceRoot,
	const FString& TargetRoot)
{
	FString Relative = SourcePackageName;

	if (!Relative.RemoveFromStart(SourceRoot))
	{
		return SourcePackageName;
	}

	return TargetRoot + Relative;
}

UObject* UDssIntegratedContentMigrator::LoadAssetByPackageAndName(
	const FString& PackageName,
	const FString& AssetName)
{
	const FString ObjectPath = PackageName + TEXT(".") + AssetName;
	return LoadObject<UObject>(nullptr, *ObjectPath);
}

bool UDssIntegratedContentMigrator::DuplicateOrMapExistingAsset(
	const FAssetData& SourceAsset,
	const FString& SourceRoot,
	const FString& TargetRoot,
	TMap<TObjectPtr<UObject>, TObjectPtr<UObject>>& OutReplacementMap)
{
	UObject* SourceObject = SourceAsset.GetAsset();

	if (!SourceObject)
	{
		UE_LOG(LogTemp, Error, TEXT("DSS migration: cannot load source asset: %s"),
		       *SourceAsset.GetObjectPathString());
		return false;
	}

	const FString SourcePackageName = SourceAsset.PackageName.ToString();
	const FString SourceAssetName = SourceAsset.AssetName.ToString();

	const FString TargetPackageName = MakeTargetPackageName(SourcePackageName, SourceRoot, TargetRoot);
	const FString TargetPath = FPackageName::GetLongPackagePath(TargetPackageName);
	const FString TargetAssetName = SourceAssetName;

	UObject* ExistingTarget = LoadAssetByPackageAndName(TargetPackageName, TargetAssetName);

	if (ExistingTarget)
	{
		OutReplacementMap.Add(SourceObject, ExistingTarget);

		UE_LOG(LogTemp, Display, TEXT("DSS migration: mapped existing asset: %s -> %s"),
		       *SourceObject->GetPathName(),
		       *ExistingTarget->GetPathName());

		return true;
	}

	FAssetToolsModule& AssetToolsModule =
		FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	UObject* Duplicated = AssetToolsModule.Get().DuplicateAsset(
		TargetAssetName,
		TargetPath,
		SourceObject
	);

	if (!Duplicated)
	{
		UE_LOG(LogTemp, Error, TEXT("DSS migration: duplicate failed: %s -> %s"),
		       *SourceObject->GetPathName(),
		       *TargetPackageName);

		return false;
	}

	OutReplacementMap.Add(SourceObject, Duplicated);

	if (UPackage* Package = Duplicated->GetOutermost())
	{
		Package->MarkPackageDirty();
	}

	UE_LOG(LogTemp, Display, TEXT("DSS migration: duplicated: %s -> %s"),
	       *SourceObject->GetPathName(),
	       *Duplicated->GetPathName());

	return true;
}

void UDssIntegratedContentMigrator::ReplaceReferencesInTargetRoots(
	const TArray<FString>& TargetRoots,
	const TMap<TObjectPtr<UObject>, TObjectPtr<UObject>>& ReplacementMap)
{
	if (ReplacementMap.Num() == 0)
	{
		return;
	}

	TArray<FAssetData> TargetAssets;

	for (const FString& TargetRoot : TargetRoots)
	{
		GetAssetsUnderPath(TargetRoot, TargetAssets);
	}

	UE_LOG(LogTemp, Display, TEXT("DSS migration: replacing references in %d target assets."), TargetAssets.Num());

	TMap<UObject*, UObject*> RawReplacementMap;

	for (const auto& Pair : ReplacementMap)
	{
		if (Pair.Key && Pair.Value)
		{
			RawReplacementMap.Add(Pair.Key.Get(), Pair.Value.Get());
		}
	}

	for (const FAssetData& AssetData : TargetAssets)
	{
		UObject* AssetObject = AssetData.GetAsset();

		if (!AssetObject)
		{
			continue;
		}

		UPackage* Package = AssetObject->GetOutermost();

		if (!Package)
		{
			continue;
		}

		TArray<UObject*> ObjectsInPackage;
		GetObjectsWithOuter(Package, ObjectsInPackage, true);

		bool bPackageChanged = false;

		for (UObject* ObjectInPackage : ObjectsInPackage)
		{
			if (!ObjectInPackage)
			{
				continue;
			}

			FArchiveReplaceObjectRef<UObject> ReplaceArchive(
				ObjectInPackage,
				RawReplacementMap,
				EArchiveReplaceObjectFlags::IgnoreOuterRef |
				EArchiveReplaceObjectFlags::IgnoreArchetypeRef
			);

			if (ReplaceArchive.GetCount() > 0)
			{
				ObjectInPackage->Modify();
				ObjectInPackage->MarkPackageDirty();
				bPackageChanged = true;
			}
		}

		if (bPackageChanged)
		{
			Package->MarkPackageDirty();

			UE_LOG(LogTemp, Display, TEXT("DSS migration: remapped refs in package: %s"),
			       *Package->GetName());
		}
	}
}

void UDssIntegratedContentMigrator::SaveTargetRoots(const TArray<FString>& TargetRoots)
{
	TArray<UPackage*> PackagesToSave;

	for (const FString& TargetRoot : TargetRoots)
	{
		TArray<FAssetData> Assets;
		GetAssetsUnderPath(TargetRoot, Assets);

		for (const FAssetData& AssetData : Assets)
		{
			UObject* Asset = AssetData.GetAsset();

			if (!Asset)
			{
				continue;
			}

			UPackage* Package = Asset->GetOutermost();

			if (Package && Package->IsDirty())
			{
				PackagesToSave.AddUnique(Package);
			}
		}
	}

	if (PackagesToSave.Num() > 0)
	{
		UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, true);
	}
}

bool UDssIntegratedContentMigrator::ValidateNoOldReferences(const TArray<FString>& TargetRoots, const TArray<FString>& ForbiddenRoots)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	auto& AssetRegistry = AssetRegistryModule.Get();

	// TODO: impl.s
	AssetRegistry.WaitForCompletion();
	//AssetRegistryModule.Get().ScanAllSynchronous();

	bool bOk = true;

	TArray<FAssetData> TargetAssets;

	for (const FString& TargetRoot : TargetRoots)
	{
		GetAssetsUnderPath(TargetRoot, TargetAssets);
	}

	for (const FAssetData& AssetData : TargetAssets)
	{
		TArray<FName> Dependencies;

		AssetRegistryModule.Get().GetDependencies(
			AssetData.PackageName,
			Dependencies,
			UE::AssetRegistry::EDependencyCategory::Package,
			UE::AssetRegistry::EDependencyQuery::Hard
		);

		for (const FName& DependencyName : Dependencies)
		{
			const FString Dep = DependencyName.ToString();

			for (const FString& ForbiddenRoot : ForbiddenRoots)
			{
				if (Dep.StartsWith(ForbiddenRoot))
				{
					UE_LOG(LogTemp, Error, TEXT("DSS migration invalid reference: %s depends on %s"),
					       *AssetData.PackageName.ToString(),
					       *Dep);

					bOk = false;
				}
			}
		}
	}

	return bOk;
}
