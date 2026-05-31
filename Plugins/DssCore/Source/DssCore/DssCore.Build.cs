// Copyright Dark State Studio.All rights reserved.

using UnrealBuildTool;

public class DssCore : ModuleRules
{
    public DssCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[]
            {
                // ... add public include paths required here ...
            }
        );


        PrivateIncludePaths.AddRange(
            [
            ]
        );


        PublicDependencyModuleNames.AddRange(
            [
                "Core", "UMG", "DeveloperSettings"
            ]
        );


        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            ]
        );


        DynamicallyLoadedModuleNames.AddRange(
            [
            ]
        );
    }
}
