// Copyright Dark State Studio.All rights reserved.

#include "Localization/DssLanguageSubsystem.h"

#include "Localization/DssLocalizationSettings.h"
#include "Internationalization/Culture.h"
#include "Localization/DssLocalizationUserSettings.h"

void UDssLanguageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LanguageById.Reset();
	LanguageIdByCulture.Reset();

	const auto Settings = GetDefault<UDssLocalizationSettings>();
	if (!Settings)
	{
		return;
	}

	FallbackLanguageId = Settings->FallbackLanguageId.IsNone()
		                     ? FName("en")
		                     : Settings->FallbackLanguageId;
}

FString UDssLanguageSubsystem::GetCurrentCulture() const
{
	if (const FCulturePtr Culture = FInternationalization::Get().GetCurrentCulture())
	{
		return Culture->GetName();
	}

	return TEXT("en");
}

FName UDssLanguageSubsystem::GetLanguageIdFromCulture(const FString& Culture) const
{
	const FString Normalized = Culture.ToLower();

	if (const FName* Found = LanguageIdByCulture.Find(Normalized))
	{
		return *Found;
	}

	const int32 DashIndex = Normalized.Find(TEXT("-"));

	if (DashIndex != INDEX_NONE)
	{
		const FString ShortCulture = Normalized.Left(DashIndex);

		if (const FName* ShortFound = LanguageIdByCulture.Find(ShortCulture))
		{
			return *ShortFound;
		}
	}

	return FallbackLanguageId;
}

FName UDssLanguageSubsystem::GetCurrentLanguageId() const
{
	const FString Culture = FInternationalization::Get().GetCurrentCulture()->GetName();
	return GetLanguageIdFromCulture(Culture);
}

bool UDssLanguageSubsystem::IsLanguageSupported(FName LanguageId) const
{
	return LanguageById.Contains(LanguageId);
}

const FGameLanguageDef* UDssLanguageSubsystem::FindLanguage(FName LanguageId) const
{
	return LanguageById.Find(LanguageId);
}

FText UDssLanguageSubsystem::GetLanguageDisplayName(FName LanguageId) const
{
	if (const auto Lang = FindLanguage(LanguageId))
	{
		return Lang->DisplayName;
	}

	return FText::FromName(FallbackLanguageId);
}

TArray<FGameLanguageDef> UDssLanguageSubsystem::GetSupportedLanguages() const
{
	TArray<FGameLanguageDef> Result;
	LanguageById.GenerateValueArray(Result);
	return Result;
}

bool UDssLanguageSubsystem::ApplyLanguage(FName LanguageId, bool bSave)
{
	const auto Lang = FindLanguage(LanguageId);
	if (!Lang)
	{
		return false;
	}

	FString CultureToSet = Lang->Culture;
	if (CultureToSet.IsEmpty())
	{
		return false;
	}

	const bool bSuccess = FInternationalization::Get().SetCurrentCulture(CultureToSet);
	if (!bSuccess)
	{
		return false;
	}

	CurrentLanguageId = LanguageId;
	if (bSave)
	{
		auto UserSettings = GetMutableDefault<UDssLocalizationUserSettings>();
		UserSettings->CurrentLanguageId = LanguageId;
		UserSettings->SaveConfig();
	}

	OnLanguageChanged.Broadcast(LanguageId);

	return true;
}
