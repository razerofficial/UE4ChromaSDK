// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//#include "ChromaSDKEditor.h" //(support 4.15 or below)___HACK_UE4_WANTS_MODULE_FIRST
#include "ChromaSDKEditorAnimation2DDetails.h"
#include "ChromaSDKEditor.h" //(support 4.16 or above)___HACK_UE4_WANTS_HEADER_FIRST

#if WITH_EDITOR
#include "ChromaSDKEditorButton2D.h"
#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

#include "AllowWindowsPlatformTypes.h" 
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
#include "SharedPointer.h"

typedef unsigned char byte;
#define ANIMATION_VERSION 1
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
			instance->AddChromaSDKDevice(text);
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
#if (ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION > 12)
			FString text = keyEnumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->_mChromaSDKKeyboardKeys.Add(MakeShared<FString>(text));

#else
			FString* text = new FString(keyEnumPtr->GetDisplayNameTextByValue(k).ToString());
			instance->_mChromaSDKKeyboardKeys.Add(MakeShareable<FString>(text));
#endif
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
#if (ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION > 12)
			FString text = ledEnumPtr->GetDisplayNameTextByValue(k).ToString();
			instance->_mChromaSDKMouseLeds.Add(MakeShared<FString>(text));
#else
			FString* text = new FString(ledEnumPtr->GetDisplayNameTextByValue(k).ToString());
			instance->_mChromaSDKMouseLeds.Add(MakeShareable<FString>(text));
#endif
		}
	}
	//set default led
	instance->_mSelectedLed = EChromaSDKMouseLed::ML_LOGO;

	// initialize
	instance->Initialize();

	return instance;
}

UChromaSDKPluginAnimation2DObject* FChromaSDKEditorAnimation2DDetails::GetAnimation()
{
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		return animation;
	}
	else
	{
		return nullptr;
	}
}

#if PLATFORM_WINDOWS

void FChromaSDKEditorAnimation2DDetails::ReadChromaFile(const FString& path)
{
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
				case EChromaSDKDeviceTypeEnum::DE_2D:
					read = fread(&device, expectedSize, 1, stream);
					if (read == expectedRead)
					{
						//device
						animation->Device = (EChromaSDKDevice2DEnum)device;

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
							TArray<FChromaSDKColorFrame2D>& frames = animation->GetFrames();
							frames.Reset();

							for (int index = 0; index < frameCount; ++index)
							{
								FChromaSDKColorFrame2D frame = FChromaSDKColorFrame2D();
								int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(animation->Device);
								int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(animation->Device);

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
									for (int i = 0; i < maxRow; ++i)
									{
										FChromaSDKColors row = FChromaSDKColors();
										for (int j = 0; j < maxColumn; ++j)
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
												row.Colors.Add(linearColor);
											}
										}
										frame.Colors.Add(row);
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

void FChromaSDKEditorAnimation2DDetails::WriteChromaFile(const FString& path)
{
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
			byte deviceType = (byte)EChromaSDKDeviceTypeEnum::DE_2D;
			expectedSize = sizeof(byte);
			fwrite(&deviceType, expectedSize, 1, stream);

			//device
			device = (byte)animation->Device;
			fwrite(&device, expectedSize, 1, stream);

			//frames
			TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;

			//frame count
			unsigned int frameCount = frames.Num();
			expectedSize = sizeof(unsigned int);
			fwrite(&frameCount, expectedSize, 1, stream);

			//frames
			COLORREF color = RGB(0, 0, 0);
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
				FChromaSDKColorFrame2D& frame = frames[index];
				TArray<FChromaSDKColors>& colors = frame.Colors;
				for (int i = 0; i < colors.Num(); ++i)
				{
					FChromaSDKColors& row = colors[i];
					TArray<FLinearColor>& rowColors = colors[i].Colors;
					for (int j = 0; j < rowColors.Num(); ++j)
					{
						//color
						FLinearColor& color = rowColors[j];
						int red = color.R * 255;
						int green = color.G * 255;
						int blue = color.B * 255;
						int bgrInt = RGB(red, green, blue);
						expectedSize = sizeof(int);
						fwrite(&bgrInt, expectedSize, 1, stream);
					}
				}
			}

			fflush(stream);
			std::fclose(stream);
		}
	}
}

#endif

void FChromaSDKEditorAnimation2DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::CustomizeDetails"));

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

	BuildColorRow(DetailBuilder, category);

	BuildFramesRow(DetailBuilder, category);

	BuildCurveRow(DetailBuilder, category);
#endif
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

float FChromaSDKEditorAnimation2DDetails::GetDuration(int index)
{
	if (index < 0)
	{
		return 0;
	}
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

void FChromaSDKEditorAnimation2DDetails::RefreshFrames()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		if (_mCurrentFrame < 0 ||
			_mCurrentFrame >= animation->Frames.Num())
		{
			_mCurrentFrame = 0;
		}
		const EChromaSDKDevice2DEnum& device = animation->Device;
		_mTextCurrentFrame->SetText(FText::AsNumber(_mCurrentFrame + 1));
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickFirstFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickPreviousFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickNextFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickLastFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickAddFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickDeleteFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

	_mCurrentFrame = 0;
#endif
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation2DDetails::OnClickResetFrame()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
			animation->Frames[0].Colors = UChromaSDKPluginBPLibrary::CreateColors2D(animation->Device);
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
void FChromaSDKEditorAnimation2DDetails::CopyPixels(COLORREF* pColor, UINT width, UINT height)
{
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickImportButton()
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickImportButton Selected=%s"),
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
FReply FChromaSDKEditorAnimation2DDetails::OnClickExportButton()
{
#if PLATFORM_WINDOWS
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		FString exportPath;
		FString Filter = TEXT("Chroma Files (*.chroma)| *.chroma; ||");
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickExportButton Selected=%s"),
				*importPath);
				*/

				WriteChromaFile(exportPath);
			}
		}
	}
