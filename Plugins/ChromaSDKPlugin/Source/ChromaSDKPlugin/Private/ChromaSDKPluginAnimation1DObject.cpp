// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

UChromaSDKPluginAnimation1DObject::UChromaSDKPluginAnimation1DObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// start with 1 frame
	FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
	frame.Colors = UChromaSDKPluginBPLibrary::CreateColors1D(Device);
	Frames.Add(frame);
}

#if WITH_EDITOR
void UChromaSDKPluginAnimation1DObject::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::PostEditChangeChainProperty"));

	int maxLeds = UChromaSDKPluginBPLibrary::GetMaxLeds(Device);
	for (int i = 0; i < Frames.Num(); ++i)
	{
		FChromaSDKColorFrame1D& frame = Frames[i];
		FChromaSDKColors& leds = frame.Colors;
		if (leds.Colors.Num() != maxLeds)
		{
			frame.Colors = UChromaSDKPluginBPLibrary::CreateColors1D(Device);
		}
	}
}
#endif
