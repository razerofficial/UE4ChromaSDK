﻿// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DAssetTypeActions.h"
#include "ChromaSDKPluginAnimation1DObject.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FChromaSDKPluginAnimation1DAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ChromaSDKPluginAnimation1D", "ChromaSDK Animation 1D");
}

FColor FChromaSDKPluginAnimation1DAssetTypeActions::GetTypeColor() const
{
	return FColor(149, 70, 255);
}

UClass* FChromaSDKPluginAnimation1DAssetTypeActions::GetSupportedClass() const
{
	return UChromaSDKPluginAnimation1DObject::StaticClass();
}

uint32 FChromaSDKPluginAnimation1DAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

// want the preview thumbnail from ForceFeedbackEffect

bool FChromaSDKPluginAnimation1DAssetTypeActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FChromaSDKPluginAnimation1DAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Effects = GetTypedWeakObjectPtrs<UChromaSDKPluginAnimation1DObject>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ForceFeedbackEffect_PlayEffect", "Play"),
		LOCTEXT("ForceFeedbackEffect_PlayEffectTooltip", "Plays the selected force feedback effect."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "MediaAsset.AssetActions.Play.Small"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FChromaSDKPluginAnimation1DAssetTypeActions::ExecutePlayEffect, Effects),
			FCanExecuteAction::CreateSP(this, &FChromaSDKPluginAnimation1DAssetTypeActions::CanExecutePlayCommand, Effects)
		)
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ForceFeedbackEffect_StopEffect", "Stop"),
		LOCTEXT("ForceFeedbackEffect_StopEffectTooltip", "Stops the selected force feedback effect."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "MediaAsset.AssetActions.Stop.Small"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FChromaSDKPluginAnimation1DAssetTypeActions::ExecuteStopEffect, Effects),
			FCanExecuteAction()
		)
	);
}

void FChromaSDKPluginAnimation1DAssetTypeActions::AssetsActivated(const TArray<UObject*>& InObjects, EAssetTypeActivationMethod::Type ActivationType)
{

}

TSharedPtr<SWidget> FChromaSDKPluginAnimation1DAssetTypeActions::GetThumbnailOverlay(const FAssetData& AssetData) const
{
	TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> EffectList;
	EffectList.Add(TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>((UChromaSDKPluginAnimation1DObject*)AssetData.GetAsset()));

	auto OnGetDisplayBrushLambda = [this, EffectList]() -> const FSlateBrush*
	{
		if (IsEffectPlaying(EffectList))
		{
			return FEditorStyle::GetBrush("MediaAsset.AssetActions.Stop.Large");
		}

	return FEditorStyle::GetBrush("MediaAsset.AssetActions.Play.Large");
	};

	auto IsEnabledLambda = [this, EffectList]() -> bool
	{
		return CanExecutePlayCommand(EffectList);
	};

	FChromaSDKPluginAnimation1DAssetTypeActions* MutableThis = const_cast<FChromaSDKPluginAnimation1DAssetTypeActions*>(this);
	auto OnClickedLambda = [MutableThis, EffectList]() -> FReply
	{
		if (MutableThis->IsEffectPlaying(EffectList))
		{
			MutableThis->ExecuteStopEffect(EffectList);
		}
		else
		{
			MutableThis->ExecutePlayEffect(EffectList);
		}
		return FReply::Handled();
	};

	auto OnToolTipTextLambda = [this, EffectList]() -> FText
	{
		if (IsEffectPlaying(EffectList))
		{
			return LOCTEXT("Thumbnail_StopForceFeedbackToolTip", "Stop selected force feedback effect");
		}

		return LOCTEXT("Thumbnail_PlayForceFeedbackToolTip", "Play selected force feedback effect");
	};

	TSharedRef<SBox> Box = SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(FMargin(2));

	auto OnGetVisibilityLambda = [this, Box, EffectList]() -> EVisibility
	{
		if (Box->IsHovered() || IsEffectPlaying(EffectList))
		{
			return EVisibility::Visible;
		}

		return EVisibility::Hidden;
	};

	TSharedRef<SButton> BoxContent = SNew(SButton)
		.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
		.ToolTipText_Lambda(OnToolTipTextLambda)
		.Cursor(EMouseCursor::Default) // The outer widget can specify a DragHand cursor, so we need to override that here
		.ForegroundColor(FSlateColor::UseForeground())
		.IsFocusable(false)
		.IsEnabled_Lambda(IsEnabledLambda)
		.OnClicked_Lambda(OnClickedLambda)
		.Visibility_Lambda(OnGetVisibilityLambda)
		[
			SNew(SBox)
			.MinDesiredWidth(16)
		.MinDesiredHeight(16)
		[
			SNew(SImage)
			.Image_Lambda(OnGetDisplayBrushLambda)
		]
		];

	Box->SetContent(BoxContent);
	Box->SetVisibility(EVisibility::Visible);

	return Box;
}

/** Return if the specified effect is playing*/
bool FChromaSDKPluginAnimation1DAssetTypeActions::IsEffectPlaying(const TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>>& Objects) const
{
	return false;
}

/** Handler for when PlayEffect is selected */
void FChromaSDKPluginAnimation1DAssetTypeActions::ExecutePlayEffect(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Objects)
{

}

/** Handler for when StopEffect is selected */
void FChromaSDKPluginAnimation1DAssetTypeActions::ExecuteStopEffect(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Objects)
{

}

/** Returns true if only one effect is selected to play */
bool FChromaSDKPluginAnimation1DAssetTypeActions::CanExecutePlayCommand(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Objects) const
{
	return true;
}