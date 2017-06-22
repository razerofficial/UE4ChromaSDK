// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DDetails.h"
#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginButton2D.h"
#include "ChromaSDKPluginBPLibrary.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
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

	//populate list of device enums
	const UEnum* deviceEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKDevice2DEnum"), true);
	if (deviceEnumPtr)
	{
		for (int k = 0; (k + 1) < deviceEnumPtr->NumEnums(); ++k)
		{
			FString text = deviceEnumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->_mChromaSDKDevices.Add(MakeShared<FString>(text));
		}
	}
	//set default key
	instance->_mSelectedDevice = EChromaSDKDevice2DEnum::DE_Keyboard;

	//populate list of key enums
	const UEnum* keyEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKKeyboardKey"), true);
	if (keyEnumPtr)
	{
		for (int k = 0; (k+1) < keyEnumPtr->NumEnums(); ++k)
		{
			FString text = keyEnumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->_mChromaSDKKeyboardKeys.Add(MakeShared<FString>(text));
		}
	}
	//set default key
	instance->_mSelectedKey = EChromaSDKKeyboardKey::KK_ESC;

	//set the default frame
	instance->_mCurrentFrame = 0;

	return instance;
}

void FChromaSDKPluginAnimation2DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::CustomizeDetails"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		_mObjectsBeingCustomized.Empty();
	}

	//set the default frame
	_mCurrentFrame = 0;

	DetailBuilder.GetObjectsBeingCustomized(/*out*/ _mObjectsBeingCustomized);

	IDetailCategoryBuilder& MyCategory = DetailBuilder.EditCategory("Animation", LOCTEXT("Animation", "Animation"), ECategoryPriority::Important);

	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Device", "Device").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Device", "Device"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 4.0f)
			.FillColumn(1, 2.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.InitiallySelectedItem(_mChromaSDKDevices[0])
				.OptionsSource(&_mChromaSDKDevices)
				.OnGenerateWidget(this, &FChromaSDKPluginAnimation2DDetails::GenerateDropdownEnum)
				.OnSelectionChanged(this, &FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKDevices)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Select a device", "Select a device"))
				]
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set device", "Set device"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickSetDeviceButton)
			]
		];


	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Apply", "Apply").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Apply", "Apply"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 1.0f)
			.FillColumn(1, 1.0f)
			.FillColumn(2, 1.0f)
			.FillColumn(3, 1.0f)
			.FillColumn(4, 1.0f)
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
				.Text(LOCTEXT("Copy", "Copy"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickCopyButton)
			]
			+ SGridPanel::Slot(3, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Paste", "Paste"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickPasteButton)
			]
			+ SGridPanel::Slot(4, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Apply", "Apply"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickApplyButton)
			]
		];

	FDetailWidgetRow& widgetRow = MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Device preview", "Device preview").ToString()));
	widgetRow.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Device preview", "Device preview"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		];
	widgetRow.ValueContent().MinDesiredWidth(300);
	TSharedRef<SGridPanel> grid = SNew(SGridPanel);
	_mGrid = grid;
	widgetRow.ValueContent()
	[
		grid
	];

	RefreshDevice();

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
				.InitiallySelectedItem(_mChromaSDKKeyboardKeys[0])
				.OptionsSource(&_mChromaSDKKeyboardKeys)
				.OnGenerateWidget(this, &FChromaSDKPluginAnimation2DDetails::GenerateDropdownEnum)
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
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickSetKeyButton)
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

	TSharedRef<STextBlock> textCurrentFrame = SNew(STextBlock)
		.Text(LOCTEXT("0","0"));
	_mTextCurrentFrame = textCurrentFrame;

	TSharedRef<STextBlock> textNumberOfFrames = SNew(STextBlock)
		.Text(LOCTEXT("0","0"));
	_mTextNumberOfFrames = textNumberOfFrames;

	TSharedRef<STextBlock> textFrameDuration = SNew(STextBlock)
		.Text(LOCTEXT("1.0", "1.0"));
	_mTextFrameDuration = textFrameDuration;

	MyCategory.AddCustomRow(FText::FromString(LOCTEXT("Animation frames", "Animation frames").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Animation frames", "Animation frames"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 1.0f)
			.FillColumn(1, 1.0f)
			.FillColumn(2, 1.0f)
			.FillColumn(3, 1.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Frame: ","Frame: "))
			]
			+ SGridPanel::Slot(1, 0)
			[
				_mTextCurrentFrame.ToSharedRef()
			]
			+ SGridPanel::Slot(2, 0)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("of","of"))
			]
			+ SGridPanel::Slot(3, 0)
			[
				_mTextNumberOfFrames.ToSharedRef()
			]
			+ SGridPanel::Slot(0, 1)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Duration:","Duration:"))
			]
			+ SGridPanel::Slot(1, 1)
			[
				_mTextFrameDuration.ToSharedRef()
			]
			+ SGridPanel::Slot(2, 1)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("second(s)","second(s)"))
			]
			+ SGridPanel::Slot(0, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Previous", "Previous"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickPreviousFrame)
			]
			+ SGridPanel::Slot(1, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Next","Next"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickNextFrame)
			]
			+ SGridPanel::Slot(2, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Add","Add"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickAddFrame)
			]
			+ SGridPanel::Slot(3, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Delete","Delete"))
				.OnClicked(this, &FChromaSDKPluginAnimation2DDetails::OnClickDeleteFrame)
			]
		];

		RefreshFrames();
}

