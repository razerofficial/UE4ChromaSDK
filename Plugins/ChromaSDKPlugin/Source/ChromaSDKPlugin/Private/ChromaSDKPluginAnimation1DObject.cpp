// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DObject.h"
#include "ChromaSDKPluginBPLibrary.h"
#include "LogMacros.h"

UChromaSDKPluginAnimation1DObject::UChromaSDKPluginAnimation1DObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// start with 1 frame
	FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
	frame.Colors = UChromaSDKPluginBPLibrary::CreateColors1D(Device);
	Frames.Add(frame);

	Curve.EditorCurveData.AddKey(1.0f, 0.0f);

	_mIsPlaying = false;
	_mTime = 0.0f;
	_mCurrentFrame = 0;
}

void UChromaSDKPluginAnimation1DObject::Tick(float deltaTime)
{
	_mTime += deltaTime;
	float nextTime = GetTime(_mCurrentFrame);
	if (nextTime < _mTime)
	{
		++_mCurrentFrame;
		if (_mCurrentFrame < _mEffects.Num())
		{
			//UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::Tick SetEffect."));
			FChromaSDKEffectResult& effect = _mEffects[_mCurrentFrame];
			int result = UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(effect.EffectId);
			if (result != 0)
			{
				UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Tick Failed to set effect!"));
			}
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::Tick Animation Complete."));
			_mIsPlaying = false;
			_mTime = 0.0f;
			_mCurrentFrame = 0;
			_mOnComplete.ExecuteIfBound();
			_mOnComplete.Clear();
		}
	}
}

bool UChromaSDKPluginAnimation1DObject::IsTickable() const
{
	return _mIsPlaying;
}

bool UChromaSDKPluginAnimation1DObject::IsTickableInEditor() const
{
	return true;
}

bool UChromaSDKPluginAnimation1DObject::IsTickableWhenPaused() const
{
	return false;
}

TStatId UChromaSDKPluginAnimation1DObject::GetStatId() const
{
	return TStatId();
}

float UChromaSDKPluginAnimation1DObject::GetTime(int index)
{
	if (index >= 0 &&
		index < Curve.EditorCurveData.Keys.Num())
	{
		return Curve.EditorCurveData.Keys[index].Time;
	}
	return 0.033f;
}

void UChromaSDKPluginAnimation1DObject::Load()
{
	if (_mIsLoaded)
	{
		UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Load Animation has already been loaded!"));
		return;
	}

	for (int i = 0; i < Frames.Num(); ++i)
	{
		FChromaSDKColorFrame1D& frame = Frames[i];
		FChromaSDKEffectResult effect = UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom1D(Device, frame.Colors);
		if (effect.Result != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Load Failed to create effect!"));
		}
		_mEffects.Add(effect);
	}

	_mIsLoaded = true;
}

bool UChromaSDKPluginAnimation1DObject::IsLoaded()
{
	return _mIsLoaded;
}

void UChromaSDKPluginAnimation1DObject::Unload()
{
	if (!_mIsLoaded)
	{
		UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Unload Animation has already been unloaded!"));
		return;
	}

	for (int i = 0; i < _mEffects.Num(); ++i)
	{
		FChromaSDKEffectResult& effect = _mEffects[i];
		int result = UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(effect.EffectId);
		if (result != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Unload Failed to delete effect!"));
		}
	}
	_mEffects.Reset();
	_mIsLoaded = false;
}

void UChromaSDKPluginAnimation1DObject::Play()
{
	//UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::Play"));

	if (!_mIsLoaded)
	{
		UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Play Animation has not been loaded!"));
		return;
	}

	_mTime = 0.0f;
	_mIsPlaying = true;
	_mCurrentFrame = 0;

	if (_mCurrentFrame < _mEffects.Num())
	{
		//UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::Tick SetEffect."));
		FChromaSDKEffectResult& effect = _mEffects[_mCurrentFrame];
		int result = UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(effect.EffectId);
		if (result != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Tick Failed to set effect!"));
		}
	}
}

void UChromaSDKPluginAnimation1DObject::PlayWithOnComplete(FDelegateChomaSDKOnComplete onComplete)
{
	UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::PlayWithOnComplete"));

	if (!_mIsLoaded)
	{
		UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::PlayWithOnComplete Animation has not been loaded!"));
		return;
	}

	_mOnComplete = onComplete;

	_mTime = 0.0f;
	_mIsPlaying = true;
	_mCurrentFrame = 0;

	if (_mCurrentFrame < _mEffects.Num())
	{
		//UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::Tick SetEffect."));
		FChromaSDKEffectResult& effect = _mEffects[_mCurrentFrame];
		int result = UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(effect.EffectId);
		if (result != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("UChromaSDKPluginAnimation1DObject::Tick Failed to set effect!"));
		}
	}
}

void UChromaSDKPluginAnimation1DObject::Stop()
{
	//UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation1DObject::Stop"));
	_mIsPlaying = false;
	_mTime = 0.0f;
	_mCurrentFrame = 0;
}

bool UChromaSDKPluginAnimation1DObject::IsPlaying()
{
	return _mIsPlaying;
}

TArray<FChromaSDKColorFrame1D>& UChromaSDKPluginAnimation1DObject::GetFrames()
{
	return Frames;
}

#if WITH_EDITOR

void UChromaSDKPluginAnimation1DObject::Reset(EChromaSDKDevice1DEnum device)
{
	// change device
	Device = device;

	// start with 1 frame
	Frames.Reset();
	FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
	frame.Colors = UChromaSDKPluginBPLibrary::CreateColors1D(Device);
	Frames.Add(frame);

	Curve.EditorCurveData.Reset();
	Curve.EditorCurveData.AddKey(1.0f, 0.0f);
}

void UChromaSDKPluginAnimation1DObject::RefreshCurve()
{
	//copy times
	TArray<float> times = TArray<float>();
	for (int i = 0; i < Curve.EditorCurveData.Keys.Num(); ++i)
	{
		FRichCurveKey& key = Curve.EditorCurveData.Keys[i];
		float time = key.Time;
		if (time <= 0.0f)
		{
			time = 0.033f;
		}
		times.Add(time);
	}

	// make sure curve data doesn't have any extra items
	while (times.Num() > Frames.Num())
	{
		int lastIndex = times.Num() - 1;
		times.RemoveAt(lastIndex);
	}

	// make sure curve data has the same number of items as frames
	while (times.Num() < Frames.Num())
	{
		if (times.Num() == 0)
		{
			times.Add(1.0f);
		}
		else
		{
			int lastIndex = times.Num() - 1;
			float time = times[lastIndex] + 1.0f;
			times.Add(time);
		}
	}

	// reset array
	Curve.EditorCurveData.Reset();
	for (int i = 0; i < times.Num(); ++i)
	{
		float time = times[i];
		Curve.EditorCurveData.AddKey(time, 0.0f);
	}
}

void UChromaSDKPluginAnimation1DObject::RefreshColors()
{
	int maxLeds = UChromaSDKPluginBPLibrary::GetMaxLeds(Device);
	for (int i = 0; i < Frames.Num(); ++i)
	{
		FChromaSDKColorFrame1D& frame = Frames[i];
		FChromaSDKColors& colors = frame.Colors;
		if (colors.Colors.Num() != maxLeds)
		{
			frame.Colors = UChromaSDKPluginBPLibrary::CreateColors1D(Device);
		}
	}
}

#endif
