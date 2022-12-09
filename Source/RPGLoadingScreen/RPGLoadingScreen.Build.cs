using UnrealBuildTool; 

public class RPGLoadingScreen: ModuleRules
{
	public RPGLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivatePCHHeaderFile = "Public/RPGLoadingScreenModule.h";

		PCHUsage = PCHUsageMode.UseSharedPCHs;
		
		PrivateIncludePaths.Add("RPGLoadingScreen/Private");
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
			
		PrivateDependencyModuleNames.AddRange(new string[] {"MoviePlayer", "Slate", "SlateCore", "InputCore" });
	}
}