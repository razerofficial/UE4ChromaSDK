// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

//expose HMODULE
#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"
#include "HideWindowsPlatformTypes.h"

typedef RZRESULT(*CHROMA_SDK_INIT)(void);
typedef RZRESULT(*CHROMA_SDK_UNINIT)(void);
typedef RZRESULT(*CHROMA_SDK_CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATECHROMALINKEFFECT)(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*CHROMA_SDK_DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*CHROMA_SDK_REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*CHROMA_SDK_UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*CHROMA_SDK_QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);
#endif

class FChromaSDKPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#if PLATFORM_WINDOWS
	HMODULE _mLibraryChroma = nullptr;

	CHROMA_SDK_INIT _mMethodInit = NULL;
	CHROMA_SDK_UNINIT _mMethodUnInit = NULL;
	CHROMA_SDK_CREATEEFFECT _mMethodCreateEffect = NULL;
	CHROMA_SDK_CREATEKEYBOARDEFFECT _mMethodCreateKeyboardEffect = NULL;
	CHROMA_SDK_CREATEMOUSEEFFECT _mMethodCreateMouseEffect = NULL;
	CHROMA_SDK_CREATEHEADSETEFFECT _mMethodCreateHeadsetEffect = NULL;
	CHROMA_SDK_CREATEMOUSEPADEFFECT _mMethodCreateMousepadEffect = NULL;
	CHROMA_SDK_CREATEKEYPADEFFECT _mMethodCreateKeypadEffect = NULL;
	CHROMA_SDK_CREATECHROMALINKEFFECT _mMethodCreateChromaLinkEffect = NULL;
	CHROMA_SDK_SETEFFECT _mMethodSetEffect = NULL;
	CHROMA_SDK_DELETEEFFECT _mMethodDeleteEffect = NULL;
	CHROMA_SDK_QUERYDEVICE _mMethodQueryDevice = NULL;
#endif
};
