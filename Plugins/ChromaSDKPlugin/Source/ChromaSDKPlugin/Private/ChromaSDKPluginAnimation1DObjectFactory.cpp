// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DObjectFactory.h"

#define LOCTEXT_NAMESPACE "ChromaSDKPluginAnimation1DObject"

UChromaSDKPluginAnimation1DObjectFactory::UChromaSDKPluginAnimation1DObjectFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UChromaSDKPluginAnimation1DObject::StaticClass();
}

UObject* UChromaSDKPluginAnimation1DObjectFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UChromaSDKPluginAnimation1DObject* NewObjectAsset = ConstructObject<UChromaSDKPluginAnimation1DObject>(Class, InParent, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE
