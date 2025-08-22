// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;
public class FF_HTTP_WF : ModuleRules
{
	public FF_HTTP_WF(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;
        bEnableExceptions = true;
        bUseRTTI = true;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "OpenSSL",      // We don't need OpenSSL certificate functions
            }
            );
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
            }
			);

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "ThirdParty", "Workflow", "Win64", "include"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "ThirdParty", "Workflow", "Win64", "lib", "workflow.lib"));

            PublicSystemLibraries.Add("Ws2_32.lib");
            PublicSystemLibraries.Add("Mswsock.lib");
            PublicSystemLibraries.Add("AdvApi32.lib");
        }
    }
}