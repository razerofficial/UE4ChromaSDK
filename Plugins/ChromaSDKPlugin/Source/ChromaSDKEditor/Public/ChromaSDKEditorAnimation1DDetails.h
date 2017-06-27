// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "ChromaSDKEditorAnimationBaseDetails.h"
#include "ChromaSDKEditorButton1D.h"

class FChromaSDKEditorAnimation1DDetails : public IChromaSDKEditorAnimationBaseDetails
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	// device preview row
	void OnClickColor(int element);

protected:

	void CopyPixels(COLORREF* pColor, UINT width, UINT height);

	// import colors from texture image
	FReply OnClickImportTextureImageButton();

	// import colors from a texture animation
	FReply OnClickImportTextureAnimationButton();

	// reset animation length to override
	FReply OnClickOverrideButton();

	// device row
	void OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	FReply OnClickSetDeviceButton();

	// apply row
	FReply OnClickClearButton();
	FReply OnClickFillButton();
	FReply OnClickRandomButton();
	FReply OnClickCopyButton();
	FReply OnClickPasteButton();
	FReply OnClickPreviewButton();
	FReply OnClickPlayButton();
	FReply OnClickStopButton();
	FReply OnClickLoadButton();
	FReply OnClickUnloadButton();

	// device preview row
	TSharedRef<SColorBlock> SetupColorButton(int element, const FLinearColor& color);
	void RefreshDevice();

	// set the color row
	void OnColorCommitted(FLinearColor color);

	// animation frames row
	FReply OnClickPreviousFrame();
	FReply OnClickNextFrame();
	FReply OnClickAddFrame();
	FReply OnClickDeleteFrame();
	void RefreshFrames();

private:

	// instance reference
	TSharedPtr<FChromaSDKEditorAnimation1DDetails> _mDetails;

	// the selected enum device
	EChromaSDKDevice1DEnum _mSelectedDevice;

	// grid buttons hold element
	TArray<TSharedRef<IChromaSDKEditorButton1D>> _mColorButtons;

	// for copy/paste for the grid colors
	TArray<FLinearColor> _mColors;
};

#endif
