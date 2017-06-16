// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

UChromaSDKPluginAnimation2DObject::UChromaSDKPluginAnimation2DObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void UChromaSDKPluginAnimation2DObject::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation2DObject::PostEditChangeChainProperty"));
	for (int i = 0; i < Frames.Num(); ++i)
	{
		FChromaSDKColorFrame2D& frame = Frames[i];
		frame.Colors = UChromaSDKPluginBPLibrary::CreateRandomColors2D(Device);
	}
}
#endif
