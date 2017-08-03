// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKEditor.h"
#if WITH_EDITOR
#include "ChromaSDKEditorAnimation2DAssetTypeActions.h"
#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

FText FChromaSDKEditorAnimation2DAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ChromaSDKEditorAnimation2D", "ChromaSDK Animation 2D");
}

FColor FChromaSDKEditorAnimation2DAssetTypeActions::GetTypeColor() const
{
	return FColor(149, 70, 255);
}

UClass* FChromaSDKEditorAnimation2DAssetTypeActions::GetSupportedClass() const
{
	return UChromaSDKPluginAnimation2DObject::StaticClass();
}

uint32 FChromaSDKEditorAnimation2DAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

bool FChromaSDKEditorAnimation2DAssetTypeActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FChromaSDKEditorAnimation2DAssetTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	TArray<TWeakObjectPtr<UChromaSDKPluginAnimation2DObject>> Effects = GetTypedWeakObjectPtrs<UChromaSDKPluginAnimation2DObject>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ChromaAnimation2D_PlayEffect", "Play"),
		LOCTEXT("ChromaAnimation2D_PlayEffectTooltip", "Plays the selected Chroma animation."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "MediaAsset.AssetActions.Play.Small"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FChromaSDKEditorAnimation2DAssetTypeActions::ExecutePlayEffect, Effects),
			FCanExecuteAction::CreateSP(this, &FChromaSDKEditorAnimation2DAssetTypeActions::CanExecutePlayCommand, Effects)
		)
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("ChromaAnimation2D_StopEffect", "Stop"),
		LOCTEXT("ChromaAnimation2D_StopEffectTooltip", "Stops the selected Chroma animation."),
		FSlateIcon(FEditorStyle::GetStyleSetName(), "MediaAsset.AssetActions.Stop.Small"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FChromaSDKEditorAnimation2DAssetTypeActions::ExecuteStopEffect, Effects),
			FCanExecuteAction()
		)
	);
}

void FChromaSDKEditorAnimation2DAssetTypeActions::AssetsActivated(const TArray<UObject*>& InObjects, EAssetTypeActivationMethod::Type ActivationType)
{

}

TSharedPtr<SWidget> FChromaSDKEditorAnimation2DAssetTypeActions::GetThumbnailOverlay(const FAssetData& AssetData) const
{
	TArray<TWeakObjectPtr<UChromaSDKPluginAnimation2DObject>> EffectList;
	EffectList.Add(TWeakObjectPtr<UChromaSDKPluginAnimation2DObject>((UChromaSDKPluginAnimation2DObject*)AssetData.GetAsset()));

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

	FChromaSDKEditorAnimation2DAssetTypeActions* MutableThis = const_cast<FChromaSDKEditorAnimation2DAssetTypeActions*>(this);
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
bool FChromaSDKEditorAnimation2DAssetTypeActions::IsEffectPlaying(const TArray<TWeakObjectPtr<UChromaSDKPluginAnimation2DObject>>& Objects) const
{
	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)Objects[0].Get();
		if (animation != nullptr)
		{
			return animation->IsPlaying();
		}
	}
	return false;
}

/** Handler for when PlayEffect is selected */
void FChromaSDKEditorAnimation2DAssetTypeActions::ExecutePlayEffect(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation2DObject>> Objects)
{
	bool initialized = UChromaSDKPluginBPLibrary::IsInitialized();
	if (!initialized)
	{
		UChromaSDKPluginBPLibrary::ChromaSDKInit();
	}

	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)Objects[0].Get();
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
void FChromaSDKEditorAnimation2DAssetTypeActions::ExecuteStopEffect(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation2DObject>> Objects)
{
	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)Objects[0].Get();
		if (animation != nullptr)
		{
			return animation->Stop();
		}
	}
}

/** Returns true if only one effect is selected to play */
bool FChromaSDKEditorAnimation2DAssetTypeActions::CanExecutePlayCommand(TArray<TWeakObjectPtr<UChromaSDKPluginAnimation2DObject>> Objects) const
{
	if (Objects.Num() > 0)
	{
		UChromaSDKPluginAnimation2DObject* animation = (UChromaSDKPluginAnimation2DObject*)Objects[0].Get();
		if (animation != nullptr)
		{
			return !animation->IsPlaying();
		}
	}
	return true;
}

#undef LOCTEXT_NAMESPACE

#endif
