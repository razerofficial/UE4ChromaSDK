// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//#include "UE4ChromaSDK.h" //(support 4.15 or below)___HACK_UE4_WANTS_MODULE_FIRST
#include "GameExampleBPLibrary.h"
#include "ChromaSDKPluginBPLibrary.h"
#include "UE4ChromaSDK.h" //(support 4.16 or above)___HACK_UE4_WANTS_HEADER_FIRST

UGameExampleBPLibrary::UGameExampleBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UGameExampleBPLibrary::PlayCompositeRandom()
{
	UChromaSDKPluginBPLibrary::PlayAnimationComposite(TEXT("Random"), false);
}
