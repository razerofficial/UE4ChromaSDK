// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "Runtime/Launch/Resources/Version.h"

//expose HMODULE
#if PLATFORM_WINDOWS
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"
#include <map>
#include <string>
#include "ChromaSDKDevice1DEnum.h"
#include "ChromaSDKDevice2DEnum.h"

typedef RZRESULT(*CHROMA_SDK_INIT)(void);
typedef RZRESULT(*CHROMA_SDK_UNINIT)(void);
typedef RZRESULT(*CHROMA_SDK_CREATE_EFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATE_CHROMA_LINK_EFFECT)(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATE_KEYBOARD_EFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATE_KEYPAD_EFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATE_HEADSET_EFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATE_MOUSE_EFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_CREATE_MOUSEPAD_EFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CHROMA_SDK_SET_EFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*CHROMA_SDK_DELETE_EFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*CHROMA_SDK_REGISTER_EVENT_NOTIFICATION)(HWND hWnd);
typedef RZRESULT(*CHROMA_SDK_UNREGISTER_EVENT_NOTIFICATION)(void);
typedef RZRESULT(*CHROMA_SDK_QUERY_DEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);
#endif

namespace ChromaSDK
{
	class AnimationBase;
}

class FChromaSDKPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FChromaSDKPluginModule& Get()
	{
		return FModuleManager::GetModuleChecked<FChromaSDKPluginModule>("ChromaSDKPlugin");
	}

#if PLATFORM_WINDOWS
	// SDK Methods
	int ChromaSDKInit();
	int ChromaSDKUnInit();
	bool IsInitialized();
	RZRESULT ChromaSDKCreateEffect(RZDEVICEID deviceId, ChromaSDK::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	RZRESULT ChromaSDKCreateChromaLinkEffect(ChromaSDK::ChromaLink::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	RZRESULT ChromaSDKCreateHeadsetEffect(ChromaSDK::Headset::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	RZRESULT ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	RZRESULT ChromaSDKCreateKeypadEffect(ChromaSDK::Keypad::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	RZRESULT ChromaSDKCreateMouseEffect(ChromaSDK::Mouse::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	RZRESULT ChromaSDKCreateMousepadEffect(ChromaSDK::Mousepad::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
	RZRESULT ChromaSDKSetEffect(RZEFFECTID effectId);
	RZRESULT ChromaSDKDeleteEffect(RZEFFECTID effectId);

	static int ToBGR(const FLinearColor& color);
	static FLinearColor ToLinearColor(int color);
	static int GetMaxLeds(const EChromaSDKDevice1DEnum& device);
	static int GetMaxRow(const EChromaSDKDevice2DEnum& device);
	static int GetMaxColumn(const EChromaSDKDevice2DEnum& device);
	int OpenAnimation(const char* path);
	int CloseAnimation(int animationId);
	int CloseAnimationName(const char* path);
	int GetAnimation(const char* path);
	ChromaSDK::AnimationBase* GetAnimationInstance(int animationId);
	void PlayAnimation(int animationId, bool loop);
	void PlayAnimationName(const char* path, bool loop);
	void StopAnimation(int animationId);
	void StopAnimationName(const char* path);
	void StopAnimationType(int deviceType, int device);
	bool IsAnimationPlaying(int animationId);
	bool IsAnimationPlayingName(const char* path);
	bool IsAnimationPlayingType(int deviceType, int device);
	int GetAnimationFrameCount(int animationId);
	int GetAnimationFrameCountName(const char* path);
	void SetKeyColor(int animationId, int frameId, int rzkey, COLORREF color);
	void SetKeyColorName(const char* path, int frameId, int rzkey, COLORREF color);
	void LoadAnimation(int animationId);
	void LoadAnimationName(const char* path);
	void UnloadAnimation(int animationId);
	void UnloadAnimationName(const char* path);
#endif

private:
#if PLATFORM_WINDOWS
	bool ValidateGetProcAddress(bool condition, FString methodName);

	bool _mInitialized;

	HMODULE _mLibraryChroma = nullptr;

	CHROMA_SDK_INIT _mMethodInit = NULL;
	CHROMA_SDK_UNINIT _mMethodUnInit = NULL;
	CHROMA_SDK_CREATE_EFFECT _mMethodCreateEffect = NULL;
	CHROMA_SDK_CREATE_CHROMA_LINK_EFFECT _mMethodCreateChromaLinkEffect = NULL;
	CHROMA_SDK_CREATE_HEADSET_EFFECT _mMethodCreateHeadsetEffect = NULL;
	CHROMA_SDK_CREATE_KEYBOARD_EFFECT _mMethodCreateKeyboardEffect = NULL;
	CHROMA_SDK_CREATE_KEYPAD_EFFECT _mMethodCreateKeypadEffect = NULL;
	CHROMA_SDK_CREATE_MOUSE_EFFECT _mMethodCreateMouseEffect = NULL;
	CHROMA_SDK_CREATE_MOUSEPAD_EFFECT _mMethodCreateMousepadEffect = NULL;
	CHROMA_SDK_SET_EFFECT _mMethodSetEffect = NULL;
	CHROMA_SDK_DELETE_EFFECT _mMethodDeleteEffect = NULL;
	CHROMA_SDK_QUERY_DEVICE _mMethodQueryDevice = NULL;

	int _mAnimationId;
	std::map<std::string, int> _mAnimationMapID;
	std::map<int, ChromaSDK::AnimationBase*> _mAnimations;
	std::map<EChromaSDKDevice1DEnum, int> _mPlayMap1D;
	std::map<EChromaSDKDevice2DEnum, int> _mPlayMap2D;
#endif
};
