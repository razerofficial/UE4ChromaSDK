// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPluginTypes.h"
#include "ChromaSDKPluginAnimation1DObject.generated.h"

UCLASS(Blueprintable, Category="Animation")
class UChromaSDKPluginAnimation1DObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Animation")
	EChromaSDKDevice1DEnum Device;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<FChromaSDKColorFrame1D> Frames;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FRuntimeFloatCurve Curve;
};
