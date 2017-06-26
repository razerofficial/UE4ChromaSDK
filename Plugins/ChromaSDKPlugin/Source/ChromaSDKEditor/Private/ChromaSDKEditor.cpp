// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKEditor.h"
#if WITH_EDITOR
#include "ChromaSDKEditorAnimation1DAssetTypeActions.h"
#include "ChromaSDKEditorAnimation1DDetails.h"
#include "ChromaSDKEditorAnimation2DAssetTypeActions.h"
#include "ChromaSDKEditorAnimation2DDetails.h"
#include "PropertyEditorModule.h"
#endif //WITH_EDITOR

#define LOCTEXT_NAMESPACE "FChromaSDKEditorModule"

void FChromaSDKEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
	// register the asset type

	// Register asset types
	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	assetTools.RegisterAssetTypeActions(MakeShareable(new FChromaSDKEditorAnimation1DAssetTypeActions));
	assetTools.RegisterAssetTypeActions(MakeShareable(new FChromaSDKEditorAnimation2DAssetTypeActions));
	// end of register the asset type

	// Register custom views
	FPropertyEditorModule& propertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	propertyModule.RegisterCustomClassLayout("ChromaSDKPluginAnimation1DObject", FOnGetDetailCustomizationInstance::CreateStatic(&FChromaSDKEditorAnimation1DDetails::MakeInstance));
	propertyModule.RegisterCustomClassLayout("ChromaSDKPluginAnimation2DObject", FOnGetDetailCustomizationInstance::CreateStatic(&FChromaSDKEditorAnimation2DDetails::MakeInstance));
	// end of register custom views
#endif
}

void FChromaSDKEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChromaSDKEditorModule, ChromaSDKEditor)
