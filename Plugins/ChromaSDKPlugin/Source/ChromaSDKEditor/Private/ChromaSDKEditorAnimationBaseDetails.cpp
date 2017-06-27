// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#if WITH_EDITOR

#include "ChromaSDKEditorAnimationBaseDetails.h"
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

	for (UINT frameIndex = 0; frameIndex < frameCount; ++frameIndex)
	{
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

#undef LOCTEXT_NAMESPACE

#endif
