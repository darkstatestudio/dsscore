#include "DssEditorCore.h"
#include "AssetRegistry/AssetData.h"
#include "Util/Data/AssetSourceInfo.h"
#include "ContentBrowserModule.h"

#define LOCTEXT_NAMESPACE "FDssEditorCoreModule"

void FDssEditorCoreModule::StartupModule()
{

 /*
	auto CBModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	auto Generator = MakeShared<FAssetViewExtraStateGenerator>(nullptr, nullptr);

	Generator->ToolTipGenerator = FOnGenerateAssetViewExtraStateIndicators::CreateLambda(
		[](const FAssetData& AssetData)
		{
			const UObject* Asset = AssetData.GetAsset();
			const UAssetSourceInfo* Info = Asset ? Asset->GetDetailedInfo()<UAssetSourceInfo>() : nullptr;
			if (!Info)
				return TSharedPtr<SWidget>(nullptr);

			return SNew(STextBlock)
				.Text(FText::FromString(TEXT("🔖")))   // mała ikonka
				.ToolTipText(FText::FromString(Info->SourcePack));
		});

	Generator->OnGenerateExtraStateToolTip = FOnGenerateAssetViewExtraStateToolTip::CreateLambda(
		[](const FAssetData& AssetData)
		{
			const UObject* Asset = AssetData.GetAsset();
			const UAssetSourceInfo* Info = Asset ? Asset->GetAssetUserData<UAssetSourceInfo>() : nullptr;
			if (!Info)
				return TSharedPtr<SToolTip>(nullptr);

			return SNew(SToolTip)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("Source Pack: %s"), *Info->SourcePack)))
					]
					+ SVerticalBox::Slot().AutoHeight()
					[
						SNew(SHyperlink)
						.Text(FText::FromString(TEXT("Open URL")))
						.OnNavigate_Lambda([Info]()
						{
							if (!Info->SourceURL.IsEmpty())
								FPlatformProcess::LaunchURL(*Info->SourceURL, nullptr, nullptr);
						})
					]
				];
		});

	CBModule.AddAssetViewExtraStateGenerator(Generator);
	*/
}

void FDssEditorCoreModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDssEditorCoreModule, DssEditorCore)
