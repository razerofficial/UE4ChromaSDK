// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKEditor.h"

#if WITH_EDITOR

#include "UnrealEd.h"
#include "Runtime/Launch/Resources/Version.h"
#include "IDetailCustomization.h"
#include "ChromaSDKEditorAnimation1DDetails.h"
#include "ChromaSDKPluginTypes.h"
#include "SharedPointer.h"

class IChromaSDKEditorButton1D : public TSharedFromThis<IChromaSDKEditorButton1D>
{
public:

	static TSharedRef<IChromaSDKEditorButton1D> MakeInstance();

	TSharedPtr<SColorBlock> CreateColorBlock(const FLinearColor& color);
	FReply OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer);

	int Element;
	TSharedPtr<IDetailCustomization> Details;
};

#endif
