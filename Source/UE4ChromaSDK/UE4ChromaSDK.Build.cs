// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UE4ChromaSDK : ModuleRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
	public UE4ChromaSDK(ReadOnlyTargetRules Target) : base(Target)
#else
    public UE4ChromaSDK(TargetInfo Target) //4.15 or lower
#endif
	{
#if WITH_FORWARDED_MODULE_RULES_CTOR
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
#endif
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ChromaSDKPlugin" });


        PrivateDependencyModuleNames.AddRange(new string[] { "ChromaSDKPlugin" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
