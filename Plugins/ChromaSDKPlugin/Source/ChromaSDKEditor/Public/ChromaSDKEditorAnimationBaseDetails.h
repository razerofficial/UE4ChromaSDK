// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKEditor.h"

#if WITH_EDITOR

#include "UnrealEd.h"
#include "Runtime/Launch/Resources/Version.h"
#include "AllowWindowsPlatformTypes.h" 
#include "IDetailCustomization.h"
#include "ChromaSDKPluginTypes.h"
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 15)
#include "Curves/CurveOwnerInterface.h"
#endif
#include "DetailCategoryBuilder.h"
#include "Widgets/Layout/SGridPanel.h"

class SCurveEditor;
struct FRuntimeFloatCurve;

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 15)
class IChromaSDKEditorAnimationBaseDetails : public IDetailCustomization, public FCurveOwnerInterface
#else
class IChromaSDKEditorAnimationBaseDetails : public IDetailCustomization
#endif
{
protected:

	void Initialize();

	const void* GetParentWindowWindowHandle();

	void ReadImage(const FString& path, bool isAnimation);

#if PLATFORM_WINDOWS
	virtual void ReadChromaFile(const FString& path) = 0;
	virtual void CopyPixels(COLORREF* pColor, UINT width, UINT height) = 0;
	virtual void WriteChromaFile(const FString& path) = 0;
#endif

	void AddChromaSDKDevice(FString& text);

	// enum dropdown handler
	TSharedRef<SWidget> GenerateDropdownEnum(TSharedPtr<FString> InItem);

	void BuildImportRow(IDetailLayoutBuilder& DetailBuilder);

	void BuildDeviceRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category);

	void BuildApplyRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category);

	void BuildInfoRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category);

	void BuildPreviewRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category);

	void BuildColorRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category);

	void BuildFramesRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category);

	void BuildCurveRow(IDetailLayoutBuilder& DetailBuilder, IDetailCategoryBuilder& category);

	// import chroma animation
	virtual FReply OnClickImportButton() = 0;

	// export chroma animation
	virtual FReply OnClickExportButton() = 0;

	// import colors from texture image
	virtual FReply OnClickImportTextureImageButton() = 0;

	// import colors from a texture animation
	virtual FReply OnClickImportTextureAnimationButton() = 0;

	// reset animation length to override
	virtual FReply OnClickOverrideButton() = 0;

	// device row
	virtual void OnChangeChromaSDKDevices(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo) = 0;
	virtual FReply OnClickSetDeviceButton() = 0;

	// apply row
	virtual FReply OnClickClearButton() = 0;
	virtual FReply OnClickFillButton() = 0;
	virtual FReply OnClickRandomButton() = 0;
	virtual FReply OnClickCopyButton() = 0;
	virtual FReply OnClickPasteButton() = 0;
	virtual FReply OnClickPreviewButton() = 0;
	virtual FReply OnClickPlayButton() = 0;
	virtual FReply OnClickStopButton() = 0;
	virtual FReply OnClickLoadButton() = 0;
	virtual FReply OnClickUnloadButton() = 0;

	// device preview row
	virtual void RefreshDevice() = 0;

	// set the color row
	virtual void OnColorCommitted(FLinearColor color) = 0;

	// animation frames row
	virtual FReply OnClickFirstFrame() = 0;
	virtual FReply OnClickPreviousFrame() = 0;
	virtual FReply OnClickNextFrame() = 0;
	virtual FReply OnClickLastFrame() = 0;
	virtual FReply OnClickAddFrame() = 0;
	virtual FReply OnClickDeleteFrame() = 0;
	virtual FReply OnClickResetFrame() = 0;
	virtual void RefreshFrames() = 0;

	// object being inspected
	TArray<TWeakObjectPtr<UObject>> _mObjectsBeingCustomized;

	// the device enum dropdown text
	TArray<TSharedPtr<FString>> _mChromaSDKDevices;

	// grid info
	TSharedPtr<STextBlock> _mTextInfo;

	// device grid
	TSharedPtr<SGridPanel> _mGrid;

	// current color in the color picker
	FLinearColor _mColor;

	// animation controls
	int _mCurrentFrame;
	TSharedPtr<STextBlock> _mTextCurrentFrame;
	TSharedPtr<STextBlock> _mTextNumberOfFrames;
	TSharedPtr<STextBlock> _mTextFrameDuration;

	// custom curve
	TSharedPtr<SCurveEditor> _mCurveWidget;
};

#include "HideWindowsPlatformTypes.h"

#endif
