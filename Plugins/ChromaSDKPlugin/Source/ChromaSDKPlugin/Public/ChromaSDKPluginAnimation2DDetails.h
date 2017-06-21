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

	void OnClickColor(int row, int column);

	TSharedRef<SWidget> GenerateChromaSDKKeyboardKeys(TSharedPtr<FString> InItem);
	void OnChangeChromaSDKKeyboardKeys(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);

	void OnColorCommitted(FLinearColor color);

	FReply OnClickPreviousFrame();
	FReply OnClickNextFrame();
	FReply OnClickAddFrame();
	FReply OnClickDeleteFrame();

	FReply OnClickSetButton();

	FReply OnClickCopyButton();

	FReply OnClickPasteButton();

	FReply OnClickClearButton();

	FReply OnClickRandomButton();

	FReply OnClickApplyButton();

private:

	int _mCurrentFrame;
	TSharedPtr<STextBlock> _mTextCurrentFrame;
	TSharedPtr<STextBlock> _mTextNumberOfFrames;
	TSharedPtr<STextBlock> _mTextFrameDuration;

	TSharedPtr<FChromaSDKPluginAnimation2DDetails> _mDetails;

	TArray<TWeakObjectPtr<UObject>> _mObjectsBeingCustomized;

	TSharedPtr<SGridPanel> _mGrid;

	TArray<TSharedRef<IChromaSDKPluginButton2D>> _mColorButtons;

	TArray<TSharedPtr<FString>> _mChromaSDKKeyboardKeys;

	EChromaSDKKeyboardKey _mSelectedKey;
	FLinearColor _mColor;
	TArray<FChromaSDKColors> _mColors;
};
