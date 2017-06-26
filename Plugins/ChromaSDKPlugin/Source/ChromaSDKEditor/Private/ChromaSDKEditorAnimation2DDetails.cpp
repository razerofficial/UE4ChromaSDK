// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#if WITH_EDITOR

#include "ChromaSDKEditorAnimation2DDetails.h"
#include "ChromaSDKEditorButton2D.h"
#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginBPLibrary.h"
#include "DesktopPlatformModule.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EditorDirectories.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SGridPanel.h"
#include "SlateApplication.h"

#if WITH_EDITOR
#if PLATFORM_WINDOWS
#include "objbase.h"
#include "Windows/WindowsHWrapper.h"
THIRD_PARTY_INCLUDES_START
#include <strsafe.h>
#include <wincodec.h>
THIRD_PARTY_INCLUDES_END
#pragma comment( lib, "windowscodecs.lib" )
#endif
#endif

#define LOCTEXT_NAMESPACE "ChromaAnimation2DDetails"

TSharedRef<IDetailCustomization> FChromaSDKEditorAnimation2DDetails::MakeInstance()
{
	TSharedRef<FChromaSDKEditorAnimation2DDetails> instance = MakeShareable(new FChromaSDKEditorAnimation2DDetails);
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

	//populate list of led enums
	const UEnum* ledEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKMouseLed"), true);
	if (ledEnumPtr)
	{
		for (int k = 0; (k + 1) < ledEnumPtr->NumEnums(); ++k)
		{
			FString text = ledEnumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->_mChromaSDKMouseLeds.Add(MakeShared<FString>(text));
		}
	}
	//set default led
	instance->_mSelectedLed = EChromaSDKMouseLed::ML_LOGO;

	//set the default frame
	instance->_mCurrentFrame = 0;

	return instance;
}

