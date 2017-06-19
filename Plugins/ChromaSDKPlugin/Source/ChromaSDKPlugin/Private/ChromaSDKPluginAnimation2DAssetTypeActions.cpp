// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation2DAssetTypeActions.h"
#include "ChromaSDKPluginAnimation2DObject.h"

FText FChromaSDKPluginAnimation2DAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ChromaSDKPluginAnimation2D", "ChromaSDK Animation 2D");
}

FColor FChromaSDKPluginAnimation2DAssetTypeActions::GetTypeColor() const
{
	return FColor(149, 70, 255);
}

UClass* FChromaSDKPluginAnimation2DAssetTypeActions::GetSupportedClass() const
{
	return UChromaSDKPluginAnimation2DObject::StaticClass();
}

uint32 FChromaSDKPluginAnimation2DAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}
