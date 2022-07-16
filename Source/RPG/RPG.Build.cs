// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG : ModuleRules
{
	public RPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "EnhancedInput", "GameplayAbilities", "GameplayTags", "GameplayTasks", "InputCore", "InteractionSystem", "InventorySystem", "QuestSystem", "Slate", "UMG", "FXManager"});
	}
}