void FChromaSDKEditorAnimation2DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::CustomizeDetails"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		_mObjectsBeingCustomized.Empty();
	}

	//set the default frame
	_mCurrentFrame = 0;

	DetailBuilder.GetObjectsBeingCustomized(/*out*/ _mObjectsBeingCustomized);

	IDetailCategoryBuilder& import = DetailBuilder.EditCategory("Import", LOCTEXT("Import", "Import"), ECategoryPriority::Important);

	import.AddCustomRow(FText::FromString(LOCTEXT("Import", "Import").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Import", "Import"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 3.0f)
			.FillColumn(1, 3.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Import image", "Import image"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickImportTextureImageButton)
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Import texture", "Import animation"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickImportTextureAnimationButton)
			]
		];

		import.AddCustomRow(FText::FromString(LOCTEXT("Reset animation length", "Reset animation length").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Reset animation length", "Reset animation length"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SButton)
			.Text(LOCTEXT("Override", "Override"))
			.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickOverrideButton)
		];

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
				.OnGenerateWidget(this, &FChromaSDKEditorAnimation2DDetails::GenerateDropdownEnum)
				.OnSelectionChanged(this, &FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKDevices)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Select a device", "Select a device"))
				]
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set device", "Set device"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickSetDeviceButton)
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
			.FillColumn(5, 2.0f)
			.FillColumn(6, 1.5f)
			.FillColumn(7, 1.5f)
			.FillColumn(8, 1.5f)
			.FillColumn(9, 2.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Clear", "Clear"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickClearButton)
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Fill", "Fill"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickFillButton)
			]
			+ SGridPanel::Slot(2, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Random", "Random"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickRandomButton)
			]
			+ SGridPanel::Slot(3, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Copy", "Copy"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickCopyButton)
			]
			+ SGridPanel::Slot(4, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Paste", "Paste"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickPasteButton)
			]
			+ SGridPanel::Slot(5, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Preview", "Preview"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickPreviewButton)
			]
			+ SGridPanel::Slot(6, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Play", "Play"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickPlayButton)
			]
			+ SGridPanel::Slot(7, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Stop", "Stop"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickStopButton)
			]
			+ SGridPanel::Slot(8, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Load", "Load"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickLoadButton)
			]
			+ SGridPanel::Slot(9, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Unload", "Unload"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickUnloadButton)
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

	category.AddCustomRow(FText::FromString(LOCTEXT("Select a key", "Select a key").ToString()))
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
				.OnGenerateWidget(this, &FChromaSDKEditorAnimation2DDetails::GenerateDropdownEnum)
				.OnSelectionChanged(this, &FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKKeyboardKeys)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Select a key", "Select a key"))
				]
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set key", "Set key"))
				.IsEnabled(this, &FChromaSDKEditorAnimation2DDetails::IsEnabledKeyboardKey)
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickSetKeyButton)
			]
		];

	category.AddCustomRow(FText::FromString(LOCTEXT("Select an LED", "Select an LED").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Select an LED", "Select an LED"))
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
				.InitiallySelectedItem(_mChromaSDKMouseLeds[0])
				.OptionsSource(&_mChromaSDKMouseLeds)
				.OnGenerateWidget(this, &FChromaSDKEditorAnimation2DDetails::GenerateDropdownEnum)
				.OnSelectionChanged(this, &FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKMouseLeds)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Select an LED", "Select an LED"))
				]
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set LED", "Set LED"))
				.IsEnabled(this, &FChromaSDKEditorAnimation2DDetails::IsEnabledMouseLed)
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickSetLedButton)
			]
		];

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
			.OnColorCommitted(this, &FChromaSDKEditorAnimation2DDetails::OnColorCommitted)
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

	/*
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation)
		{
			TWeakObjectPtr<UChromaSDKPluginAnimation2DObject> UserDefinedStruct = CastChecked<UChromaSDKPluginAnimation2DObject>(animation);
			for (TFieldIterator<UProperty> propertyIter(animation->GetClass()); propertyIter; ++propertyIter)
			{
				UProperty* property = (*propertyIter);
				const FName& propertyName = property->GetFName();
				FString stringName = propertyName.ToString();
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails propertyName=%s"), *stringName);
				if (stringName == "Curve")
				{
					//TSharedPtr<FStructOnScope> structData = MakeShareable(new FStructOnScope(UserDefinedStruct.Get()));
					//TSharedPtr<FStructOnScope> structData = MakeShareable(new FStructOnScope(animation->Curve, false));
					//category.AddExternalProperty(structData, (*propertyIter)->GetFName());
				}
			}

			TArray<UObject*> objects = TArray<UObject*>();
			objects.Add(animation);
			category.AddExternalProperty(objects, "Curve");
		}
	}

	category.AddCustomRow(FText::FromString(LOCTEXT("Reset animation length", "Reset animation length").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Reset animation length", "Reset animation length"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SNew(SGridPanel)
			.FillColumn(0, 5.0f)
			.FillColumn(1, 2.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SProperty, DetailBuilder.GetProperty("Curve"))
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set", "Set"))
			]
		];
	*/

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
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickPreviousFrame)
			]
			+ SGridPanel::Slot(1, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Next","Next"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickNextFrame)
			]
			+ SGridPanel::Slot(2, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Add","Add"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickAddFrame)
			]
			+ SGridPanel::Slot(3, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Delete","Delete"))
				.OnClicked(this, &FChromaSDKEditorAnimation2DDetails::OnClickDeleteFrame)
			]
		];

		RefreshFrames();
}

bool FChromaSDKEditorAnimation2DDetails::IsEnabledKeyboardKey() const
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			return animation->Device == EChromaSDKDevice2DEnum::DE_Keyboard;
		}
	}
	return false;
}

bool FChromaSDKEditorAnimation2DDetails::IsEnabledMouseLed() const
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			return animation->Device == EChromaSDKDevice2DEnum::DE_Mouse;
		}
	}
	return false;
}

void FChromaSDKEditorAnimation2DDetails::RefreshFrames()
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickPreviousFrame()
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickNextFrame()
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickAddFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickDeleteFrame()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
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

