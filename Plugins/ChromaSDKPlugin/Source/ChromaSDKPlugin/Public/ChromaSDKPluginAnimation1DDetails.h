// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "ChromaSDKPluginTypes.h"

class FChromaSDKPluginAnimation1DDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	void OnColorCommitted(FLinearColor color);

	FReply OnClickSetButton();

private:

	TArray<TWeakObjectPtr<UObject>> _mObjectsBeingCustomized;

	FLinearColor _mColor;
};
