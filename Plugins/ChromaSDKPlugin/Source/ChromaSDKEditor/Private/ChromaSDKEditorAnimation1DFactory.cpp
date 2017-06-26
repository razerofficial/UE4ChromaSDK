// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#if WITH_EDITOR

#include "ChromaSDKEditorAnimation1DFactory.h"
#include "ChromaSDKPluginAnimation1DObject.h"

#define LOCTEXT_NAMESPACE "ChromaSDKPluginAnimation1DObject"

UChromaSDKEditorAnimation1DFactory::UChromaSDKEditorAnimation1DFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UChromaSDKPluginAnimation1DObject::StaticClass();
}

UObject* UChromaSDKEditorAnimation1DFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UChromaSDKPluginAnimation1DObject* NewObjectAsset = ConstructObject<UChromaSDKPluginAnimation1DObject>(Class, InParent, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE

#endif
