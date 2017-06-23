// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DDetails.h"
#include "ChromaSDKPluginAnimation1DObject.h"
#include "ChromaSDKPluginButton1D.h"
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

#define LOCTEXT_NAMESPACE "ChromaAnimation1DDetails"

TSharedRef<IDetailCustomization> FChromaSDKPluginAnimation1DDetails::MakeInstance()
{
	TSharedRef<FChromaSDKPluginAnimation1DDetails> instance = MakeShareable(new FChromaSDKPluginAnimation1DDetails);
	instance->_mDetails = instance;

	//populate list of device enums
	const UEnum* deviceEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKDevice1DEnum"), true);
	if (deviceEnumPtr)
	{
		for (int k = 0; (k + 1) < deviceEnumPtr->NumEnums(); ++k)
		{
			FString text = deviceEnumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->_mChromaSDKDevices.Add(MakeShared<FString>(text));
		}
	}
	//set default key
	instance->_mSelectedDevice = EChromaSDKDevice1DEnum::DE_ChromaLink;

	//set the default frame
	instance->_mCurrentFrame = 0;

	return instance;
}

void FChromaSDKPluginAnimation1DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::CustomizeDetails"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		_mObjectsBeingCustomized.Empty();
	}

	//set the default frame
	_mCurrentFrame = 0;

	DetailBuilder.GetObjectsBeingCustomized(/*out*/ _mObjectsBeingCustomized);

	IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Animation", LOCTEXT("Animation", "Animation"), ECategoryPriority::Important);

	category.AddCustomRow(FText::FromString(LOCTEXT("Device", "Device").ToString()))
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
				.OnGenerateWidget(this, &FChromaSDKPluginAnimation1DDetails::GenerateDropdownEnum)
				.OnSelectionChanged(this, &FChromaSDKPluginAnimation1DDetails::OnChangeChromaSDKDevices)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Select a device", "Select a device"))
				]
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set device", "Set device"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickSetDeviceButton)
			]
		];


	category.AddCustomRow(FText::FromString(LOCTEXT("Apply", "Apply").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Apply", "Apply"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 1.5f)
			.FillColumn(1, 1.0f)
			.FillColumn(2, 2.0f)
			.FillColumn(3, 1.5f)
			.FillColumn(4, 1.5f)
			.FillColumn(5, 1.5f)
			.FillColumn(6, 1.5f)
			.FillColumn(7, 1.5f)
			.FillColumn(8, 1.5f)
			.FillColumn(9, 2.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Clear", "Clear"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickClearButton)
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Fill", "Fill"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickFillButton)
			]
			+ SGridPanel::Slot(2, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Random", "Random"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickRandomButton)
			]
			+ SGridPanel::Slot(3, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Copy", "Copy"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickCopyButton)
			]
			+ SGridPanel::Slot(4, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Paste", "Paste"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickPasteButton)
			]
			+ SGridPanel::Slot(5, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Apply", "Apply"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickApplyButton)
			]
			+ SGridPanel::Slot(6, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Play", "Play"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickPlayButton)
			]
			+ SGridPanel::Slot(7, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Stop", "Stop"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickStopButton)
			]
			+ SGridPanel::Slot(8, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Load", "Load"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickLoadButton)
			]
			+ SGridPanel::Slot(9, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Unload", "Unload"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickUnloadButton)
			]
		];

	FDetailWidgetRow& widgetRow = category.AddCustomRow(FText::FromString(LOCTEXT("Device preview", "Device preview").ToString()));
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

	category.AddCustomRow(FText::FromString(LOCTEXT("Set the color", "Set the color").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Set the color", "Set the color"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SColorPicker)
			.OnColorCommitted(this, &FChromaSDKPluginAnimation1DDetails::OnColorCommitted)
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

	category.AddCustomRow(FText::FromString(LOCTEXT("Animation frames", "Animation frames").ToString()))
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
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickPreviousFrame)
			]
			+ SGridPanel::Slot(1, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Next","Next"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickNextFrame)
			]
			+ SGridPanel::Slot(2, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Add","Add"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickAddFrame)
			]
			+ SGridPanel::Slot(3, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Delete","Delete"))
				.OnClicked(this, &FChromaSDKPluginAnimation1DDetails::OnClickDeleteFrame)
			]
		];

		RefreshFrames();
}

