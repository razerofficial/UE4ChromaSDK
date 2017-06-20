// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DDetails.h"
#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "ChromaAnimation1DDetails"

TSharedRef<IDetailCustomization> FChromaSDKPluginAnimation1DDetails::MakeInstance()
{
	return MakeShareable(new FChromaSDKPluginAnimation1DDetails);
}

void FChromaSDKPluginAnimation1DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::CustomizeDetails"));

	/*

	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory("CategoryName", TEXT("Extra info"), ECategoryPriority::Important);

	//You can get properties using the detailbuilder
	//MyProperty= DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(MyClass, MyClassPropertyName));

	MyCategory.AddCustomRow(LOCTEXT("Extra info", "Row header name").ToString())
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Extra info", "Custom row header name"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
		]
	.ValueContent().MinDesiredWidth(500)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Extra info", "Custom row content"))
		.Font(IDetailLayoutBuilder::GetDetailFont())
		];
	*/
}

#undef LOCTEXT_NAMESPACE
