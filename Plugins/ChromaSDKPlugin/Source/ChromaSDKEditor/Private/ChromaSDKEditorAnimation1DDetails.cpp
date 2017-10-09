// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//#include "ChromaSDKEditor.h" //(support 4.15 or below)___HACK_UE4_WANTS_MODULE_FIRST
#include "ChromaSDKEditorAnimation1DDetails.h"
#include "ChromaSDKEditor.h" //(support 4.16 or above)___HACK_UE4_WANTS_HEADER_FIRST

#if WITH_EDITOR
#include "ChromaSDKEditorButton1D.h"
#include "ChromaSDKPluginAnimation1DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h" 
#include "DesktopPlatformModule.h"
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
#endif

typedef unsigned char byte;
#define ANIMATION_VERSION 1
#define LOCTEXT_NAMESPACE "ChromaAnimation1DDetails"

TSharedRef<IDetailCustomization> FChromaSDKEditorAnimation1DDetails::MakeInstance()
{
	TSharedRef<FChromaSDKEditorAnimation1DDetails> instance = MakeShareable(new FChromaSDKEditorAnimation1DDetails);
#if PLATFORM_WINDOWS
	instance->_mDetails = instance;

	//populate list of device enums
	const UEnum* deviceEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EChromaSDKDevice1DEnum"), true);
	if (deviceEnumPtr)
	{
		for (int k = 0; (k + 1) < deviceEnumPtr->NumEnums(); ++k)
		{
			FString text = deviceEnumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->AddChromaSDKDevice(text);
		}
	}
	//set default key
	instance->_mSelectedDevice = EChromaSDKDevice1DEnum::DE_ChromaLink;

	// initialize
	instance->Initialize();

#endif
    
	return instance;
}

UChromaSDKPluginAnimation1DObject* FChromaSDKEditorAnimation1DDetails::GetAnimation()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		return animation;
	}
	else
	{
		return nullptr;
	}
}

#if PLATFORM_WINDOWS

void FChromaSDKEditorAnimation1DDetails::ReadChromaFile(const FString& path)
{
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		const char* strPath = TCHAR_TO_ANSI(*path);
		fprintf(stdout, "OpenAnimation: %s\r\n", strPath);
		FILE* stream;
		if (0 == fopen_s(&stream, strPath, "rb") &&
			stream)
		{
			long read = 0;
			long expectedRead = 1;
			long expectedSize = sizeof(byte);

			//version
			int version = 0;
			expectedSize = sizeof(int);
			read = fread(&version, expectedSize, 1, stream);
			if (read != expectedRead)
			{
				fprintf(stderr, "OpenAnimation: Failed to read version!\r\n");
				std::fclose(stream);
				return;
			}
			if (version != ANIMATION_VERSION)
			{
				fprintf(stderr, "OpenAnimation: Unexpected Version!\r\n");
				std::fclose(stream);
				return;
			}

			fprintf(stdout, "OpenAnimation: Version: %d\r\n", version);

			//device
			byte device = 0;

			// device type
			byte deviceType = 0;
			expectedSize = sizeof(byte);
			read = fread(&deviceType, expectedSize, 1, stream);
			if (read == expectedRead)
			{
				//device
				switch ((EChromaSDKDeviceTypeEnum)deviceType)
				{
				case EChromaSDKDeviceTypeEnum::DE_1D:
				case EChromaSDKDeviceTypeEnum::DE_2D:
					break;
				default:
					fprintf(stderr, "OpenAnimation: Unexpected DeviceType!\r\n");
					std::fclose(stream);
					return;
				}

				switch ((EChromaSDKDeviceTypeEnum)deviceType)
				{
				case EChromaSDKDeviceTypeEnum::DE_1D:
					read = fread(&device, expectedSize, 1, stream);
					if (read == expectedRead)
					{
						// device
						animation->Device = (EChromaSDKDevice1DEnum)device;

						//frame count
						int frameCount;

						expectedSize = sizeof(int);
						read = fread(&frameCount, expectedSize, 1, stream);
						if (read != expectedRead)
						{
							fprintf(stderr, "OpenAnimation: Error detected reading frame count!\r\n");
							std::fclose(stream);
							return;
						}
						else
						{
							//time
							float time = 0;
							animation->Curve.EditorCurveData.Reset();
							animation->Curve.EditorCurveData.Keys.Reset();

							//frames
							TArray<FChromaSDKColorFrame1D>& frames = animation->GetFrames();
							frames.Reset();

							for (int index = 0; index < frameCount; ++index)
							{
								FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
								int maxLeds = UChromaSDKPluginBPLibrary::GetMaxLeds(animation->Device);

								//duration
								float duration = 0.0f;
								expectedSize = sizeof(float);
								read = fread(&duration, expectedSize, 1, stream);
								if (read != expectedRead)
								{
									fprintf(stderr, "OpenAnimation: Error detected reading duration!\r\n");
									std::fclose(stream);
									return;
								}
								else
								{
									// set duration
									if (duration < 0.1f)
									{
										duration = 0.1f;
									}
									time += duration;
									animation->Curve.EditorCurveData.AddKey(time, 0.0f);

									// colors
									expectedSize = sizeof(int);
									for (int i = 0; i < maxLeds; ++i)
									{
										int color = 0;
										read = fread(&color, expectedSize, 1, stream);
										if (read != expectedRead)
										{
											fprintf(stderr, "OpenAnimation: Error detected reading color!\r\n");
											std::fclose(stream);
											return;
										}
										else
										{
											float red = GetRValue(color) / 255.0f;
											float green = GetGValue(color) / 255.0f;
											float blue = GetBValue(color) / 255.0f;
											FLinearColor linearColor = FLinearColor(red, green, blue, 1.0f);
											frame.Colors.Add(linearColor);
										}
									}
									frames.Add(frame);
								}
							}

							animation->RefreshCurve();
						}
					}
					break;
				}
			}

			std::fclose(stream);
		}
		RefreshFrames();
		RefreshDevice();
	}
}

