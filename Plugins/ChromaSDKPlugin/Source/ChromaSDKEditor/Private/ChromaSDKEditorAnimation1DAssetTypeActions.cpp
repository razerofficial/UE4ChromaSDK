// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKEditor.h"
#if WITH_EDITOR
#include "ChromaSDKEditorAnimation1DAssetTypeActions.h"
#include "ChromaSDKPluginAnimation1DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FChromaSDKEditorAnimation1DAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ChromaSDKEditorAnimation1D", "ChromaSDK Animation 1D");
}

FColor FChromaSDKEditorAnimation1DAssetTypeActions::GetTypeColor() const
{
	return FColor(149, 70, 255);
}

UClass* FChromaSDKEditorAnimation1DAssetTypeActions::GetSupportedClass() const
{
	return UChromaSDKPluginAnimation1DObject::StaticClass();
}

uint32 FChromaSDKEditorAnimation1DAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

bool FChromaSDKEditorAnimation1DAssetTypeActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FChromaSDKEditorAnimation1DAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Effects = GetTypedWeakObjectPtrs<UChromaSDKPluginAnimation1DObject>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ChromaAnimation1D_PlayEffect", "Play"),
		LOCTEXT("ChromaAnimation1D_PlayEffectTooltip", "Plays the selected Chroma animation."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "MediaAsset.AssetActions.Play.Small"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FChromaSDKEditorAnimation1DAssetTypeActions::ExecutePlayEffect, Effects),
			FCanExecuteAction::CreateSP(this, &FChromaSDKEditorAnimation1DAssetTypeActions::CanExecutePlayCommand, Effects)
		)
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ChromaAnimation1D_StopEffect", "Stop"),
		LOCTEXT("ChromaAnimation1D_StopEffectTooltip", "Stops the selected Chroma animation."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "MediaAsset.AssetActions.Stop.Small"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FChromaSDKEditorAnimation1DAssetTypeActions::ExecuteStopEffect, Effects),
			FCanExecuteAction()
		)
	);
}

void FChromaSDKEditorAnimation1DAssetTypeActions::AssetsActivated(const TArray<UObject*>& InObjects, EAssetTypeActivationMethod::Type ActivationType)
{

}

TSharedPtr<SWidget> FChromaSDKEditorAnimation1DAssetTypeActions::GetThumbnailOverlay(const FAssetData& AssetData) const
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

	FChromaSDKEditorAnimation1DAssetTypeActions* MutableThis = const_cast<FChromaSDKEditorAnimation1DAssetTypeActions*>(this);
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
			return LOCTEXT("Thumbnail_StopChromaAnimationToolTip", "Stop selected Chroma animation");
		}

		return LOCTEXT("Thumbnail_PlayChromaAnimationToolTip", "Play selected Chroma animation");
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
bool FChromaSDKEditorAnimation1DAssetTypeActions::IsEffectPlaying(const TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>>& Objects) const
{
	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)Objects[0].Get();
		if (animation != nullptr)
		{
			return animation->IsPlaying();
		}
	}
	return false;
}

/** Handler for when PlayEffect is selected */
void FChromaSDKEditorAnimation1DAssetTypeActions::ExecutePlayEffect(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Objects)
{
	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)Objects[0].Get();
		if (animation != nullptr)
		{
			if (!animation->IsLoaded())
			{
				animation->Load();
			}
			animation->Play();
		}
	}
}

/** Handler for when StopEffect is selected */
void FChromaSDKEditorAnimation1DAssetTypeActions::ExecuteStopEffect(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Objects)
{
	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)Objects[0].Get();
		if (animation != nullptr)
		{
			return animation->Stop();
		}
	}
}

/** Returns true if only one effect is selected to play */
bool FChromaSDKEditorAnimation1DAssetTypeActions::CanExecutePlayCommand(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation1DObject>> Objects) const
{
	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation1DObject* animation = (UChromaSDKPluginAnimation1DObject*)Objects[0].Get();
		if (animation != nullptr)
		{
			return !animation->IsPlaying();
		}
	}
	return true;
}

#undef LOCTEXT_NAMESPACE

#endif
