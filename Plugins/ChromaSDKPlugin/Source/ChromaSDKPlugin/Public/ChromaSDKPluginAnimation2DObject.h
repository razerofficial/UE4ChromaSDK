// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPluginTypes.h"
#include "ChromaSDKPluginAnimation2DObject.generated.h"

UCLASS(Blueprintable, Category = "Animation")
class UChromaSDKPluginAnimation2DObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Animation")
	EChromaSDKDevice2DEnum Device;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<FChromaSDKColorFrame2D> Frames;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FRuntimeFloatCurve Curve;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};
