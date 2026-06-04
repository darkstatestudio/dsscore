// Copyright Dark State Studio.All rights reserved.

using UnrealBuildTool;

public class DssTestCore : ModuleRules
{
    public DssTestCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "DssCore",
                "Json"
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DssCore"
            ]
        );
    }
}
