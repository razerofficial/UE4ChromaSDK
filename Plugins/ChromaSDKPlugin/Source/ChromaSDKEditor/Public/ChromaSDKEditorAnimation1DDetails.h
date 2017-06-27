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

	void RefreshFrames();

	void RefreshDevice();

	// enum dropdown handler
	TSharedRef<SWidget> GenerateDropdownEnum(TSharedPtr<FString> InItem);

	// device row
	void OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	FReply OnClickSetDeviceButton();

	// device preview row
	TSharedRef<SColorBlock> SetupColorButton(int element, const FLinearColor& color);
	void OnClickColor(int element);

	// apply row
	FReply OnClickClearButton();
	FReply OnClickFillButton();
	FReply OnClickRandomButton();
	FReply OnClickCopyButton();
	FReply OnClickPreviewButton();
	FReply OnClickPlayButton();
	FReply OnClickStopButton();
	FReply OnClickLoadButton();
	FReply OnClickUnloadButton();

	// set the color row
	void OnColorCommitted(FLinearColor color);

	// animation frames row
	FReply OnClickPreviousFrame();
	FReply OnClickNextFrame();
	FReply OnClickDeleteFrame();

	// import colors from texture image
	FReply OnClickImportTextureImageButton();

	// import colors from a texture animation
	FReply OnClickImportTextureAnimationButton();

	// reset animation length to override
	FReply OnClickOverrideButton();

protected:

	void CopyPixels(COLORREF* pColor, UINT width, UINT height);

	// apply row
	FReply OnClickPasteButton();

	// animation frames row
	FReply OnClickAddFrame();

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
