// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

UChromaSDKPluginAnimation2DObject::UChromaSDKPluginAnimation2DObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// start with 1 frame
	FChromaSDKColorFrame2D frame = FChromaSDKColorFrame2D();
	frame.Colors = UChromaSDKPluginBPLibrary::CreateColors2D(Device);
	Frames.Add(frame);

	Curve.EditorCurveData.AddKey(1.0f, 0.0f);
}

#if WITH_EDITOR

void UChromaSDKPluginAnimation2DObject::Reset(EChromaSDKDevice2DEnum device)
{
	// change device
	Device = device;

	// start with 1 frame
	Frames.Reset();
	FChromaSDKColorFrame2D frame = FChromaSDKColorFrame2D();
	frame.Colors = UChromaSDKPluginBPLibrary::CreateColors2D(Device);
	Frames.Add(frame);

	Curve.EditorCurveData.Reset();
	Curve.EditorCurveData.AddKey(1.0f, 0.0f);
}

void UChromaSDKPluginAnimation2DObject::RefreshCurve()
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

void UChromaSDKPluginAnimation2DObject::RefreshColors()
{
	int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(Device);
	int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(Device);
	for (int i = 0; i < Frames.Num(); ++i)
	{
		FChromaSDKColorFrame2D& frame = Frames[i];
		TArray<FChromaSDKColors>& rows = frame.Colors;
		if (rows.Num() != maxRow)
		{
			frame.Colors = UChromaSDKPluginBPLibrary::CreateColors2D(Device);
		}
		else
		{
			for (int j = 0; j < maxRow; ++j)
			{
				FChromaSDKColors& col = rows[j];
				if (col.Colors.Num() != maxColumn)
				{
					frame.Colors = UChromaSDKPluginBPLibrary::CreateColors2D(Device);
					break;
				}
			}
		}
	}
}

#endif
