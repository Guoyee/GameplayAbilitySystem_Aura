using UnrealBuildTool;
using System.Collections.Generic;

public class AuraEditorTarget : TargetRules
{
    public AuraEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.AddRange(new string[] { "Aura" });
    }
}
