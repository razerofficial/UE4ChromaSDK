// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DetailCustomizations.h"
#include "IPropertyTypeCustomization.h"

class FChromaSDKPluginAnimation1DCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	/** Chroma animation 1D struct handle */
	TSharedPtr<IPropertyHandle> StructPropertyHandle;

	TSharedPtr<IPropertyHandle> DeviceHandle;
	TSharedPtr<IPropertyHandle> FramesHandle;
	TSharedPtr<IPropertyHandle> CurveHandle;
};
