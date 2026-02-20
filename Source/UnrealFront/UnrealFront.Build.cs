// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class UnrealFront : ModuleRules
{
	public UnrealFront(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput", 
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] 
			{
				"ProceduralMeshComponent"
			}
		);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		PrivateIncludePaths.AddRange(
			new string[] 
			{
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "LibSWBF2", "include"),
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "lua5.0", "include"),
			}
		);

		PublicAdditionalLibraries.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "LibSWBF2", "build", "libSWBF2.so"),
				Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "lua5.0", "build", "liblua50.so"),
			}
		);
	}
}
