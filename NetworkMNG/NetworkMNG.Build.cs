// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class NetworkMNG : ModuleRules
{
	public NetworkMNG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "Sockets", "Networking","UMG"});

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // var basePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));
        //ModuleDirectory
        //PublicIncludePaths.AddRange(new string[] { ModuleDirectory+"../../Source/Include" });
        //PublicIncludePaths.Add(Path.Combine(basePath, "Public")+"../../Include");

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
