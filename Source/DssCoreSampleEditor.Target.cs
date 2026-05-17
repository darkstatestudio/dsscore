// Dark State Studio

using UnrealBuildTool;
using System.Collections.Generic;

public class DssCoreSampleEditorTarget : TargetRules
{
	public DssCoreSampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "DssCoreSample" } );
	}
}
