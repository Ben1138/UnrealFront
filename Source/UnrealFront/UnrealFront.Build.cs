// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class UnrealFront : ModuleRules
{
	public UnrealFront(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "LibSWBF2", "LibSWBF2"),
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "lua5.0-swbf2-x64", "include"),
			}
		);

		PublicAdditionalLibraries.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "LibSWBF2", "LibSWBF2", "build", "libSWBF2.a"),
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "lua5.0", "lib", "liblua50.a"),
			}
		);
	}
}
