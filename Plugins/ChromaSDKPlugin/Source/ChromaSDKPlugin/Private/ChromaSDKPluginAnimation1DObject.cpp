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
