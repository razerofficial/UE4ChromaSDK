// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPluginTypes.h"
#include "ChromaSDKPluginAnimation1DObject.generated.h"

UCLASS(Blueprintable, Category="Lighting")
class UChromaSDKPluginAnimation1DObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere)
	EChromaSDKDevice1DEnum Device;

	UPROPERTY(EditAnywhere)
	TArray<FChromaSDKColorFrame1D> Frames;
};