void FChromaSDKPluginAnimation2DDetails::RefreshFrames()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			const EChromaSDKDevice2DEnum& device = animation->Device;
			_mTextCurrentFrame->SetText(FText::AsNumber(_mCurrentFrame+1));
			_mTextNumberOfFrames->SetText(FText::AsNumber(animation->Frames.Num()));
			float duration = 0.0f;
			if (_mCurrentFrame < animation->Frames.Num() &&
				_mCurrentFrame < animation->Curve.EditorCurveData.Keys.Num())
			{
				if (_mCurrentFrame == 0)
				{
					duration = animation->Curve.EditorCurveData.Keys[_mCurrentFrame].Time;
				}
				else
				{
					duration =
						animation->Curve.EditorCurveData.Keys[_mCurrentFrame].Time -
						animation->Curve.EditorCurveData.Keys[_mCurrentFrame-1].Time;
				}
			}
			_mTextFrameDuration->SetText(FText::AsNumber(duration));
			return;
		}
	}

	_mCurrentFrame = 0;
	_mTextCurrentFrame->SetText(FText::AsNumber(0));
	_mTextNumberOfFrames->SetText(FText::AsNumber(0));
	_mTextFrameDuration->SetText(FText::AsNumber(0.0f));
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickPreviousFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			if (_mCurrentFrame > 0)
			{
				--_mCurrentFrame;
			}
			animation->RefreshCurve();
			RefreshFrames();
			RefreshDevice();
			return FReply::Handled();
		}
	}

	_mCurrentFrame = 0;
	return FReply::Handled();
}
FReply FChromaSDKPluginAnimation2DDetails::OnClickNextFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			if ((_mCurrentFrame+1) < animation->Frames.Num())
			{
				++_mCurrentFrame;
			}
			animation->RefreshCurve();
			RefreshFrames();
			RefreshDevice();
			return FReply::Handled();
		}
	}

	_mCurrentFrame = 0;
	return FReply::Handled();
}
FReply FChromaSDKPluginAnimation2DDetails::OnClickAddFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			FChromaSDKColorFrame2D frame = FChromaSDKColorFrame2D();
			frame.Colors = UChromaSDKPluginBPLibrary::CreateColors2D(animation->Device);
			if (_mCurrentFrame == animation->Frames.Num() ||
				(_mCurrentFrame+1) == animation->Frames.Num())
			{
				animation->Frames.Add(frame);
				_mCurrentFrame = animation->Frames.Num() - 1;
			}
			else
			{
				++_mCurrentFrame;
				animation->Frames.Insert(frame, _mCurrentFrame);

			}
			animation->RefreshCurve();
			RefreshFrames();
			RefreshDevice();
			return FReply::Handled();
		}
	}

	_mCurrentFrame = 0;
	return FReply::Handled();
}
FReply FChromaSDKPluginAnimation2DDetails::OnClickDeleteFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			if (animation->Frames.Num() == 1)
			{
				// reset frame
				animation->Frames[0].Colors = UChromaSDKPluginBPLibrary::CreateColors2D(animation->Device);
			}
			else if (animation->Frames.Num() > 0)
			{
				animation->Frames.RemoveAt(_mCurrentFrame);
				if (_mCurrentFrame == animation->Frames.Num())
				{
					_mCurrentFrame = animation->Frames.Num() - 1;
				}
			}
			animation->RefreshCurve();
			RefreshFrames();
			RefreshDevice();
			return FReply::Handled();
		}
	}

	_mCurrentFrame = 0;
	return FReply::Handled();
}

