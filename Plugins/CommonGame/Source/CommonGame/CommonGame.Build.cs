// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonGame : ModuleRules
{
	public CommonGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"NetCore",
				"ModularGameplay",
				"ModularGameplayActors",
				"CommonCoroutine",
				
				"DeveloperTools",
				
				"GameFeatures",
				"EnhancedInput",
				"GameplayTags"
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
