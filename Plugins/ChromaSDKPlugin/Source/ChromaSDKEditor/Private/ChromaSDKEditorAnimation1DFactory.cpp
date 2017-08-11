// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//#include "ChromaSDKEditor.h" //(support 4.15 or below)___HACK_UE4_WANTS_MODULE_FIRST
#include "ChromaSDKEditorAnimation1DFactory.h"
#include "ChromaSDKEditor.h" //(support 4.16 or above)___HACK_UE4_WANTS_HEADER_FIRST

#if WITH_EDITOR
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
	//UChromaSDKPluginAnimation1DObject* NewObjectAsset = ConstructObject<UChromaSDKPluginAnimation1DObject>(Class, InParent, Name, Flags | RF_Transactional);
	UChromaSDKPluginAnimation1DObject* NewObjectAsset = NewObject<UChromaSDKPluginAnimation1DObject>(InParent, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE

#endif
