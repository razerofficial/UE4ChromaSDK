// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DDetails.h"
#include "ChromaSDKPluginTypes.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "SlateApplication.h"

#define LOCTEXT_NAMESPACE "ChromaAnimation1DDetails"

TSharedRef<IDetailCustomization> FChromaSDKPluginAnimation1DDetails::MakeInstance()
{
	TSharedRef<FChromaSDKPluginAnimation1DDetails> instance = MakeShareable(new FChromaSDKPluginAnimation1DDetails);

	FChromaSDKPluginAnimation1DDetails& details = instance.Get();

	//populate list of enums
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKKeyboardKey"), true);
	if (enumPtr)
	{
		for (int64 k = (int64)EChromaSDKKeyboardKey::KK_ESC; k <= (int64)EChromaSDKKeyboardKey::KK_INVALID; ++k)
		{
			FString text = enumPtr->GetNameByValue(k).ToString();
			details.ChromaSDKKeyboardKeys.Add(MakeShared<FString>(text));
		}
	}

	return instance;
}

void FChromaSDKPluginAnimation1DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::CustomizeDetails"));

	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory("CategoryName", LOCTEXT("Extra info", "Extra info"), ECategoryPriority::Important);

	//You can get properties using the detailbuilder
	//MyProperty= DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(MyClass, MyClassPropertyName));

	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Select a key on the keyboard", "Select a key on the keyboard").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Select a key", "Select a key"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SComboBox<TSharedPtr<FString>>)
			.InitiallySelectedItem(ChromaSDKKeyboardKeys[0])
			.OptionsSource(&ChromaSDKKeyboardKeys)
			.OnGenerateWidget(this, &FChromaSDKPluginAnimation1DDetails::GenerateChromaSDKKeyboardKeys)
			//.OnSelectionChanged(this, &FChromaSDKPluginAnimation1DDetails::OnChangeChromaSDKKeyboardKeys)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Select a key", "Select a key"))
			]
		];

	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Select a key color", "Select a key color").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Set the color", "Set the color"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SColorPicker)
			//.OnMouseButtonDown(this, &FChromaSDKPluginAnimation1DDetails::OnClickColor, false)
		];

	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Apply color on the keyboard", "Apply color on the keyboard").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Apply", "Apply"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(50)
		[
			SNew(SButton)
			.Text(LOCTEXT("Button Chroma Set", "Set"))
			.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickSetButton)
		];
}

TSharedRef<SWidget> FChromaSDKPluginAnimation1DDetails::GenerateChromaSDKKeyboardKeys(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickColor(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, bool bCheckAlpha)
{
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickColor"));

	FColorPickerArgs PickerArgs;
	{
		/*
		PickerArgs.bUseAlpha = !bIgnoreAlpha;
		PickerArgs.bOnlyRefreshOnMouseUp = false;
		PickerArgs.bOnlyRefreshOnOk = bRefreshOnlyOnOk;
		PickerArgs.sRGBOverride = sRGBOverride;
		PickerArgs.DisplayGamma = TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(GEngine, &UEngine::GetDisplayGamma));
		PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &FColorStructCustomization::OnSetColorFromColorPicker);
		PickerArgs.OnColorPickerCancelled = FOnColorPickerCancelled::CreateSP(this, &FColorStructCustomization::OnColorPickerCancelled);
		PickerArgs.OnInteractivePickBegin = FSimpleDelegate::CreateSP(this, &FColorStructCustomization::OnColorPickerInteractiveBegin);
		PickerArgs.OnInteractivePickEnd = FSimpleDelegate::CreateSP(this, &FColorStructCustomization::OnColorPickerInteractiveEnd);
		PickerArgs.InitialColorOverride = InitialColor;
		PickerArgs.ParentWidget = ColorPickerParentWidget;

		FWidgetPath ParentWidgetPath;
		if (FSlateApplication::Get().FindPathToWidget(ColorPickerParentWidget.ToSharedRef(), ParentWidgetPath))
		{
			PickerArgs.bOpenAsMenu = FSlateApplication::Get().FindMenuInWidgetPath(ParentWidgetPath).IsValid();
		}
		*/
	}

	OpenColorPicker(PickerArgs);

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickSetButton()
{
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickSetButton"));
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
