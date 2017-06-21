// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPluginTypes.h"

class IChromaSDKPluginButton2D : public TSharedFromThis<IChromaSDKPluginButton2D>
{
public:

	static TSharedRef<IChromaSDKPluginButton2D> MakeInstance();

	TSharedPtr<SColorBlock> CreateColorBlock(const FLinearColor& color);
	FReply OnMouseButtonDownColor(const FGeometry& geometry, const FPointerEvent& pointer);

	int Row;
	int Column;
};
