// Copyright Dark State Studio.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameLanguageSettingsAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DssLanguageSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDssOnLanguageChanged, FName, LanguageId);

/**
 *
 */
UCLASS()
class DSSCORE_API UDssLanguageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintPure)
	FName GetCurrentLanguageId() const;

	UFUNCTION(BlueprintPure)
	FName GetLanguageIdFromCulture(const FString& Culture) const;

	UFUNCTION(BlueprintPure)
	bool IsLanguageSupported(FName LanguageId) const;

	const FGameLanguageDef* FindLanguage(FName LanguageId) const;

	UFUNCTION(BlueprintPure)
	TArray<FGameLanguageDef> GetSupportedLanguages() const;

	UFUNCTION(BlueprintPure)
	FText GetLanguageDisplayName(FName LanguageId) const;

	UFUNCTION(BlueprintPure)
	FString GetCurrentCulture() const;

	UFUNCTION(BlueprintCallable)
	bool ApplyLanguage(FName LanguageId, bool bSave);

	UPROPERTY(BlueprintAssignable)
	FDssOnLanguageChanged OnLanguageChanged;

private:
	UPROPERTY()
	TMap<FName, FGameLanguageDef> LanguageById;

	UPROPERTY()
	TMap<FString, FName> LanguageIdByCulture;

	UPROPERTY()
	FName FallbackLanguageId = "en";

	UPROPERTY()
	FName CurrentLanguageId;
};
