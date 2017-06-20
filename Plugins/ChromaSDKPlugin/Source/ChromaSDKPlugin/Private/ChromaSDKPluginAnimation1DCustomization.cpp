// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DCustomization.h"
#include "PropertyHandle.h"
#include "SlateFontInfoCustomization.h"

#define LOCTEXT_NAMESPACE "ChromaAnimation1DDetails"

TSharedRef<IPropertyTypeCustomization> FChromaSDKPluginAnimation1DCustomization::MakeInstance()
{
	return MakeShareable(new FChromaSDKPluginAnimation1DCustomization);
}

void FChromaSDKPluginAnimation1DCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InStructPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InStructCustomizationUtils)
{
	StructPropertyHandle = InStructPropertyHandle;
	IPropertyHandle* handle = StructPropertyHandle.Get();
	if (handle)
	{
		UProperty* property = handle->GetProperty();
		if (property)
		{
			UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DCustomization PropertyName: %s"), *property->GetName());
		}
	}
}

void FChromaSDKPluginAnimation1DCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	//Create further customization here
}

#undef LOCTEXT_NAMESPACE