void FChromaSDKPluginAnimation1DDetails::RefreshFrames()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			const EChromaSDKDevice1DEnum& device = animation->Device;
			_mTextCurrentFrame->SetText(FText::AsNumber(_mCurrentFrame+1));
			_mTextNumberOfFrames->SetText(FText::AsNumber(animation->Frames.Num()));
			float duration = 0.0f;
			if (_mCurrentFrame >= 0 &&
				_mCurrentFrame < animation->Frames.Num() &&
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

FReply FChromaSDKPluginAnimation1DDetails::OnClickPreviousFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
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
FReply FChromaSDKPluginAnimation1DDetails::OnClickNextFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
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
FReply FChromaSDKPluginAnimation1DDetails::OnClickAddFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsLoaded())
			{
				animation->Unload();
			}
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
			frame.Colors = UChromaSDKPluginBPLibrary::CreateColors1D(animation->Device);
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
FReply FChromaSDKPluginAnimation1DDetails::OnClickDeleteFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsLoaded())
			{
				animation->Unload();
			}
			if (_mCurrentFrame < 0 ||
				_mCurrentFrame >= animation->Frames.Num())
			{
				_mCurrentFrame = 0;
			}
			if (animation->Frames.Num() == 1)
			{
				// reset frame
				animation->Frames[0].Colors = UChromaSDKPluginBPLibrary::CreateColors1D(animation->Device);
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

TSharedRef<SColorBlock> FChromaSDKPluginAnimation1DDetails::SetupColorButton(int element, const FLinearColor& color)
{
	TSharedRef<IChromaSDKPluginButton1D> button =
		IChromaSDKPluginButton1D::MakeInstance();
	button->Element = element;
	button->Details = _mDetails;

	TSharedPtr<SColorBlock> ptrColor = button->CreateColorBlock(color);
	TSharedRef<SColorBlock> newColor = ptrColor.ToSharedRef();

	// store the button event reference
	_mColorButtons.Add(button);

	return newColor;
}

void FChromaSDKPluginAnimation1DDetails::RefreshDevice()
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
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			int maxLeds = UChromaSDKPluginBPLibrary::GetMaxLeds(animation->Device);

			FChromaSDKColorFrame1D& frame = animation->Frames[_mCurrentFrame];
			TArray<FLinearColor>& colors = frame.Colors;
			if (colors.Num() == maxLeds)
			{
				for (int i = 0; i < maxLeds; ++i)
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
								SetupColorButton(i, colors[i])
							]
						];

					(*_mGrid).AddSlot(i, 0)
						.AttachWidget(overlay);
				}
			}
		}
	}
}

void FChromaSDKPluginAnimation1DDetails::OnClickColor(int element)
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice1DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
			if (frames.Num() > 0 &&
				element >= 0 &&
				element < frames[_mCurrentFrame].Colors.Num())
			{
				TArray<FLinearColor>& colors = frames[_mCurrentFrame].Colors;
				colors[element] = _mColor;
			}
		}
	}

	RefreshDevice();
}

TSharedRef<SWidget> FChromaSDKPluginAnimation1DDetails::GenerateDropdownEnum(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void FChromaSDKPluginAnimation1DDetails::OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	FString selectedItem = *Item;
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnChangeChromaSDKDevices Selected=%s"),
		*selectedItem);

	// set the selected enum
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKDevice1DEnum"), true);
	if (enumPtr)
	{
		for (int k = 0; (k + 1) < enumPtr->NumEnums(); ++k)
		{
			FString text = enumPtr->GetDisplayNameTextByValue(k).ToString();
			if (text == selectedItem)
			{
				_mSelectedDevice = (EChromaSDKDevice1DEnum)k;
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnChangeChromaSDKDevices Item=%d"), k);
				break;
			}
		}
	}
}

void FChromaSDKPluginAnimation1DDetails::OnColorCommitted(FLinearColor color)
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnColorCommitted"));
	color.A = 1.0f; //full alpha
	_mColor = color;
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickClearButton()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice1DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
			if (frames.Num() > 0)
			{
				frames[_mCurrentFrame].Colors = UChromaSDKPluginBPLibrary::CreateColors1D(animation->Device);
			}
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickFillButton()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice1DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
			if (frames.Num() > 0)
			{
				FChromaSDKColorFrame1D& frame = frames[_mCurrentFrame];
				TArray<FLinearColor>& colors = frame.Colors;
				int maxLeds = UChromaSDKPluginBPLibrary::GetMaxLeds(device);
				for (int i = 0; i < maxLeds; ++i)
				{
					colors[i] = _mColor;
				}
			}
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickCopyButton()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice1DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
			FChromaSDKColorFrame1D& frame = frames[_mCurrentFrame];
			_mColors = frame.Colors;
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickPasteButton()
{
	// refresh the UI
	RefreshDevice();

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice1DEnum& device = animation->Device;
			int maxLed = UChromaSDKPluginBPLibrary::GetMaxLeds(device);
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
			FChromaSDKColorFrame1D& frame = frames[_mCurrentFrame];
			if (_mColors.Num() == maxLed)
			{
				frame.Colors = _mColors;
			}
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickRandomButton()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice1DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
			frames[_mCurrentFrame].Colors = UChromaSDKPluginBPLibrary::CreateRandomColors1D(animation->Device);
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickSetDeviceButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickSetDeviceButton"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			animation->Device != _mSelectedDevice)
		{
			animation->Reset(_mSelectedDevice);

			// reset clipboard
			_mColors = UChromaSDKPluginBPLibrary::CreateColors1D(animation->Device);

			// set default frame
			_mCurrentFrame = 0;

			// refresh the UI
			RefreshDevice();
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickApplyButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickApplyButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			const EChromaSDKDevice1DEnum& device = animation->Device;
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
			FChromaSDKColorFrame1D& frame = frames[_mCurrentFrame];
			TArray<FLinearColor>& colors = frame.Colors;
			FChromaSDKEffectResult effect = UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom1D(device, colors);
			if (effect.Result == 0)
			{
				UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(effect.EffectId);
				UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(effect.EffectId);
			}
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickPlayButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickPlayButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsPlaying())
			{
				UE_LOG(LogTemp, Error, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickPlayButton Animation is already playing!"));
			}
			else
			{
				if (!animation->IsLoaded())
				{
					animation->Load();
				}
				animation->Play();
			}
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickStopButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickStopButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsPlaying())
			{
				animation->Stop();
			}
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickLoadButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickLoadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (!animation->IsLoaded())
			{
				animation->Load();
			}
		}
	}

	return FReply::Handled();
}

FReply FChromaSDKPluginAnimation1DDetails::OnClickUnloadButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKPluginAnimation1DDetails::OnClickUnloadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsLoaded())
			{
				animation->Unload();
			}
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
