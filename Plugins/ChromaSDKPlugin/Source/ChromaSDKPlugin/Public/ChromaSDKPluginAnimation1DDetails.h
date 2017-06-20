// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"

class FChromaSDKPluginAnimation1DDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	FReply OnClickColor(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, bool bCheckAlpha);

	FReply OnClickSetButton();
};
