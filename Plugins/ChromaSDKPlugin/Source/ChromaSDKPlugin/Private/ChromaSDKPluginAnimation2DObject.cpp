// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DObject.h"
#include "ChromaSDKPluginBPLibrary.h"

UChromaSDKPluginAnimation2DObject::UChromaSDKPluginAnimation2DObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR
void UChromaSDKPluginAnimation2DObject::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	/*
	UE_LOG(LogTemp, Log, TEXT("UChromaSDKPluginAnimation2DObject::PostEditChangeChainProperty %s"),
		*PropertyChangedEvent.Property->GetName());
	*/

	if (PropertyChangedEvent.Property->GetName() == "Curve")
	{
		for (int i = 1; i < Curve.EditorCurveData.Keys.Num() && i < Frames.Num(); ++i)
		{
			FRichCurveKey& previousKey = Curve.EditorCurveData.Keys[i-1];
			FRichCurveKey& key = Curve.EditorCurveData.Keys[i];
			key.Value = 1.0f;
			
			float t = key.Time - previousKey.Time;
			Frames[i-1].Duration = t;
		}
	}

	int maxRow = UChromaSDKPluginBPLibrary::GetMaxRow(Device);
	int maxColumn = UChromaSDKPluginBPLibrary::GetMaxColumn(Device);
	for (int i = 0; i < Frames.Num(); ++i)
	{
		FChromaSDKColorFrame2D& frame = Frames[i];
		if (frame.Duration <= 0.0f)
		{
			frame.Duration = 1.0f;
		}
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