#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickImportTextureImageButton()
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickImportTextureButton Selected=%s"),
					*texturePath);
				*/

				ReadImage(texturePath, false);
			}
		}
	}

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickImportTextureAnimationButton()
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
				UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickImportTextureButton Selected=%s"),
					*texturePath);
				*/

				ReadImage(texturePath, true);
			}
		}
	}

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickOverrideButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
#endif

void FChromaSDKEditorAnimation2DDetails::RefreshDevice()
{
#if PLATFORM_WINDOWS
	// Remove existing button events
	if (_mColorButtons.Num() > 0)
	{
		_mColorButtons.Empty();
	}

	// clear grid
	_mGrid->ClearChildren();

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(animation->Device);
		int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(animation->Device);

		FFormatNamedArguments textArgs;
		textArgs.Add(TEXT("maxRow"), FText::AsNumber(maxRow));
		textArgs.Add(TEXT("maxColumn"), FText::AsNumber(maxColumn));
		_mTextInfo->SetText(FText::Format(LOCTEXT("GridInfo", "{maxRow} x {maxColumn}"), textArgs));

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
#endif
}

void FChromaSDKEditorAnimation2DDetails::OnClickColor(int row, int column)
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

	RefreshDevice();
#endif
}



void FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
#if PLATFORM_WINDOWS
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
#endif
}

void FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKKeyboardKeys(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
#if PLATFORM_WINDOWS
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
#endif
}

void FChromaSDKEditorAnimation2DDetails::OnChangeChromaSDKMouseLeds(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
#if PLATFORM_WINDOWS
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
#endif
}

void FChromaSDKEditorAnimation2DDetails::OnColorCommitted(FLinearColor color)
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnColorCommitted"));
	color.A = 1.0f; //full alpha
	_mColor = color;
#endif
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickClearButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickFillButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickCopyButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
		_mColors = frames[_mCurrentFrame].Colors;
	}

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickPasteButton()
{
#if PLATFORM_WINDOWS
	// refresh the UI
	RefreshDevice();

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickRandomButton()
{
#if PLATFORM_WINDOWS
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		const EChromaSDKDevice2DEnum& device = animation->Device;
		TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
		frames[_mCurrentFrame].Colors = UChromaSDKPluginBPLibrary::CreateRandomColors2D(animation->Device);
	}

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickSetDeviceButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickSetDeviceButton"));

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickSetKeyButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickSetKeyButton"));

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr &&
		animation->Device == EChromaSDKDevice2DEnum::DE_Keyboard &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
		TArray<FChromaSDKColors>& colors = frames[_mCurrentFrame].Colors;
		UChromaSDKPluginBPLibrary::SetKeyboardKeyColor(_mSelectedKey, _mColor, colors);
	}

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickSetLedButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickSetLedButton"));

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr &&
		animation->Device == EChromaSDKDevice2DEnum::DE_Mouse &&
		_mCurrentFrame >= 0 &&
		_mCurrentFrame < animation->Frames.Num())
	{
		TArray<FChromaSDKColorFrame2D>& frames = animation->Frames;
		TArray<FChromaSDKColors>& colors = frames[_mCurrentFrame].Colors;
		UChromaSDKPluginBPLibrary::SetMouseLedColor(_mSelectedLed, _mColor, colors);
	}

	// refresh the UI
	RefreshDevice();

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickPreviewButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickPreviewButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickPlayButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickPlayButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

#endif
	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation2DDetails::OnClickStopButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickStopButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickLoadButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickLoadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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

FReply FChromaSDKEditorAnimation2DDetails::OnClickUnloadButton()
{
#if PLATFORM_WINDOWS
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation2DDetails::OnClickUnloadButton"));

	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
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
TArray<FRichCurveEditInfoConst> FChromaSDKEditorAnimation2DDetails::GetCurves() const
{
	TArray<FRichCurveEditInfoConst> result = TArray<FRichCurveEditInfoConst>();
	return result;
}

/** Returns set of curves to query. Must not release the curves while being edited. */
TArray<FRichCurveEditInfo> FChromaSDKEditorAnimation2DDetails::GetCurves()
{
	TArray<FRichCurveEditInfo> result = TArray<FRichCurveEditInfo>();
	UChromaSDKPluginAnimation2DObject* animation = GetAnimation();
	if (animation != nullptr)
	{
		result.Add(&animation->Curve.EditorCurveData);
	}
	return result;
}

/** Called to modify the owner of the curve */
void FChromaSDKEditorAnimation2DDetails::ModifyOwner()
{

}

/** Returns the owner(s) of the curve */
TArray<const UObject*> FChromaSDKEditorAnimation2DDetails::GetOwners() const
{
	TArray<const UObject*> result = TArray<const UObject*>();
	if (_mObjectsBeingCustomized.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)_mObjectsBeingCustomized[0].Get();
		if (animation != nullptr)
		{
			result.Add(animation);
		}
	}
	return result;
}

/** Called to make curve owner transactional */
void FChromaSDKEditorAnimation2DDetails::MakeTransactional()
{

}

/** Called when any of the curves have been changed */
void FChromaSDKEditorAnimation2DDetails::OnCurveChanged(const TArray<FRichCurveEditInfo>& ChangedCurveEditInfos)
{

}

/** Validates that a previously retrieved curve is still valid for editing. */
bool FChromaSDKEditorAnimation2DDetails::IsValidCurve(FRichCurveEditInfo CurveInfo)
{
	return true;
}

/* end of FCurveOwnerInterface */

#undef LOCTEXT_NAMESPACE

#include "HideWindowsPlatformTypes.h"

#endif
