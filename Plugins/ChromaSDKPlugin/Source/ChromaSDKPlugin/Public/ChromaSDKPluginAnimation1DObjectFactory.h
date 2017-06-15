// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UnrealEd.h"
#include "ChromaSDKPluginAnimation1DObjectFactory.generated.h"

UCLASS()
class UChromaSDKPluginAnimation1DObjectFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