void FChromaSDKEditorAnimation2DDetails::ReadImage(const FString& path, bool isAnimation)
{
#if PLATFORM_WINDOWS

	HRESULT hr = CoInitialize(NULL);

	// The factory pointer
	IWICImagingFactory *Factory = NULL;

	// Create the COM imaging factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&Factory)
	);

	// Decode the source image to IWICBitmapSource
	IWICBitmapDecoder *Decoder = NULL;

	// Create a decoder
	hr = Factory->CreateDecoderFromFilename(
		(LPCTSTR)*path, // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&Decoder                        // Pointer to the decoder
	);

	IWICBitmapFrameDecode *Frame = NULL;

	UINT frameCount = 1;

	for (UINT frameIndex = 0; frameIndex < frameCount; ++frameIndex)
	{
		if (isAnimation)
		{
			if (SUCCEEDED(hr))
			{
				hr = (Decoder->GetFrameCount(&frameCount));
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails frameCount=%d"),
					(int)frameCount);
			}
		}

		// Retrieve the first frame of the image from the decoder
		if (SUCCEEDED(hr))
		{
			hr = Decoder->GetFrame(frameIndex, &Frame);
		}

		// Retrieve IWICBitmapSource from the frame
		IWICBitmapSource *OriginalBitmapSource = NULL;
		if (SUCCEEDED(hr))
		{
			hr = Frame->QueryInterface(IID_IWICBitmapSource, reinterpret_cast<void **>(&OriginalBitmapSource));
		}

		IWICBitmapSource *ToRenderBitmapSource = NULL;

		// convert the pixel format
		if (SUCCEEDED(hr))
		{
			IWICFormatConverter *Converter = NULL;

			hr = Factory->CreateFormatConverter(&Converter);

			// Format convert to 32bppBGR
			if (SUCCEEDED(hr))
			{
				hr = Converter->Initialize(
					Frame,                          // Input bitmap to convert
					GUID_WICPixelFormat32bppBGR,     // Destination pixel format
					WICBitmapDitherTypeNone,         // Specified dither patterm
					NULL,                            // Specify a particular palette 
					0.f,                             // Alpha threshold
					WICBitmapPaletteTypeCustom       // Palette translation type
				);

				// Store the converted bitmap if successful
				if (SUCCEEDED(hr))
				{
					hr = Converter->QueryInterface(IID_PPV_ARGS(&ToRenderBitmapSource));
				}
			}

			Converter->Release();
		}

		// Create a DIB from the converted IWICBitmapSource
		HBITMAP hDIBBitmap = 0;
		if (SUCCEEDED(hr))
		{
			// Get image attributes and check for valid image
			UINT width = 0;
			UINT height = 0;

			void *ImageBits = NULL;

			// Check BitmapSource format
			WICPixelFormatGUID pixelFormat;
			hr = ToRenderBitmapSource->GetPixelFormat(&pixelFormat);

			if (SUCCEEDED(hr))
			{
				hr = (pixelFormat == GUID_WICPixelFormat32bppBGR) ? S_OK : E_FAIL;
			}

			if (SUCCEEDED(hr))
			{
				hr = ToRenderBitmapSource->GetSize(&width, &height);
			}

			// Create a DIB section based on Bitmap Info
			// BITMAPINFO Struct must first be setup before a DIB can be created.
			// Note that the height is negative for top-down bitmaps
			if (SUCCEEDED(hr))
			{
				BITMAPINFO bminfo;
				ZeroMemory(&bminfo, sizeof(bminfo));
				bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bminfo.bmiHeader.biWidth = width;
				bminfo.bmiHeader.biHeight = -(LONG)height;
				bminfo.bmiHeader.biPlanes = 1;
				bminfo.bmiHeader.biBitCount = 32;
				bminfo.bmiHeader.biCompression = BI_RGB;

				// Get a DC for the full screen
				HDC hdcScreen = GetDC(NULL);

				hr = hdcScreen ? S_OK : E_FAIL;

				// Release the previously allocated bitmap 
				if (SUCCEEDED(hr))
				{
					if (hDIBBitmap)
					{
						ensure(DeleteObject(hDIBBitmap));
					}

					hDIBBitmap = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &ImageBits, NULL, 0);

					ReleaseDC(NULL, hdcScreen);

					hr = hDIBBitmap ? S_OK : E_FAIL;
				}
			}

			UINT cbStride = 0;
			if (SUCCEEDED(hr))
			{
				// Size of a scan line represented in bytes: 4 bytes each pixel
				hr = UIntMult(width, sizeof(DWORD), &cbStride);
			}

			UINT cbImage = 0;
			if (SUCCEEDED(hr))
			{
				// Size of the image, represented in bytes
				hr = UIntMult(cbStride, height, &cbImage);
			}

			// Extract the image into the HBITMAP    
			if (SUCCEEDED(hr) && ToRenderBitmapSource)
			{
				hr = ToRenderBitmapSource->CopyPixels(
					NULL,
					cbStride,
					cbImage,
					reinterpret_cast<BYTE *> (ImageBits));

				if (_mObjectsBeingCustomized.Num() > 0)
				{
					UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
					if (animation != nullptr)
					{
						if (frameIndex != 0)
						{
							OnClickAddFrame();
						}

						COLORREF* pColor = (COLORREF*)ImageBits;

						_mColors = UChromaSDKPluginBPLibrary::CreateColors2D(animation->Device);
						for (int i = 0; i < _mColors.Num() && i < (int)height; i++)
						{
							COLORREF* nextRow = pColor + width;
							for (int j = 0; j < _mColors[i].Colors.Num() && j < (int)width; j++)
							{
								float red = GetBValue(*pColor) / 255.0f;
								float green = GetGValue(*pColor) / 255.0f;
								float blue = GetRValue(*pColor) / 255.0f;
								FLinearColor color = FLinearColor(red, green, blue, 1.0f);
								_mColors[i].Colors[j] = color;
								pColor++;
								/*
								UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails r=%f g=%f b=%f"),
									red, green, blue);
								*/
							}
							pColor = nextRow;
						}

						OnClickPasteButton();
					}
				}
			}

			// Image Extraction failed, clear allocated memory
			if (FAILED(hr) && hDIBBitmap)
			{
				ensure(DeleteObject(hDIBBitmap));
				hDIBBitmap = NULL;
			}

			// done with bitmap
			if (hDIBBitmap)
			{
				ensure(DeleteObject(hDIBBitmap));
				hDIBBitmap = NULL;
			}
		}

		if (OriginalBitmapSource)
		{
			OriginalBitmapSource->Release();
		}

		if (ToRenderBitmapSource)
		{
			ToRenderBitmapSource->Release();
		}

		if (Frame)
		{
			Frame->Release();
		}
	}

	if (Decoder)
	{
		Decoder->Release();
	}

	if (Factory)
	{
		Factory->Release();
	}

