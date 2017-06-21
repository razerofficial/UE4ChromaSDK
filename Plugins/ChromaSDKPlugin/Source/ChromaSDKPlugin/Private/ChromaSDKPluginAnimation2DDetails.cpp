// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DDetails.h"
#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginButton2D.h"
#include "ChromaSDKPluginBPLibrary.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "SlateApplication.h"

#define LOCTEXT_NAMESPACE "ChromaAnimation2DDetails"

TSharedRef<IDetailCustomization> FChromaSDKPluginAnimation2DDetails::MakeInstance()
{
	TSharedRef<FChromaSDKPluginAnimation2DDetails> instance = MakeShareable(new FChromaSDKPluginAnimation2DDetails);
	instance->_mDetails = instance;

	//populate list of enums
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKKeyboardKey"), true);
	if (enumPtr)
	{
		for (int k = 0; (k+1) < enumPtr->NumEnums(); ++k)
		{
			FString text = enumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->ChromaSDKKeyboardKeys.Add(MakeShared<FString>(text));
		}
	}

	//set default key
	instance->_mSelectedKey = EChromaSDKKeyboardKey::KK_ESC;

	return instance;
}

void FChromaSDKPluginAnimation2DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::CustomizeDetails"));

	if (ObjectsBeingCustomized.Num() > 0)
	{
		ObjectsBeingCustomized.Empty();
	}
	DetailBuilder.GetObjectsBeingCustomized(/*out*/ ObjectsBeingCustomized);

	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory("CategoryName", LOCTEXT("Extra info", "Extra info"), ECategoryPriority::Important);

	FDetailWidgetRow& widgetRow = MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Keyboard preview", "Keyboard preview").ToString()));
	widgetRow.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Keyboard preview", "Keyboard preview"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		];
	widgetRow.ValueContent().MinDesiredWidth(300);
	TSharedRef<SGridPanel> grid = SNew(SGridPanel);
	_mGrid = grid;
	widgetRow.ValueContent()
	[
		grid
	];

	CreateKeyboard();
	RefreshKeyboard();

	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Select a key on the keyboard", "Select a key on the keyboard").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Select a key", "Select a key"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 4.0f)
			.FillColumn(1, 1.0f)
			+ SGridPanel::Slot(0, 0)
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
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set key", "Set key"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickSetButton)
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
		.ValueContent().MinDesiredWidth(30)
		[
			SNew(SGridPanel)
			.FillColumn(0, 1.0f)
			.FillColumn(1, 1.0f)
			.FillColumn(2, 1.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Clear", "Clear"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickClearButton)
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Random", "Random"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickRandomButton)
			]
			+ SGridPanel::Slot(2, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Apply", "Apply"))
			.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickApplyButton)
			]
		];
}

void FChromaSDKPluginAnimation2DDetails::CreateKeyboard()
{
	if (ObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)ObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			animation->Device == EChromaSDKDevice2DEnum::DE_Keyboard &&
			animation->Frames.Num() > 0)
		{
			int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(EChromaSDKDevice2DEnum::DE_Keyboard);
			int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(EChromaSDKDevice2DEnum::DE_Keyboard);
			for (int i = 0; i < maxRow; ++i)
			{
				for (int j = 0; j < maxColumn; ++j)
				{
					TSharedRef<SOverlay> overlay = SNew(SOverlay)
						+ SOverlay::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SNew(SBorder)
							.Padding(1.0f)
							.BorderBackgroundColor(FLinearColor::Black)
							[
								SNew(SColorBlock)
								.Color(FLinearColor::Black)
							]
						];

					(*_mGrid).AddSlot(j, i)
						.AttachWidget(overlay);
				}
			}
		}
	}
}

void FChromaSDKPluginAnimation2DDetails::RefreshKeyboard()
{
	// Remove existing button events
	if (ColorButtons.Num() > 0)
	{
		ColorButtons.Empty();
	}

	if (ObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)ObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			animation->Device == EChromaSDKDevice2DEnum::DE_Keyboard &&
			animation->Frames.Num() > 0)
		{
			int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(EChromaSDKDevice2DEnum::DE_Keyboard);
			int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(EChromaSDKDevice2DEnum::DE_Keyboard);

			FChromaSDKColorFrame2D& frame = animation->Frames[0];
			TArray<FChromaSDKColors>& colors = frame.Colors;

			FChildren* children1 = _mGrid->GetChildren();
			if (children1)
			{

				int index = 0;
				for (int i = 0; i < maxRow; ++i)
				{
					FChromaSDKColors& row = colors[i];
					for (int j = 0; j < maxColumn; ++j)
					{
						FLinearColor& color = row.Colors[j];
						color.A = 1.0f; //full alpha

						TSharedRef<SWidget> child = children1->GetChildAt(index);
						SOverlay* overlay = (SOverlay*)&child.Get();
						if (overlay)
						{
							FChildren* children2 = overlay->GetChildren();
							if (children2 &&
								children2->Num() > 0)
							{
								TSharedRef<SWidget> child2 = children2->GetChildAt(0);
								SOverlay::FOverlaySlot* slot = (SOverlay::FOverlaySlot*)&child2.Get();
								if (slot)
								{
									FChildren* children3 = overlay->GetChildren();
									if (children3 &&
										children3->Num() > 0)
									{
										TSharedRef<SWidget> child3 = children3->GetChildAt(0);
										SBorder* border = (SBorder*)&child3.Get();
										if (border)
										{
											FChildren* children4 = border->GetChildren();
											if (children4 &&
												children4->Num() > 0)
											{
												TSharedRef<SWidget> child4 = children4->GetChildAt(0);
												SColorBlock* block = (SColorBlock*)&child4.Get();
												if (block)
												{
													TSharedRef<IChromaSDKPluginButton2D> button =
														IChromaSDKPluginButton2D::MakeInstance();
													button->Row = i;
													button->Column = j;
													button->Details = _mDetails;

													// would rather update existing color
													TSharedPtr<SColorBlock> ptrColor = button->CreateColorBlock(color);
													TSharedRef<SColorBlock> newColor = ptrColor.ToSharedRef();

													// store the button event reference
													ColorButtons.Add(button);

													border->ClearContent();
													border->SetContent(newColor);
												}
											}
										}
									}
								}
							}
						}
						++index;
					}
				}
			}
		}
	}
}

void FChromaSDKPluginAnimation2DDetails::OnClickColor(int row, int column)
{
	if (ObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)ObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			if (frames.Num() > 0 &&
				row >= 0 &&
				row < frames[0].Colors.Num() &&
				column >= 0 &&
				column < frames[0].Colors[row].Colors.Num())
			{
				TArray<FChromaSDKColors>& colors = frames[0].Colors;
				colors[row].Colors[column] = _mColor;
			}
		}
	}

	RefreshKeyboard();
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
	color.A = 1.0f; //full alpha
	_mColor = color;
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickClearButton()
{
	if (ObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)ObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			if (frames.Num() > 0)
			{
				frames[0].Colors = UChromaSDKPluginBPLibrary::CreateColors2D(animation->Device);
			}
		}
	}

	// refresh the UI
	RefreshKeyboard();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickRandomButton()
{
	if (ObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)ObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			if (frames.Num() > 0)
			{
				frames[0].Colors = UChromaSDKPluginBPLibrary::CreateRandomColors2D(animation->Device);
			}
		}
	}

	// refresh the UI
	RefreshKeyboard();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickSetButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnClickSetButton"));

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
			}
		}
	}

	// refresh the UI
	RefreshKeyboard();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickApplyButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnClickApplyButton"));

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
