// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginBPLibrary.h"
#include "ChromaSDKPlugin.h"

using namespace ChromaSDK;

UChromaSDKPluginBPLibrary::UChromaSDKPluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UChromaSDKPluginBPLibrary::IsPlatformWindows()
{
#if PLATFORM_WINDOWS
	return true;
#else
	return false;
#endif
}

int UChromaSDKPluginBPLibrary::ChromaSDKInit()
{
	return FChromaSDKPluginModule::Get().ChromaSDKInit();
}

int UChromaSDKPluginBPLibrary::ChromaSDKUnInit()
{
	return FChromaSDKPluginModule::Get().ChromaSDKUnInit();
}

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectStatic(const EChromaSDKDeviceEnum& device, const FColor& color)
{
	FChromaSDKEffectResult data;
	
	ChromaSDK::Keyboard::STATIC_EFFECT_TYPE pParam = {};
	pParam.Color = RGB(color.R, color.G, color.B);
	int result = 0;
	RZEFFECTID effectId;
	switch (device)
	{
	case EChromaSDKDeviceEnum::DE_Keyboard:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &pParam, &effectId);
		break;
	}

	data.EffectId = FChromaSDKGuid();
	data.EffectId.Data1 = effectId.Data1;
	data.EffectId.Data2 = effectId.Data2;
	data.EffectId.Data3 = effectId.Data3;
	for (int i = 0; i < 8; ++i)
	{
		data.EffectId.Data4[i] = effectId.Data4[i];
	}

	data.Result = result;

	return data;
}
