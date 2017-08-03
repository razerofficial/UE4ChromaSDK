// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKEditor.h"
#if WITH_EDITOR
#include "ChromaSDKEditorAnimationBaseDetails.h"
#include "ChromaSDKPluginBPLibrary.h"
#include "DesktopPlatformModule.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EditorDirectories.h"
#include "PropertyCustomizationHelpers.h"
#include "SCurveEditor.h"
#include "SlateApplication.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SGridPanel.h"

#if PLATFORM_WINDOWS
#include "objbase.h"
#include "Windows/WindowsHWrapper.h"
THIRD_PARTY_INCLUDES_START
#include <strsafe.h>
#include <wincodec.h>
THIRD_PARTY_INCLUDES_END
#pragma comment( lib, "windowscodecs.lib" )
#endif

#define LOCTEXT_NAMESPACE "ChromaAnimationBaseDetails"

void IChromaSDKEditorAnimationBaseDetails::Initialize()
{
	//set the default frame
	_mCurrentFrame = 0;

	//show custom property
	_mCurveWidget.Reset();
}

void IChromaSDKEditorAnimationBaseDetails::ReadImage(const FString& path, bool isAnimation)
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

	if (isAnimation)
	{
		if (SUCCEEDED(hr))
		{
			hr = (Decoder->GetFrameCount(&frameCount));
			/*
			UE_LOG(LogTemp, Log, TEXT("IChromaSDKEditorAnimationBaseDetails frameCount=%d"),
			(int)frameCount);
			*/
		}
	}

	for (UINT frameIndex = 0; frameIndex < frameCount; ++frameIndex)
	{
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
					if (frameIndex != 0)
					{
						OnClickAddFrame();
					}

					COLORREF* pColor = (COLORREF*)ImageBits;
					CopyPixels(pColor, width, height);

					OnClickPasteButton();
				}
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

void IChromaSDKEditorAnimationBaseDetails::AddChromaSDKDevice(FString& text)
{
	_mChromaSDKDevices.Add(MakeShared<FString>(text));
}

TSharedRef<SWidget> IChromaSDKEditorAnimationBaseDetails::GenerateDropdownEnum(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

void IChromaSDKEditorAnimationBaseDetails::BuildImportRow(IDetailLayoutBuilder& DetailBuilder)
{
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
				.Text(LOCTEXT("Import", "Import"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickImportButton)
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Export", "Export"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickExportButton)
			]
			+ SGridPanel::Slot(2, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Import image", "Import image"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickImportTextureImageButton)
			]
			+ SGridPanel::Slot(3, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Import texture", "Import animation"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickImportTextureAnimationButton)
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
			.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickOverrideButton)
		];
}

void IChromaSDKEditorAnimationBaseDetails::BuildDeviceRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category)
{
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
				.OnGenerateWidget(this, &IChromaSDKEditorAnimationBaseDetails::GenerateDropdownEnum)
				.OnSelectionChanged(this, &IChromaSDKEditorAnimationBaseDetails::OnChangeChromaSDKDevices)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Select a device", "Select a device"))
				]
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Set device", "Set device"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickSetDeviceButton)
			]
		];
}

