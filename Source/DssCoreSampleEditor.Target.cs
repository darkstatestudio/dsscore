// Dark State Studio

using UnrealBuildTool;

public class DssCoreSampleEditorTarget : TargetRules
{
    public DssCoreSampleEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V7;

        ExtraModuleNames.AddRange(new[] { "DssCoreSample" });
    }
}
