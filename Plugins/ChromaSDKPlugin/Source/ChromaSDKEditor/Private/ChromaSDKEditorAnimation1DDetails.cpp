// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#if WITH_EDITOR

#include "ChromaSDKEditorAnimation1DDetails.h"
#include "ChromaSDKEditorButton1D.h"
#include "ChromaSDKPluginAnimation1DObject.h"
#include "ChromaSDKPluginBPLibrary.h"
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

#define LOCTEXT_NAMESPACE "ChromaAnimation1DDetails"

TSharedRef<IDetailCustomization> FChromaSDKEditorAnimation1DDetails::MakeInstance()
{
	TSharedRef<FChromaSDKEditorAnimation1DDetails> instance = MakeShareable(new FChromaSDKEditorAnimation1DDetails);
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

void FChromaSDKEditorAnimation1DDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
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
}

void FChromaSDKEditorAnimation1DDetails::RefreshFrames()
{
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
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickPreviousFrame()
{
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
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickNextFrame()
{
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
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickAddFrame()
{
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
	return FReply::Handled();
}
FReply FChromaSDKEditorAnimation1DDetails::OnClickDeleteFrame()
{
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
	return FReply::Handled();
}

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

FReply FChromaSDKEditorAnimation1DDetails::OnClickImportTextureImageButton()
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickImportTextureAnimationButton()
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickOverrideButton()
{
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
	return FReply::Handled();
}

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

void FChromaSDKEditorAnimation1DDetails::RefreshDevice()
{
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
}

void FChromaSDKEditorAnimation1DDetails::OnClickColor(int element)
{
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
}

void FChromaSDKEditorAnimation1DDetails::OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
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
}

void FChromaSDKEditorAnimation1DDetails::OnColorCommitted(FLinearColor color)
{
	//UE_LOG(LogTemp, Log, TEXT("FChromaSDKEditorAnimation1DDetails::OnColorCommitted"));
	color.A = 1.0f; //full alpha
	_mColor = color;
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickClearButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickFillButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickCopyButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickPasteButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickRandomButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickSetDeviceButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickPreviewButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickPlayButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickStopButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickLoadButton()
{
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

	return FReply::Handled();
}

FReply FChromaSDKEditorAnimation1DDetails::OnClickUnloadButton()
{
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

#endif
