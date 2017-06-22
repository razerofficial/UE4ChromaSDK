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
	FRuntimeFloatCurve Curve;

	UPROPERTY(EditAnywhere, Category = "Data")
	EChromaSDKDevice2DEnum Device;

	UPROPERTY(EditAnywhere, Category = "Data")
	TArray<FChromaSDKColorFrame2D> Frames;

#if WITH_EDITOR
	void Reset(EChromaSDKDevice2DEnum device);
	void RefreshColors();
	void RefreshCurve();

	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};
