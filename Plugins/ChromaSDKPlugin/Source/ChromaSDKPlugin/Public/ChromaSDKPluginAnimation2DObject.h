// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPluginTypes.h"
#include "ChromaSDKPluginAnimation2DObject.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FDelegateChomaSDKOnComplete2D, UChromaSDKPluginAnimation2DObject*, animation);

UCLASS(Blueprintable, BlueprintType, Category = "Animation")
class CHROMASDKPLUGIN_API UChromaSDKPluginAnimation2DObject : public UObject, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Import")
	float OverrideFrameTime;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FRuntimeFloatCurve Curve;

	UPROPERTY(EditAnywhere, Category = "Data")
	EChromaSDKDevice2DEnum Device;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FChromaSDKColorFrame2D> Frames;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Play", Keywords = "Play the animation"), Category = "ChromaSDK")
	void Play();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PlayWithOnComplete", Keywords = "Play the animation and fire the OnComplete event"), Category = "ChromaSDK")
	void PlayWithOnComplete(FDelegateChomaSDKOnComplete2D onComplete);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stop", Keywords = "Stop the animation"), Category = "ChromaSDK")
	void Stop();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsPlaying", Keywords = "Is the animation currently playing?"), Category = "ChromaSDK")
	bool IsPlaying();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Load", Keywords = "Load the effects before playing"), Category = "ChromaSDK")
	void Load();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsLoaded", Keywords = "Check if the animation has loaded"), Category = "ChromaSDK")
	bool IsLoaded();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Unload", Keywords = "Unload the effects"), Category = "ChromaSDK")
	void Unload();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetFrames2D", Keywords = "Get two-dimensional animation frames"), Category = "ChromaSDK")
	TArray<FChromaSDKColorFrame2D>& GetFrames();

	/* Implements FTickableGameObject */
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	/* Implements FTickableGameObject */

#if WITH_EDITOR
	void Reset(EChromaSDKDevice2DEnum device);
	void RefreshColors();
	void RefreshCurve();
#endif

private:
	float GetTime(int index);

	// Callback when animation completes
	FDelegateChomaSDKOnComplete2D _mOnComplete;

	bool _mCompleted;

	// Effects needs to be loaded before the animation can be played
	bool _mIsLoaded;

	bool _mIsPlaying;
	float _mTime;
	int _mCurrentFrame;
	TArray<FChromaSDKEffectResult> _mEffects;
};