void FChromaSDKEditorAnimation1DDetails::WriteChromaFile(const FString& path)
{
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		const char* strPath = TCHAR_TO_ANSI(*path);
		fprintf(stdout, "WriteChromaFile: %s\r\n", strPath);
		FILE* stream;
		if (0 == fopen_s(&stream, strPath, "wb") &&
			stream)
		{
			long write = 0;
			long expectedWrite = 1;
			long expectedSize = 0;

			//version
			int version = ANIMATION_VERSION;
			expectedSize = sizeof(int);
			write = fwrite(&version, expectedSize, 1, stream);
			if (expectedWrite != write)
			{
				fprintf(stderr, "SaveFile: Failed to write version!\r\n");
				std::fclose(stream);
				return;
			}

			//device
			byte device = 0;

			//device type
			byte deviceType = (byte)EChromaSDKDeviceTypeEnum::DE_1D;
			expectedSize = sizeof(byte);
			fwrite(&deviceType, expectedSize, 1, stream);

			//device
			device = (byte)animation->Device;
			fwrite(&device, expectedSize, 1, stream);

			//frames
			TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;

			//frame count
			unsigned int frameCount = frames.Num();
			expectedSize = sizeof(unsigned int);
			fwrite(&frameCount, expectedSize, 1, stream);

			//frames
			for (int index = 0; index < frames.Num(); ++index)
			{
				//duration
				float duration = GetDuration(index);
				if (duration < 0.1f)
				{
					duration = 0.1f;
				}
				expectedSize = sizeof(float);
				fwrite(&duration, expectedSize, 1, stream);

				//colors
				FChromaSDKColorFrame1D& frame = frames[index];
				for (int i = 0; i < frame.Colors.Num(); ++i)
				{
					//color
					FLinearColor& color = frame.Colors[i];
					int red = color.R * 255;
					int green = color.G * 255;
					int blue = color.B * 255;
					int bgrInt = RGB(red, green, blue);
					expectedSize = sizeof(int);
					fwrite(&bgrInt, expectedSize, 1, stream);
				}
			}

			fflush(stream);
			std::fclose(stream);
		}
	}
}

#endif

