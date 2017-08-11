// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//#include "ChromaSDKEditor.h" //(support 4.15 or below)___HACK_UE4_WANTS_MODULE_FIRST
#include "ChromaSDKEditorButton2D.h"
#include "ChromaSDKEditor.h" //(support 4.16 or above)___HACK_UE4_WANTS_HEADER_FIRST

#if WITH_EDITOR

TSharedRef<IChromaSDKEditorButton2D> IChromaSDKEditorButton2D::MakeInstance()
{
	return MakeShareable(new IChromaSDKEditorButton2D);
}

TSharedPtr<SColorBlock> IChromaSDKEditorButton2D::CreateColorBlock(const FLinearColor& color)
{
	TSharedRef<SColorBlock> newColor = SNew(SColorBlock)
		.Color(color)
		.OnMouseButtonDown(this, &IChromaSDKEditorButton2D::OnMouseButtonDownColor);

	return newColor;
}

FReply IChromaSDKEditorButton2D::OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer)
{
	//UE_LOG(LogTemp, Log, TEXT("IChromaSDKEditorButton2D::OnMouseButtonDownColor row=%d col=%d"), Row, Column);

	if (Details.IsValid())
	{
		FChromaSDKEditorAnimation2DDetails* details = (FChromaSDKEditorAnimation2DDetails*)Details.Get();
		if (details)
		{
			details->OnClickColor(Row, Column);
		}
	}
	return FReply::Handled();
}

#endif
