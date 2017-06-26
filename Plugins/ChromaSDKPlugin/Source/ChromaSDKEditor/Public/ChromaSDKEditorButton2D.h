// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "IDetailCustomization.h"
#include "ChromaSDKEditorAnimation2DDetails.h"
#include "ChromaSDKPluginTypes.h"
#include "SharedPointer.h"

class IChromaSDKEditorButton2D : public TSharedFromThis<IChromaSDKEditorButton2D>
{
public:

	static TSharedRef<IChromaSDKEditorButton2D> MakeInstance();

	TSharedPtr<SColorBlock> CreateColorBlock(const FLinearColor& color);
	FReply OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer);

	int Row;
	int Column;
	TSharedPtr<IDetailCustomization> Details;
};

#endif