void FChromaSDKEditorAnimation1DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::CustomizeDetails"));

	if (_mObjectsBeingCustomized.Num() > 0)
	{
		_mObjectsBeingCustomized.Empty();
	}

	//set the default frame
	_mCurrentFrame = 0;

	DetailBuilder.GetObjectsBeingCustomized(/*out*/ _mObjectsBeingCustomized);

	BuildImportRow(DetailBuilder);

	IDetailCategoryBuilder& category = DetailBuilder.EditCategory("Animation", LOCTEXT("Animation", "Animation"), ECategoryPriority::Important);

	BuildDeviceRow(DetailBuilder, category);

	BuildApplyRow(DetailBuilder, category);

	BuildInfoRow(DetailBuilder, category);

	BuildPreviewRow(DetailBuilder, category);

	BuildColorRow(DetailBuilder, category);

	BuildFramesRow(DetailBuilder, category);

	BuildCurveRow(DetailBuilder, category);
#endif
}

float FChromaSDKEditorAnimation1DDetails::GetDuration(int index)
{
	if (index < 0)
	{
		return 0;
	}
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (index >= animation->Curve.EditorCurveData.Keys.Num())
		{
			return 0;
		}
		float duration = 0.0f;
		if (index >= 0 &&
			index < animation->Curve.EditorCurveData.Keys.Num())
		{
			if (index == 0)
			{
				return animation->Curve.EditorCurveData.Keys[index].Time;
			}
			else
			{
				return
					animation->Curve.EditorCurveData.Keys[index].Time -
					animation->Curve.EditorCurveData.Keys[index - 1].Time;
			}
		}
	}
#endif
	return 0;
}

void FChromaSDKEditorAnimation1DDetails::RefreshFrames()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (_mCurrentFrame < 0 ||
			_mCurrentFrame >= animation->Frames.Num())
		{
			_mCurrentFrame = 0;
		}
		const EChromaSDKDevice1DEnum& device = animation->Device;
		_mTextCurrentFrame->SetText(FText::AsNumber(_mCurrentFrame + 1));
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
					animation->Curve.EditorCurveData.Keys[_mCurrentFrame - 1].Time;
			}
		}
		_mTextFrameDuration->SetText(FText::AsNumber(duration));
		return;
	}

	_mCurrentFrame = 0;
	_mTextCurrentFrame->SetText(FText::AsNumber(0));
	_mTextNumberOfFrames->SetText(FText::AsNumber(0));
	_mTextFrameDuration->SetText(FText::AsNumber(0.0f));
#endif
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickFirstFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		_mCurrentFrame = 0;
		animation->RefreshCurve();
		RefreshFrames();
		RefreshDevice();
		return FReply::Handled();
	}
#endif
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickPreviousFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

	_mCurrentFrame = 0;
#endif
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickNextFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (_mCurrentFrame < 0 ||
			_mCurrentFrame >= animation->Frames.Num())
		{
			_mCurrentFrame = 0;
		}
		if ((_mCurrentFrame + 1) < animation->Frames.Num())
		{
			++_mCurrentFrame;
		}
		animation->RefreshCurve();
		RefreshFrames();
		RefreshDevice();
		return FReply::Handled();
	}

	_mCurrentFrame = 0;
#endif
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickLastFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (_mCurrentFrame < 0 ||
			_mCurrentFrame >= animation->Frames.Num())
		{
			_mCurrentFrame = 0;
		}
		if (animation->Frames.Num() > 0)
		{
			_mCurrentFrame = animation->Frames.Num() - 1;
		}
		animation->RefreshCurve();
		RefreshFrames();
		RefreshDevice();
		return FReply::Handled();
	}
#endif
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickAddFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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
			(_mCurrentFrame + 1) == animation->Frames.Num())
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

	_mCurrentFrame = 0;
#endif
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickDeleteFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

	_mCurrentFrame = 0;
#endif
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickResetFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (animation->IsLoaded())
		{
			animation->Unload();
		}
		_mCurrentFrame = 0;
		while (animation->Frames.Num() > 1)
		{
			animation->Frames.RemoveAt(0);
		}
		if (animation->Frames.Num() == 1)
		{
			// reset frame
			animation->Frames[0].Colors = UChromaSDKPluginBPLibrary::CreateColors1D(animation->Device);
		}
		animation->RefreshCurve();
		RefreshFrames();
		RefreshDevice();
		return FReply::Handled();
	}
#endif
	return FReply::Handled();
}

