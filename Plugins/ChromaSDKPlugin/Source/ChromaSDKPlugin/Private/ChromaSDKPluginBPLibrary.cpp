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

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectStatic(const EChromaSDKDeviceEnum& device, const FLinearColor& color)
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

void UChromaSDKPluginBPLibrary::ChromaSDKPrintEffectId(const FChromaSDKGuid& effectId)
{
	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin EffectId: %d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d"), 
		effectId.Data1, //1
		effectId.Data2, //2
		effectId.Data3, //3
		effectId.Data4[0], //4
		effectId.Data4[1], //5
		effectId.Data4[2], //6
		effectId.Data4[3], //7
		effectId.Data4[4], //8
		effectId.Data4[5], //9
		effectId.Data4[6], //10
		effectId.Data4[7]); //11
}
