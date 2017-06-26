// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#if WITH_EDITOR

#include "ChromaSDKEditorButton1D.h"

TSharedRef<IChromaSDKEditorButton1D> IChromaSDKEditorButton1D::MakeInstance()
{
	return MakeShareable(new IChromaSDKEditorButton1D);
}

TSharedPtr<SColorBlock> IChromaSDKEditorButton1D::CreateColorBlock(const FLinearColor& color)
{
	TSharedRef<SColorBlock> newColor = SNew(SColorBlock)
		.Color(color)
		.OnMouseButtonDown(this, &IChromaSDKEditorButton1D::OnMouseButtonDownColor);

	return newColor;
}

FReply IChromaSDKEditorButton1D::OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer)
{
	//UE_LOG(LogTemp, Log, TEXT("IChromaSDKEditorButton1D::OnMouseButtonDownColor element=%d"), Element);

	if (Details.IsValid())
	{
		FChromaSDKEditorAnimation1DDetails* details = (FChromaSDKEditorAnimation1DDetails*)Details.Get();
		if (details)
		{
			details->OnClickColor(Element);
		}
	}
	return FReply::Handled();
}

#endif