#if PLATFORM_WINDOWS
void FChromaSDKEditorAnimation1DDetails::CopyPixels(COLORREF* pColor, UINT width, UINT height)
{
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		_mColors = UChromaSDKPluginBPLibrary::CreateColors1D(animation->Device);
		for (int i = 0; i < (int)height; i++)
		{
			COLORREF* nextRow = pColor + width;
			for (int j = 0; j < _mColors.Num() && j < (int)width; j++)
			{
				float red = GetBValue(*pColor) / 255.0f;
				float green = GetGValue(*pColor) / 255.0f;
				float blue = GetRValue(*pColor) / 255.0f;
				FLinearColor color = FLinearColor(red, green, blue, 1.0f);
				_mColors[j] = color;
				pColor++;
				/*
				UE_LOG(LogTemp, Log, TEXT("IChromaSDKEditorAnimationBaseDetails r=%f g=%f b=%f"),
				red, green, blue);
				*/
			}
			pColor = nextRow;
		}
	}
}
#endif

// import chroma animation
FReply FChromaSDKEditorAnimation1DDetails::OnClickImportButton()
{
#if PLATFORM_WINDOWS
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		FString importPath;
		FString Filter = TEXT("Chroma Files (*.chroma)| *.chroma; ||");
		TArray<FString> OutFiles;
		if (DesktopPlatform->OpenFileDialog(
			GetParentWindowWindowHandle(),
			TEXT("Import Chroma..."),
			FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT),
			TEXT(""),
			Filter,
			EFileDialogFlags::None,
			OutFiles))
		{
			if (OutFiles.Num() > 0)
			{
				importPath = OutFiles[0];
				/*
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickImportButton Selected=%s"),
				*importPath);
				*/

				ReadChromaFile(importPath);
			}
		}
	}
#endif
	return FReply::Handled();
}

// export chroma animation
FReply FChromaSDKEditorAnimation1DDetails::OnClickExportButton()
{
#if PLATFORM_WINDOWS
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		FString exportPath;
		FString Filter = TEXT("Chroma Files (*.chroma)| *.chroma ||");
		TArray<FString> OutFiles;
		if (DesktopPlatform->SaveFileDialog(
			GetParentWindowWindowHandle(),
			TEXT("Export Chroma..."),
			FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT),
			TEXT(""),
			Filter,
			EFileDialogFlags::None,
			OutFiles))
		{
			if (OutFiles.Num() > 0)
			{
				exportPath = OutFiles[0];
				/*
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickExportButton Selected=%s"),
				*importPath);
				*/

				WriteChromaFile(exportPath);
			}
		}
	}
#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickImportTextureImageButton()
{
#if PLATFORM_WINDOWS
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		FString texturePath;
		FString Filter = TEXT("Image Files (*.bmp;*.jpg;*.png)| *.bmp;*.jpg;*.png; ||");
		TArray<FString> OutFiles;
		if (DesktopPlatform->OpenFileDialog(
			GetParentWindowWindowHandle(),
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickImportTextureButton Selected=%s"),
				*texturePath);
				*/

				ReadImage(texturePath, false);
			}
		}
	}

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickImportTextureAnimationButton()
{
#if PLATFORM_WINDOWS
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		FString texturePath;
		FString Filter = TEXT("Image Files (*.gif)| *.gif; ||");
		TArray<FString> OutFiles;
		if (DesktopPlatform->OpenFileDialog(
			GetParentWindowWindowHandle(),
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickImportTextureButton Selected=%s"),
				*texturePath);
				*/

				ReadImage(texturePath, true);
			}
		}
	}

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickOverrideButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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
#endif
	return FReply::Handled();
}

#if PLATFORM_WINDOWS
TSharedRef<SColorBlock> FChromaSDKEditorAnimation1DDetails::SetupColorButton(int element, const FLinearColor& color)
{
	TSharedRef<IChromaSDKEditorButton1D> button =
		IChromaSDKEditorButton1D::MakeInstance();
	button->Element = element;
	button->Details = _mDetails;

	TSharedPtr<SColorBlock> ptrColor = button->CreateColorBlock(color);
	TSharedRef<SColorBlock> newColor = ptrColor.ToSharedRef();

	// store the button event reference
	_mColorButtons.Add(button);

	return newColor;
}
#endif

