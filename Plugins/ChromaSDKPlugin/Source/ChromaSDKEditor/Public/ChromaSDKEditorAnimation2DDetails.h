// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "ChromaSDKEditorAnimationBaseDetails.h"
#include "ChromaSDKEditorButton2D.h"

class FChromaSDKEditorAnimation2DDetails : public IChromaSDKEditorAnimationBaseDetails
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
	TSharedRef<SColorBlock> SetupColorButton(int row, int column, const FLinearColor& color);
	void OnClickColor(int row, int column);

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

	// select a key row
	bool IsEnabledKeyboardKey() const;
	void OnChangeChromaSDKKeyboardKeys(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	FReply OnClickSetKeyButton();

	// select an led row
	bool IsEnabledMouseLed() const;
	void OnChangeChromaSDKMouseLeds(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	FReply OnClickSetLedButton();

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
	TSharedPtr<FChromaSDKEditorAnimation2DDetails> _mDetails;

	// the device enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKDevices;

	// the selected enum device
	EChromaSDKDevice2DEnum _mSelectedDevice;

	// grid buttons hold row/column
	TArray<TSharedRef<IChromaSDKEditorButton2D>> _mColorButtons;

	// select a key, enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKKeyboardKeys;

	// select a key, selected enum
	EChromaSDKKeyboardKey _mSelectedKey;

	// select an led, enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKMouseLeds;

	// select an led, selected enum
	EChromaSDKMouseLed _mSelectedLed;
	
	// for copy/paste for the grid colors
	TArray<FChromaSDKColors> _mColors;
};

#endif