#endif
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickImportTextureImageButton()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		FString texturePath;
		FString Filter = TEXT("Image Files (*.bmp;*.jpg;*.png)| *.bmp;*.jpg;*.png; ||");
		TArray<FString> OutFiles;
		const void* parentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
		if (DesktopPlatform->OpenFileDialog(
			parentWindowWindowHandle,
			TEXT("Import texture..."),
			FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT),
			TEXT(""),
			Filter,
			EFileDialogFlags::None,
			OutFiles))
		{
			if (OutFiles.Num() > 0)
			{
				texturePath = OutFiles[0];
				/*
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickImportTextureButton Selected=%s"),
					*texturePath);
				*/

				ReadImage(texturePath, false);
			}
		}
	}

	/*
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsLoaded())
			{
				animation->Unload();
			}
			animation->RefreshCurve();
			RefreshFrames();
			RefreshDevice();
		}
	}
	*/
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickImportTextureAnimationButton()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		FString texturePath;
		FString Filter = TEXT("Image Files (*.gif)| *.gif; ||");
		TArray<FString> OutFiles;
		const void* parentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
		if (DesktopPlatform->OpenFileDialog(
			parentWindowWindowHandle,
			TEXT("Import texture..."),
			FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT),
			TEXT(""),
			Filter,
			EFileDialogFlags::None,
			OutFiles))
		{
			if (OutFiles.Num() > 0)
			{
				texturePath = OutFiles[0];
				/*
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickImportTextureButton Selected=%s"),
					*texturePath);
				*/

				ReadImage(texturePath, true);
			}
		}
	}

	/*
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsLoaded())
			{
				animation->Unload();
			}
			animation->RefreshCurve();
			RefreshFrames();
			RefreshDevice();
		}
	}
	*/
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickOverrideButton()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsLoaded())
			{
				animation->Unload();
			}
			float frameTime = animation->OverrideFrameTime;
			float time = 0.0f;
			for (int i = 0; i < animation->Curve.EditorCurveData.Keys.Num(); ++i)
			{
				time += frameTime;
				animation->Curve.EditorCurveData.Keys[i].Time = time;
			}
			animation->RefreshCurve();
			RefreshFrames();
			RefreshDevice();
		}
	}
	return FReply::Handled();
}

