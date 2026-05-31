using UnrealBuildTool;

public class DssEditorCore : ModuleRules
{
    public DssEditorCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange([
            "Core",
            "DssCore"
        ]);

        PrivateDependencyModuleNames.AddRange([
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "UnrealEd",
            "AssetTools",
            "AssetRegistry",
            "Projects"
        ]);
    }
}
