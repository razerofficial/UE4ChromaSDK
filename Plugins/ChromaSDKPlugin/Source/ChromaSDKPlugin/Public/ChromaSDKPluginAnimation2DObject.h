// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPluginTypes.h"
#include "ChromaSDKPluginAnimation2DObject.generated.h"

UCLASS(Blueprintable, Category = "Lighting")
class UChromaSDKPluginAnimation2DObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere)
	EChromaSDKDevice2DEnum Device;

	UPROPERTY(EditAnywhere)
	TArray<FChromaSDKColorFrame2D> Frames;
};
