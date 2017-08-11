// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_FORWARDED_MODULE_RULES_CTOR
#define ENGINE_4_16_OR_NEWER
#endif

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4ChromaSDKTarget : TargetRules
{
#if ENGINE_4_16_OR_NEWER
    public UE4ChromaSDKTarget(TargetInfo Target) : base(Target)
#else
    public UE4ChromaSDKTarget(TargetInfo Target)
#endif
    {
        Type = TargetType.Game;

#if ENGINE_4_16_OR_NEWER
        ExtraModuleNames.AddRange( new string[] { "UE4ChromaSDK" } );
#endif
	}

#if !ENGINE_4_16_OR_NEWER
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
