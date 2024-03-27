// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Targan : ModuleRules
{
	public Targan(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameLiftServerSDK" });
		bEnableExceptions = true;
	}
}
