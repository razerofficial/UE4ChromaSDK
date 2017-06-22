// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "ChromaSDKPluginTypes.h"
#include "ChromaSDKPluginButton2D.h"
#include "Widgets/Layout/SGridPanel.h"

class FChromaSDKPluginAnimation2DDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	void RefreshFrames();

	void CreateKeyboard();
	void RefreshKeyboard();

	// enum dropdown handler
	TSharedRef<SWidget> GenerateDropdownEnum(TSharedPtr<FString> InItem);

	// device row
	void OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	FReply OnClickSetDeviceButton();

	// device preview row
	void OnClickColor(int row, int column);

	// apply row
	FReply OnClickClearButton();
	FReply OnClickRandomButton();
	FReply OnClickCopyButton();
	FReply OnClickPasteButton();
	FReply OnClickApplyButton();

	// select a key row
	void OnChangeChromaSDKKeyboardKeys(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	FReply OnClickSetKeyButton();

	// set the color row
	void OnColorCommitted(FLinearColor color);

	// animation frames row
	FReply OnClickPreviousFrame();
	FReply OnClickNextFrame();
	FReply OnClickAddFrame();
	FReply OnClickDeleteFrame();

private:

	// instance reference
	TSharedPtr<FChromaSDKPluginAnimation2DDetails> _mDetails;

	// object being inspected
	TArray<TWeakObjectPtr<UObject>> _mObjectsBeingCustomized;

	// the device enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKDevices;

	// the selected enum device
	EChromaSDKDevice2DEnum _mSelectedDevice;

	// device grid
	TSharedPtr<SGridPanel> _mGrid;

	// grid buttons hold row/column
	TArray<TSharedRef<IChromaSDKPluginButton2D>> _mColorButtons;

	// the enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKKeyboardKeys;

	// the selected enum key
	EChromaSDKKeyboardKey _mSelectedKey;
	
	// current color in the color picker
	FLinearColor _mColor;

	// for copy/paste for the grid colors
	TArray<FChromaSDKColors> _mColors;

	// animation controls
	int _mCurrentFrame;
	TSharedPtr<STextBlock> _mTextCurrentFrame;
	TSharedPtr<STextBlock> _mTextNumberOfFrames;
	TSharedPtr<STextBlock> _mTextFrameDuration;
};
