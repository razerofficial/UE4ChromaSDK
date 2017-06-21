// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginButton2D.h"

TSharedRef<IChromaSDKPluginButton2D> IChromaSDKPluginButton2D::MakeInstance()
{
	return MakeShareable(new IChromaSDKPluginButton2D);
}

TSharedPtr<SColorBlock> IChromaSDKPluginButton2D::CreateColorBlock(const FLinearColor& color)
{
	TSharedRef<SColorBlock> newColor = SNew(SColorBlock)
		.Color(color)
		.OnMouseButtonDown(this, &IChromaSDKPluginButton2D::OnMouseButtonDownColor);

	return newColor;
}

FReply IChromaSDKPluginButton2D::OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer)
{
	//UE_LOG(LogTemp, Log, TEXT("IChromaSDKPluginButton2D::OnMouseButtonDownColor row=%d col=%d"), Row, Column);

	if (Details.IsValid())
	{
		FChromaSDKPluginAnimation2DDetails* details = (FChromaSDKPluginAnimation2DDetails*)Details.Get();
		if (details)
		{
			details->OnClickColor(Row, Column);
		}
	}
	return FReply::Handled();
}
