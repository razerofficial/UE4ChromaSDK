// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "IDetailCustomization.h"
#include "ChromaSDKPluginTypes.h"
#include "Widgets/Layout/SGridPanel.h"

class IChromaSDKEditorAnimationBaseDetails : public IDetailCustomization
{
protected:

	void ReadImage(const FString& path, bool isAnimation);

	virtual void CopyPixels(COLORREF* pColor, UINT width, UINT height) = 0;

	// apply row
	virtual FReply OnClickPasteButton() = 0;

	// animation frames row
	virtual FReply OnClickAddFrame() = 0;

	// object being inspected
	TArray<TWeakObjectPtr<UObject>> _mObjectsBeingCustomized;

	// the device enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKDevices;

	// device grid
	TSharedPtr<SGridPanel> _mGrid;

	// current color in the color picker
	FLinearColor _mColor;

	// animation controls
	int _mCurrentFrame;
	TSharedPtr<STextBlock> _mTextCurrentFrame;
	TSharedPtr<STextBlock> _mTextNumberOfFrames;
	TSharedPtr<STextBlock> _mTextFrameDuration;
};

#endif
