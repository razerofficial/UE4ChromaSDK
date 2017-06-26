// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "IDetailCustomization.h"
#include "ChromaSDKEditorButton1D.h"
#include "ChromaSDKPluginTypes.h"
#include "Widgets/Layout/SGridPanel.h"

class FDetailWidgetRow;

class FChromaSDKEditorAnimation1DDetails : public IDetailCustomization
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
	FReply OnClickPasteButton();
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
	FReply OnClickAddFrame();
	FReply OnClickDeleteFrame();

private:

	// instance reference
	TSharedPtr<FChromaSDKEditorAnimation1DDetails> _mDetails;

	// object being inspected
	TArray<TWeakObjectPtr<UObject>> _mObjectsBeingCustomized;

	// the device enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKDevices;

	// the selected enum device
	EChromaSDKDevice1DEnum _mSelectedDevice;

	// device grid
	TSharedPtr<SGridPanel> _mGrid;

	// grid buttons hold element
	TArray<TSharedRef<IChromaSDKEditorButton1D>> _mColorButtons;

	// current color in the color picker
	FLinearColor _mColor;

	// for copy/paste for the grid colors
	TArray<FLinearColor> _mColors;

	// animation controls
	int _mCurrentFrame;
	TSharedPtr<STextBlock> _mTextCurrentFrame;
	TSharedPtr<STextBlock> _mTextNumberOfFrames;
	TSharedPtr<STextBlock> _mTextFrameDuration;
};

#endif
