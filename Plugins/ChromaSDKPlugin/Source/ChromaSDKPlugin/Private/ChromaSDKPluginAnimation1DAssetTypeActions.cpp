// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginAnimation1DAssetTypeActions.h"
#include "ChromaSDKPluginAnimation1DObject.h"

FText FChromaSDKPluginAnimation1DAssetTypeActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ChromaSDKPluginAnimation1D", "ChromaSDK Animation 1D");
}

FColor FChromaSDKPluginAnimation1DAssetTypeActions::GetTypeColor() const
{
	return FColor(149, 70, 255);
}

UClass* FChromaSDKPluginAnimation1DAssetTypeActions::GetSupportedClass() const
{
	return UChromaSDKPluginAnimation1DObject::StaticClass();
}

uint32 FChromaSDKPluginAnimation1DAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}