void IChromaSDKEditorAnimationBaseDetails::BuildApplyRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category)
{
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
			.FillColumn(0, 4.0f)
			.FillColumn(1, 4.0f)
			.FillColumn(2, 4.0f)
			.FillColumn(3, 4.0f)
			.FillColumn(4, 4.0f)
			+ SGridPanel::Slot(0, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Clear", "Clear"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickClearButton)
			]
			+ SGridPanel::Slot(1, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Fill", "Fill"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickFillButton)
			]
			+ SGridPanel::Slot(2, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Random", "Random"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickRandomButton)
			]
			+ SGridPanel::Slot(3, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Copy", "Copy"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickCopyButton)
			]
			+ SGridPanel::Slot(4, 0)
			[
				SNew(SButton)
				.Text(LOCTEXT("Paste", "Paste"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickPasteButton)
			]
			+ SGridPanel::Slot(0, 1)
			[
				SNew(SButton)
				.Text(LOCTEXT("Preview", "Preview"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickPreviewButton)
			]
			+ SGridPanel::Slot(1, 1)
			[
				SNew(SButton)
				.Text(LOCTEXT("Play", "Play"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickPlayButton)
			]
			+ SGridPanel::Slot(2, 1)
			[
				SNew(SButton)
				.Text(LOCTEXT("Stop", "Stop"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickStopButton)
			]
			+ SGridPanel::Slot(3, 1)
			[
				SNew(SButton)
				.Text(LOCTEXT("Load", "Load"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickLoadButton)
			]
			+ SGridPanel::Slot(4, 1)
			[
				SNew(SButton)
				.Text(LOCTEXT("Unload", "Unload"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickUnloadButton)
			]
		];
}

void IChromaSDKEditorAnimationBaseDetails::BuildInfoRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category)
{
	TSharedRef<STextBlock> textInfo = SNew(STextBlock)
		.Text(LOCTEXT("0", "0"));
	_mTextInfo = textInfo;

	category.AddCustomRow(FText::FromString(LOCTEXT("Info", "Info").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Info", "Info"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			_mTextInfo.ToSharedRef()			
		];
}

void IChromaSDKEditorAnimationBaseDetails::BuildPreviewRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category)
{
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
}

void IChromaSDKEditorAnimationBaseDetails::BuildColorRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category)
{
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
			.OnColorCommitted(this, &IChromaSDKEditorAnimationBaseDetails::OnColorCommitted)
		];
}

void IChromaSDKEditorAnimationBaseDetails::BuildFramesRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category)
{
	TSharedRef<STextBlock> textCurrentFrame = SNew(STextBlock)
		.Text(LOCTEXT("0", "0"));
	_mTextCurrentFrame = textCurrentFrame;

	TSharedRef<STextBlock> textNumberOfFrames = SNew(STextBlock)
		.Text(LOCTEXT("0", "0"));
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
				.Text(LOCTEXT("First", "First"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickFirstFrame)
			]
			+ SGridPanel::Slot(1, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Previous", "Previous"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickPreviousFrame)
			]
			+ SGridPanel::Slot(2, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Next","Next"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickNextFrame)
			]
			+ SGridPanel::Slot(3, 2)
			[
				SNew(SButton)
				.Text(LOCTEXT("Last", "Last"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickLastFrame)
			]
			+ SGridPanel::Slot(0, 3)
			[
				SNew(SButton)
				.Text(LOCTEXT("Add","Add"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickAddFrame)
			]
			+ SGridPanel::Slot(1, 3)
			[
				SNew(SButton)
				.Text(LOCTEXT("Delete","Delete"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickDeleteFrame)
			]
			+ SGridPanel::Slot(2, 3)
			[
				SNew(SButton)
				.Text(LOCTEXT("Reset","Reset"))
				.OnClicked(this, &IChromaSDKEditorAnimationBaseDetails::OnClickResetFrame)
			]
		];

		RefreshFrames();
}

void IChromaSDKEditorAnimationBaseDetails::BuildCurveRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category)
{
	//hide default property
	DetailBuilder.HideProperty(DetailBuilder.GetProperty("Curve"));

	//show custom property
	category.AddCustomRow(FText::FromString(LOCTEXT("Custom Curve", "Custom Curve").ToString()))
		.NameContent()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Custom Curve", "Custom Curve"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent().MinDesiredWidth(300)
		[
			SAssignNew(_mCurveWidget, SCurveEditor)
			.ShowZoomButtons(false)
			.XAxisName(FString(TEXT("Time")))
			.DesiredSize(FVector2D(512, 128))
		];

	_mCurveWidget->SetCurveOwner(this);
}

#undef LOCTEXT_NAMESPACE

#endif
