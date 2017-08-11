// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKEditor.h"

#if WITH_EDITOR

#include "UnrealEd.h"
#include "Runtime/Launch/Resources/Version.h"
#include "ChromaSDKEditorAnimation1DFactory.generated.h"

UCLASS()
class UChromaSDKEditorAnimation1DFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};

#endif