TSharedRef<SColorBlock> FChromaSDKPluginAnimation2DDetails::SetupColorButton(int row, int column, const FLinearColor& color)
{
	TSharedRef<IChromaSDKPluginButton2D> button =
		IChromaSDKPluginButton2D::MakeInstance();
	button->Row = row;
	button->Column = column;
	button->Details = _mDetails;

	TSharedPtr<SColorBlock> ptrColor = button->CreateColorBlock(color);
	TSharedRef<SColorBlock> newColor = ptrColor.ToSharedRef();

	// store the button event reference
	_mColorButtons.Add(button);

	return newColor;
}

void FChromaSDKPluginAnimation2DDetails::RefreshDevice()
{
	// Remove existing button events
	if (_mColorButtons.Num() > 0)
	{
		_mColorButtons.Empty();
	}

	// clear grid
	_mGrid->ClearChildren();

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame < animation->Frames.Num())
		{
			int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(animation->Device);
			int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(animation->Device);

			FChromaSDKColorFrame2D& frame = animation->Frames[_mCurrentFrame];
			TArray<FChromaSDKColors>& colors = frame.Colors;

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
								SetupColorButton(i, j, colors[i].Colors[j])
							]
						];

					(*_mGrid).AddSlot(j, i)
						.AttachWidget(overlay);
				}
			}
		}
	}
}

void FChromaSDKPluginAnimation2DDetails::OnClickColor(int row, int column)
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			if (frames.Num() > 0 &&
				row >= 0 &&
				row < frames[_mCurrentFrame].Colors.Num() &&
				column >= 0 &&
				column < frames[_mCurrentFrame].Colors[row].Colors.Num())
			{
				TArray<FChromaSDKColors>& colors = frames[_mCurrentFrame].Colors;
				colors[row].Colors[column] = _mColor;
			}
		}
	}

	RefreshDevice();
}

TSharedRef<SWidget> FChromaSDKPluginAnimation2DDetails::GenerateDropdownEnum(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	FString selectedItem = *Item;
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKDevices Selected=%s"),
		*selectedItem);

	// set the selected enum
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKDevice2DEnum"), true);
	if (enumPtr)
	{
		for (int k = 0; (k + 1) < enumPtr->NumEnums(); ++k)
		{
			FString text = enumPtr->GetDisplayNameTextByValue(k).ToString();
			if (text == selectedItem)
			{
				_mSelectedDevice = (EChromaSDKDevice2DEnum)k;
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnChangeChromaSDKDevices Item=%d"), k);
				break;
			}
		}
	}
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
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			if (frames.Num() > 0)
			{
				frames[_mCurrentFrame].Colors = UChromaSDKPluginBPLibrary::CreateColors2D(animation->Device);
			}
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickCopyButton()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			_mColors = frames[_mCurrentFrame].Colors;
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickPasteButton()
{
	// refresh the UI
	RefreshDevice();

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(device);
			int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(device);
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			if (_mColors.Num() == maxRow &&
				_mColors[_mCurrentFrame].Colors.Num() == maxColumn)
			{
				frames[_mCurrentFrame].Colors = _mColors;
			}
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickRandomButton()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			frames[_mCurrentFrame].Colors = UChromaSDKPluginBPLibrary::CreateRandomColors2D(animation->Device);
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickSetDeviceButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnClickSetDeviceButton"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			animation->Device != _mSelectedDevice)
		{
			animation->Reset(_mSelectedDevice);

			// reset clipboard
			_mColors = UChromaSDKPluginBPLibrary::CreateColors2D(animation->Device);

			// set default frame
			_mCurrentFrame = 0;

			// refresh the UI
			RefreshDevice();
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation2DDetails::OnClickSetKeyButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation2DDetails::OnClickSetKeyButton"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			TArray<FChromaSDKColors>& colors = frames[_mCurrentFrame].Colors;
			UChromaSDKPluginBPLibrary::SetKeyboardKeyColor(_mSelectedKey, _mColor, colors);
		}
	}

	// refresh the UI
	RefreshDevice();

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

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice2DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			TArray<FChromaSDKColors>& colors = frames[_mCurrentFrame].Colors;
			FChromaSDKEffectResult effect = UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom2D(device, colors);
			if (effect.Result == 0)
			{
				UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(effect.EffectId);
				UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(effect.EffectId);
			}
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
