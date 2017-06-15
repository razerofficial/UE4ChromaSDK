// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DObjectFactory.h"

#define LOCTEXT_NAMESPACE "ChromaSDKPluginAnimation2DObject"

UChromaSDKPluginAnimation2DObjectFactory::UChromaSDKPluginAnimation2DObjectFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UChromaSDKPluginAnimation2DObject::StaticClass();
}

UObject* UChromaSDKPluginAnimation2DObjectFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UChromaSDKPluginAnimation2DObject* NewObjectAsset = ConstructObject<UChromaSDKPluginAnimation2DObject>(Class, InParent, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE
