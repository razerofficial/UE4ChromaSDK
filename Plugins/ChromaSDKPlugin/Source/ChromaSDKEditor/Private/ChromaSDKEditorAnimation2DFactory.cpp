// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#if WITH_EDITOR

#include "ChromaSDKEditorAnimation2DFactory.h"
#include "ChromaSDKPluginAnimation2DObject.h"

#define LOCTEXT_NAMESPACE "ChromaSDKPluginAnimation2DObject"

UChromaSDKEditorAnimation2DFactory::UChromaSDKEditorAnimation2DFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UChromaSDKPluginAnimation2DObject::StaticClass();
}

UObject* UChromaSDKEditorAnimation2DFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UChromaSDKPluginAnimation2DObject* NewObjectAsset = ConstructObject<UChromaSDKPluginAnimation2DObject>(Class, InParent, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE

#endif
