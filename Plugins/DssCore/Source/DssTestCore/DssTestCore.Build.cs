// Copyright Dark State Studio.All rights reserved.
using UnrealBuildTool;

public class DssTestCore : ModuleRules
{
    public DssTestCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "DssCore",
                "Json"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DssCore"
            }
        );
    }
}
