// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FYPTraining : ModuleRules
{
	public FYPTraining(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
