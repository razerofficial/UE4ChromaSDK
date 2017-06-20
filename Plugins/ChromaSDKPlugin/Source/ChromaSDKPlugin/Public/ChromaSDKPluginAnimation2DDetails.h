// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "ChromaSDKPluginTypes.h"

class FChromaSDKPluginAnimation2DDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	TSharedRef<SWidget> GenerateChromaSDKKeyboardKeys(TSharedPtr<FString> InItem);
	void OnChangeChromaSDKKeyboardKeys(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);

	void OnColorCommitted(FLinearColor color);

	FReply OnClickSetButton();

	TArray<TSharedPtr<FString>> ChromaSDKKeyboardKeys;

private:
	
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;

	EChromaSDKKeyboardKey _mSelectedKey;
	FLinearColor _mColor;
};
