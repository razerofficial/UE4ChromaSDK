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
using namespace ChromaSDK::ChromaLink;
using namespace ChromaSDK::Headset;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Mousepad;

bool FChromaSDKPluginModule::ValidateGetProcAddress(bool condition, FString methodName)
{
	if (condition)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin failed to load %s!"), *methodName);
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin loaded %s."), *methodName);
	}
	return condition;
}

#endif

void FChromaSDKPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if PLATFORM_WINDOWS
	_mLibraryChroma = LoadLibrary(CHROMASDKDLL);
	if (_mLibraryChroma == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin failed to load!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin loaded."));

	// GetProcAddress will throw 4191 because it's an unsafe type cast
#pragma warning(disable: 4191)
	_mMethodInit = (CHROMA_SDK_INIT)GetProcAddress(_mLibraryChroma, "Init");
	if (ValidateGetProcAddress(_mMethodInit == nullptr, FString("Init")))
	{
		return;
	}
	_mMethodUnInit = (CHROMA_SDK_UNINIT)GetProcAddress(_mLibraryChroma, "UnInit");
	if (ValidateGetProcAddress(_mMethodUnInit == nullptr, FString("UnInit")))
	{
		return;
	}
	_mMethodQueryDevice = (CHROMA_SDK_QUERY_DEVICE)GetProcAddress(_mLibraryChroma, "QueryDevice");
	if (ValidateGetProcAddress(_mMethodQueryDevice == nullptr, FString("QueryDevice")))
	{
		return;
	}
	_mMethodUnInit = (CHROMA_SDK_UNINIT)GetProcAddress(_mLibraryChroma, "UnInit");
	if (ValidateGetProcAddress(_mMethodUnInit == nullptr, FString("UnInit")))
	{
		return;
	}

	_mMethodCreateChromaLinkEffect = (CHROMA_SDK_CREATE_CHROMA_LINK_EFFECT)GetProcAddress(_mLibraryChroma, "CreateChromaLinkEffect");
	if (ValidateGetProcAddress(_mMethodCreateChromaLinkEffect == nullptr, FString("CreateChromaLinkEffect")))
	{
		return;
	}
	_mMethodCreateHeadsetEffect = (CHROMA_SDK_CREATE_HEADSET_EFFECT)GetProcAddress(_mLibraryChroma, "CreateHeadsetEffect");
	if (ValidateGetProcAddress(_mMethodCreateHeadsetEffect == nullptr, FString("CreateHeadsetEffect")))
	{
		return;
	}
	_mMethodCreateKeyboardEffect = (CHROMA_SDK_CREATE_KEYBOARD_EFFECT)GetProcAddress(_mLibraryChroma, "CreateKeyboardEffect");
	if (ValidateGetProcAddress(_mMethodCreateKeyboardEffect == nullptr, FString("CreateKeyboardEffect")))
	{
		return;
	}
	_mMethodCreateMouseEffect = (CHROMA_SDK_CREATE_MOUSE_EFFECT)GetProcAddress(_mLibraryChroma, "CreateMouseEffect");
	if (ValidateGetProcAddress(_mMethodCreateMouseEffect == nullptr, FString("CreateMouseEffect")))
	{
		return;
	}
	_mMethodCreateMousepadEffect = (CHROMA_SDK_CREATE_MOUSEPAD_EFFECT)GetProcAddress(_mLibraryChroma, "CreateMousepadEffect");
	if (ValidateGetProcAddress(_mMethodCreateMousepadEffect == nullptr, FString("CreateMousepadEffect")))
	{
		return;
	}
	_mMethodCreateKeypadEffect = (CHROMA_SDK_CREATE_KEYPAD_EFFECT)GetProcAddress(_mLibraryChroma, "CreateKeypadEffect");
	if (ValidateGetProcAddress(_mMethodCreateKeypadEffect == nullptr, FString("CreateKeypadEffect")))
	{
		return;
	}

	_mMethodCreateEffect = (CHROMA_SDK_CREATE_EFFECT)GetProcAddress(_mLibraryChroma, "CreateEffect");
	if (ValidateGetProcAddress(_mMethodCreateEffect == nullptr, FString("CreateEffect")))
	{
		return;
	}
	_mMethodSetEffect = (CHROMA_SDK_SET_EFFECT)GetProcAddress(_mLibraryChroma, "SetEffect");
	if (ValidateGetProcAddress(_mMethodSetEffect == nullptr, FString("SetEffect")))
	{
		return;
	}
	_mMethodDeleteEffect = (CHROMA_SDK_DELETE_EFFECT)GetProcAddress(_mLibraryChroma, "DeleteEffect");
	if (ValidateGetProcAddress(_mMethodDeleteEffect == nullptr, FString("DeleteEffect")))
	{
		return;
	}
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

#if PLATFORM_WINDOWS

int FChromaSDKPluginModule::ChromaSDKInit()
{
	if (_mMethodInit == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin Init method is not set!"));
		return -1;
	}

	return _mMethodInit();
}

int FChromaSDKPluginModule::ChromaSDKUnInit()
{
	if (_mMethodUnInit == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin UnInit method is not set!"));
		return -1;
	}

	return _mMethodUnInit();
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateEffect(RZDEVICEID deviceId, ChromaSDK::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateEffect(deviceId, effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateChromaLinkEffect(ChromaSDK::ChromaLink::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateChromaLinkEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateChromaLinkEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateChromaLinkEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateHeadsetEffect(ChromaSDK::Headset::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateHeadsetEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateHeadsetEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateHeadsetEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateKeyboardEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateKeyboardEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateKeyboardEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateKeypadEffect(ChromaSDK::Keypad::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateKeypadEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateKeypadEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateKeypadEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateMouseEffect(ChromaSDK::Mouse::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateMouseEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateMouseEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateMouseEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateMousepadEffect(ChromaSDK::Mousepad::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateMousepadEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateMousepadEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateMousepadEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKSetEffect(RZEFFECTID effectId)
{
	if (_mMethodSetEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin SetEffect method is not set!"));
		return -1;
	}

	return _mMethodSetEffect(effectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKDeleteEffect(RZEFFECTID effectId)
{
	if (_mMethodDeleteEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin DeleteEffect method is not set!"));
		return -1;
	}

	return _mMethodDeleteEffect(effectId);
}

#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChromaSDKPluginModule, ChromaSDKPlugin)
