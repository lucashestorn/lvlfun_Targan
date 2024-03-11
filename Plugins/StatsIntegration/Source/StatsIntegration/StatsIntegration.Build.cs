// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StatsIntegration : ModuleRules
{
	public StatsIntegration(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "CoreUObject", "OnlineSubsystemUtils", "Networking", "Sockets" });
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "Sockets", "Networking", "OnlineSubsystemUtils" });

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Linux) || (Target.Platform == UnrealTargetPlatform.Mac))
        {
            PublicDependencyModuleNames.AddRange(new string[] { "Steamworks", "OnlineSubsystemSteam" });
            PublicIncludePaths.AddRange(new string[] { "../Plugins/Online/OnlineSubsystemSteam/Source/Private" });
        }
    }
}
