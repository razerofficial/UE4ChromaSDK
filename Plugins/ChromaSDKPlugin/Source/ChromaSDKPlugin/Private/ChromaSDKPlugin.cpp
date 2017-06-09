// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPlugin.h"

#define LOCTEXT_NAMESPACE "FChromaSDKPluginModule"

#if PLATFORM_WINDOWS

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Mousepad;
using namespace ChromaSDK::Headset;

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

	// GetProcAddress will throw 4191 because it's an unsafe type cast
#pragma warning(disable: 4191)
	_mMethodInit = (CHROMA_SDK_INIT)GetProcAddress(_mLibraryChroma, "Init");
	if (_mMethodInit == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin failed to find Init method!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin found Init method."));
	}
	_mMethodQueryDevice = (CHROMA_SDK_QUERYDEVICE)GetProcAddress(_mLibraryChroma, "QueryDevice");
	_mMethodUnInit = (CHROMA_SDK_UNINIT)GetProcAddress(_mLibraryChroma, "UnInit");

	_mMethodCreateKeyboardEffect = (CHROMA_SDK_CREATEKEYBOARDEFFECT)GetProcAddress(_mLibraryChroma, "CreateKeyboardEffect");
	_mMethodCreateMouseEffect = (CHROMA_SDK_CREATEMOUSEEFFECT)GetProcAddress(_mLibraryChroma, "CreateMouseEffect");
	_mMethodCreateHeadsetEffect = (CHROMA_SDK_CREATEHEADSETEFFECT)GetProcAddress(_mLibraryChroma, "CreateHeadsetEffect");
	_mMethodCreateMousepadEffect = (CHROMA_SDK_CREATEMOUSEPADEFFECT)GetProcAddress(_mLibraryChroma, "CreateMousepadEffect");
	_mMethodCreateKeypadEffect = (CHROMA_SDK_CREATEKEYPADEFFECT)GetProcAddress(_mLibraryChroma, "CreateKeypadEffect");

	_mMethodCreateEffect = (CHROMA_SDK_CREATEEFFECT)GetProcAddress(_mLibraryChroma, "CreateEffect");
	_mMethodSetEffect = (CHROMA_SDK_SETEFFECT)GetProcAddress(_mLibraryChroma, "SetEffect");
	_mMethodDeleteEffect = (CHROMA_SDK_DELETEEFFECT)GetProcAddress(_mLibraryChroma, "DeleteEffect");

	RZRESULT Result = RZRESULT_INVALID;
#pragma warning(default: 4191)
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
