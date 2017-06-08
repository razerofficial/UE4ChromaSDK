// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPlugin.h"

#define LOCTEXT_NAMESPACE "FChromaSDKPluginModule"

#if PLATFORM_WINDOWS

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

#endif

void FChromaSDKPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if PLATFORM_WINDOWS
	_mLibraryChroma = LoadLibrary(CHROMASDKDLL);
	if (_mLibraryChroma == NULL)
	{
		UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin failed to load!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin loaded."));
#endif
}

void FChromaSDKPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
#if PLATFORM_WINDOWS
	if (_mLibraryChroma)
	{
		FreeLibrary(_mLibraryChroma);
		_mLibraryChroma = nullptr;
	}

	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin unloaded."));
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChromaSDKPluginModule, ChromaSDKPlugin)