void FChromaSDKEditorAnimation1DDetails::RefreshDevice()
{
#if PLATFORM_WINDOWS
	// Remove existing button events
	if (_mColorButtons.Num() > 0)
	{
		_mColorButtons.Empty();
	}

	// clear grid
	_mGrid->ClearChildren();

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		int maxLeds = UChromaSDKPluginBPLibrary::GetMaxLeds(animation->Device);

		FFormatNamedArguments textArgs;
		textArgs.Add(TEXT("maxLeds"), FText::AsNumber(maxLeds));
		_mTextInfo->SetText(FText::Format(LOCTEXT("GridInfo", "1 x {maxLeds}"), textArgs));

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
#endif
}

void FChromaSDKEditorAnimation1DDetails::OnClickColor(int element)
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

	RefreshDevice();
#endif
}

void FChromaSDKEditorAnimation1DDetails::OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
#if PLATFORM_WINDOWS
	FString selectedItem = *Item;
	UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnChangeChromaSDKDevices Selected=%s"),
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnChangeChromaSDKDevices Item=%d"), k);
				break;
			}
		}
	}
#endif
}

void FChromaSDKEditorAnimation1DDetails::OnColorCommitted(FLinearColor color)
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnColorCommitted"));
	color.A = 1.0f; //full alpha
	_mColor = color;
#endif
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickClearButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickFillButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickCopyButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		const EChromaSDKDevice1DEnum& device = animation->Device;
		TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
		FChromaSDKColorFrame1D& frame = frames[_mCurrentFrame];
		_mColors = frame.Colors;
	}

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickPasteButton()
{
#if PLATFORM_WINDOWS
	// refresh the UI
	RefreshDevice();

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickRandomButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		const EChromaSDKDevice1DEnum& device = animation->Device;
		TArray<FChromaSDKColorFrame1D>& frames = animation->Frames;
		frames[_mCurrentFrame].Colors = UChromaSDKPluginBPLibrary::CreateRandomColors1D(animation->Device);
	}

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickSetDeviceButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickSetDeviceButton"));

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickPreviewButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickApplyButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickPlayButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickPlayButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (animation->IsPlaying())
		{
			UE_LOG(LogTemp, Error, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickPlayButton Animation is already playing!"));
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickStopButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickStopButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (animation->IsPlaying())
		{
			animation->Stop();
		}
	}

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickLoadButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickLoadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (!animation->IsLoaded())
		{
			animation->Load();
		}
	}

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickUnloadButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnClickUnloadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (animation->IsLoaded())
		{
			animation->Unload();
		}
	}

#endif
	return FReply::Handled();
}

/* implements FCurveOwnerInterface */

/** Returns set of curves to edit. Must not release the curves while being edited. */
TArray<FRichCurveEditInfoConst> FChromaSDKEditorAnimation1DDetails::GetCurves() const
{
	TArray<FRichCurveEditInfoConst> result = TArray<FRichCurveEditInfoConst>();
	return result;
}

/** Returns set of curves to query. Must not release the curves while being edited. */
TArray<FRichCurveEditInfo> FChromaSDKEditorAnimation1DDetails::GetCurves()
{
	TArray<FRichCurveEditInfo> result = TArray<FRichCurveEditInfo>();
	UChromaSDKPluginAnimation1DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		result.Add(&animation->Curve.EditorCurveData);
	}
	return result;
}

/** Called to modify the owner of the curve */
void FChromaSDKEditorAnimation1DDetails::ModifyOwner()
{

}

/** Returns the owner(s) of the curve */
TArray<const UObject*> FChromaSDKEditorAnimation1DDetails::GetOwners() const
{
	TArray<const UObject*> result = TArray<const UObject*>();
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			result.Add(animation);
		}
	}
	return result;
}

/** Called to make curve owner transactional */
void FChromaSDKEditorAnimation1DDetails::MakeTransactional()
{

}

/** Called when any of the curves have been changed */
void FChromaSDKEditorAnimation1DDetails::OnCurveChanged(const TArray<FRichCurveEditInfo>& ChangedCurveEditInfos)
{

}

/** Validates that a previously retrieved curve is still valid for editing. */
bool FChromaSDKEditorAnimation1DDetails::IsValidCurve(FRichCurveEditInfo CurveInfo)
{
	return true;
}

/* end of FCurveOwnerInterface */

#undef LOCTEXT_NAMESPACE

#if PLATFORM_WINDOWS
#include "HideWindowsPlatformTypes.h"
#endif

#endif
