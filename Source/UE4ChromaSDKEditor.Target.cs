// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4ChromaSDKEditorTarget : TargetRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
	public UE4ChromaSDKEditorTarget(TargetInfo Target) : base(Target)
#else
	public UE4ChromaSDKEditorTarget(TargetInfo Target)
#endif
	{
		Type = TargetType.Editor;

#if WITH_FORWARDED_MODULE_RULES_CTOR
		ExtraModuleNames.AddRange( new string[] { "UE4ChromaSDK" } );
#endif
	}

#if !WITH_FORWARDED_MODULE_RULES_CTOR
	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "UE4ChromaSDK" } );
	}
#endif
}