TSharedRef<SColorBlock> FChromaSDKEditorAnimation2DDetails::SetupColorButton(int row, int column, const FLinearColor& color)
{
	TSharedRef<IChromaSDKEditorButton2D> button =
		IChromaSDKEditorButton2D::MakeInstance();
	button->Row = row;
	button->Column = column;
	button->Details = _mDetails;

	TSharedPtr<SColorBlock> ptrColor = button->CreateColorBlock(color);
	TSharedRef<SColorBlock> newColor = ptrColor.ToSharedRef();

	// store the button event reference
	_mColorButtons.Add(button);

	return newColor;
}

void FChromaSDKEditorAnimation2DDetails::RefreshDevice()
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
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(animation->Device);
			int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(animation->Device);

			FChromaSDKColorFrame2D& frame = animation->Frames[_mCurrentFrame];
			TArray<FChromaSDKColors>& colors = frame.Colors;

			if (colors.Num() == maxRow &&
				colors[0].Colors.Num() == maxColumn)
			{
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
}

void FChromaSDKEditorAnimation2DDetails::OnClickColor(int row, int column)
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

TSharedRef<SWidget> FChromaSDKEditorAnimation2DDetails::GenerateDropdownEnum(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	FString selectedItem = *Item;
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKDevices Selected=%s"),
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKDevices Item=%d"), k);
				break;
			}
		}
	}
}

void FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKKeyboardKeys(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	FString selectedItem = *Item;
	/*
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKKeyboardKeys Selected=%s"),
		*selectedItem);
	*/

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
				//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKKeyboardKeys Item=%d"), k);
				break;
			}
		}
	}
}

void FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKMouseLeds(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	FString selectedItem = *Item;
	/*
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKMouseLeds Selected=%s"),
		*selectedItem);
	*/

	// set the selected enum
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKMouseLed"), true);
	if (enumPtr)
	{
		for (int k = 0; (k + 1) < enumPtr->NumEnums(); ++k)
		{
			FString text = enumPtr->GetDisplayNameTextByValue(k).ToString();
			if (text == selectedItem)
			{
				_mSelectedLed = (EChromaSDKMouseLed)k;
				//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKMouseLeds Item=%d"), k);
				break;
			}
		}
	}
}

void FChromaSDKEditorAnimation2DDetails::OnColorCommitted(FLinearColor color)
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnColorCommitted"));
	color.A = 1.0f; //full alpha
	_mColor = color;
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickClearButton()
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickFillButton()
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
				FChromaSDKColorFrame2D& frame = frames[_mCurrentFrame];
				TArray<FChromaSDKColors>& colors = frame.Colors;
				int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(device);
				int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(device);
				for (int i = 0; i < maxRow; ++i)
				{
					for (int j = 0; j < maxColumn; ++j)
					{
						colors[i].Colors[j] = _mColor;
					}
				}
			}
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickCopyButton()
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickPasteButton()
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
				_mColors[0].Colors.Num() == maxColumn)
			{
				frames[_mCurrentFrame].Colors = _mColors;
			}
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickRandomButton()
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickSetDeviceButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickSetDeviceButton"));

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

FReply FChromaSDKEditorAnimation2DDetails::OnClickSetKeyButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickSetKeyButton"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			animation->Device == EChromaSDKDevice2DEnum::DE_Keyboard &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			TArray<FChromaSDKColors>& colors = frames[_mCurrentFrame].Colors;
			UChromaSDKPluginBPLibrary::SetKeyboardKeyColor(_mSelectedKey, _mColor, colors);
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickSetLedButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickSetLedButton"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr &&
			animation->Device == EChromaSDKDevice2DEnum::DE_Mouse &&
			_mCurrentFrame >= 0 &&
			_mCurrentFrame < animation->Frames.Num())
		{
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
			TArray<FChromaSDKColors>& colors = frames[_mCurrentFrame].Colors;
			UChromaSDKPluginBPLibrary::SetMouseLedColor(_mSelectedLed, _mColor, colors);
		}
	}

	// refresh the UI
	RefreshDevice();

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickPreviewButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickPreviewButton"));

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

FReply FChromaSDKEditorAnimation2DDetails::OnClickPlayButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickPlayButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			if (animation->IsPlaying())
			{
				UE_LOG(LogTemp, Error, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickPlayButton Animation is already playing!"));
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickStopButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickStopButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickLoadButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickLoadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickUnloadButton()
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickUnloadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
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

#endif
