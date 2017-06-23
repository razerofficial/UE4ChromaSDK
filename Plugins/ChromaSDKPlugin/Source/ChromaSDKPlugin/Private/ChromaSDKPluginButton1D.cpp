// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginButton1D.h"

TSharedRef<IChromaSDKPluginButton1D> IChromaSDKPluginButton1D::MakeInstance()
{
	return MakeShareable(new IChromaSDKPluginButton1D);
}

TSharedPtr<SColorBlock> IChromaSDKPluginButton1D::CreateColorBlock(const FLinearColor& color)
{
	TSharedRef<SColorBlock> newColor = SNew(SColorBlock)
		.Color(color)
		.OnMouseButtonDown(this, &IChromaSDKPluginButton1D::OnMouseButtonDownColor);

	return newColor;
}

FReply IChromaSDKPluginButton1D::OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer)
{
	//UE_LOG(LogTemp, Log, TEXT("IChromaSDKPluginButton1D::OnMouseButtonDownColor element=%d"), Element);

	if (Details.IsValid())
	{
		FChromaSDKPluginAnimation1DDetails* details = (FChromaSDKPluginAnimation1DDetails*)Details.Get();
		if (details)
		{
			details->OnClickColor(Element);
		}
	}
	return FReply::Handled();
}
