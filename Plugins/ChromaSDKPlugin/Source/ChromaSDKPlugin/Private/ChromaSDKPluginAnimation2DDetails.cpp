// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DDetails.h"
#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginBPLibrary.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "SlateApplication.h"

#define LOCTEXT_NAMESPACE "ChromaAnimation2DDetails"

TSharedRef<IDetailCustomization> FChromaSDKPluginAnimation2DDetails::MakeInstance()
{
	TSharedRef<FChromaSDKPluginAnimation2DDetails> instance = MakeShareable(new FChromaSDKPluginAnimation2DDetails);

	FChromaSDKPluginAnimation2DDetails& details = instance.Get();

	//populate list of enums
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKKeyboardKey"), true);
	if (enumPtr)
	{
		for (int k = 0; (k+1) < enumPtr->NumEnums(); ++k)
		{
			FString text = enumPtr->GetDisplayNameTextByValue(k).ToString();
			details.ChromaSDKKeyboardKeys.Add(MakeShared<FString>(text));
		}
	}

	//set default key
	details._mSelectedKey = EChromaSDKKeyboardKey::KK_ESC;

	return instance;
}

void FChromaSDKPluginAnimation2DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::CustomizeDetails"));

	ObjectsBeingCustomized.Empty();
	DetailBuilder.GetObjectsBeingCustomized(/*out*/ ObjectsBeingCustomized);

	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory("CategoryName", LOCTEXT("Extra info", "Extra info"), ECategoryPriority::Important);

	//You can get properties using the detailbuilder
	//MyProperty= DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(MyClass, MyClassPropertyName));

	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Keyboard preview", "Keyboard preview").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Keyboard preview", "Keyboard preview"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 2.f)
			.FillColumn(1, 1.f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBorder)
					.Padding(1.0f)
					.BorderBackgroundColor(FLinearColor::Black)
					[
						SNew(SColorBlock)
					]
				]
			]
			+ SGridPanel::Slot(0, 1)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBorder)
					.Padding(1.0f)
					.BorderBackgroundColor(FLinearColor::Black)
					[
						SNew(SColorBlock)
					]
				]
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBorder)
					.Padding(1.0f)
					.BorderBackgroundColor(FLinearColor::Black)
					[
						SNew(SColorBlock)
					]
				]
			]
			+ SGridPanel::Slot(1, 1)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBorder)
					.Padding(1.0f)
					.BorderBackgroundColor(FLinearColor::Black)
					[
						SNew(SColorBlock)
					]
				]
			]
		];

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
			.OnGenerateWidget(this, &FChromaSDKPluginAnimation2DDetails::GenerateChromaSDKKeyboardKeys)
			.OnSelectionChanged(this, &FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKKeyboardKeys)
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
			.OnColorCommitted(this, &FChromaSDKPluginAnimation2DDetails::OnColorCommitted)
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
			.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickSetButton)
		];
}

TSharedRef<SWidget> FChromaSDKPluginAnimation2DDetails::GenerateChromaSDKKeyboardKeys(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKKeyboardKeys(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	FString selectedItem = *Item;
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKKeyboardKeys Selected=%s"),
		*selectedItem);

	// set the selected enum
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKKeyboardKey"), true);
	if (enumPtr)
	{
		for (int k = 0; (k + 1) < enumPtr->NumEnums(); ++k)
		{
			FString text = enumPtr->GetDisplayNameTextByValue(k).ToString();
			if (text == selectedItem)
			{
				_mSelectedKey = (EChromaSDKKeyboardKey)k;
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKKeyboardKeys Item=%d"), k);
				break;
			}
		}
	}
}

void FChromaSDKPluginAnimation2DDetails::OnColorCommitted(FLinearColor color)
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnColorCommitted"));
	_mColor = color;
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickSetButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnClickSetButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (ObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)ObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			if (frames.Num() > 0)
			{
				TArray<FChromaSDKColors>& colors = frames[0].Colors;
				UChromaSDKPluginBPLibrary::SetKeyboardKeyColor(_mSelectedKey, _mColor, colors);
				FChromaSDKEffectResult effect = UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom2D(device, colors);
				if (effect.Result == 0)
				{
					UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(effect.EffectId);
					UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(effect.EffectId);
				}
			}
		}
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
