// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AJ_DialogueSystem : ModuleRules
{
	public AJ_DialogueSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
			}
			);
	}
}
