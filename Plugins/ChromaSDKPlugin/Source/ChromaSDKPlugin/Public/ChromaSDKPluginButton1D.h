// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "ChromaSDKPluginTypes.h"
#include "ChromaSDKPluginAnimation1DDetails.h"
#include "SharedPointer.h"

class IChromaSDKPluginButton1D : public TSharedFromThis<IChromaSDKPluginButton1D>
{
public:

	static TSharedRef<IChromaSDKPluginButton1D> MakeInstance();

	TSharedPtr<SColorBlock> CreateColorBlock(const FLinearColor& color);
	FReply OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer);

	int Element;
	TSharedPtr<IDetailCustomization> Details;
};
