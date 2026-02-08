// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonCoroutine : ModuleRules
{
	public CommonCoroutine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameFeatures",
				"GameplayAbilities",
				"GameplayTags",
				// ... add other public dependencies that you statically link with here ...
			}
			);
	}
}
