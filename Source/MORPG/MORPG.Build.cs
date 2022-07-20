// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MORPG : ModuleRules
{
	public MORPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "UMG" });
        PublicIncludePaths.AddRange(new string[]{"MORPG/Game", "MORPG/Character","MORPG/Items","MORPG/DamageType", "MORPG/Weapon", "MORPG/Components", "MORPG/Interfaces"});
    }
}