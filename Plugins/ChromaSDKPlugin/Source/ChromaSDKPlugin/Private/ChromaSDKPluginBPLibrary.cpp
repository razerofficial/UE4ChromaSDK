// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginBPLibrary.h"
#include "ChromaSDKPlugin.h"
#include <string>

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

FString UChromaSDKPluginBPLibrary::DebugToString(const FChromaSDKGuid& effectId)
{
#if PLATFORM_WINDOWS
	FString result;
	ToString(effectId.Data, result);
	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin::DebugToString EffectString=%s"), *result);
	return result;
#else
	return TEXT("NOT SUPPORTED");
#endif
}

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectStatic(const EChromaSDKDeviceEnum& device, const FLinearColor& color)
{
	FChromaSDKEffectResult data = FChromaSDKEffectResult();

#if PLATFORM_WINDOWS

	//UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin:: Color R=%f G=%f B=%f"), color.R, color.G, color.B);
	
	ChromaSDK::Keyboard::STATIC_EFFECT_TYPE pParam = {};
	int red = color.R * 255;
	int green = color.G * 255;
	int blue = color.B * 255;
	pParam.Color = RGB(red, green, blue);
	int result = 0;
	RZEFFECTID effectId;
	switch (device)
	{
	case EChromaSDKDeviceEnum::DE_ChromaLink:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateChromaLinkEffect(ChromaSDK::ChromaLink::CHROMA_STATIC, &pParam, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Headset:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateHeadsetEffect(ChromaSDK::Headset::CHROMA_STATIC, &pParam, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Keyboard:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &pParam, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Keypad:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeypadEffect(ChromaSDK::Keypad::CHROMA_STATIC, &pParam, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Mouse:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateMouseEffect(ChromaSDK::Mouse::CHROMA_STATIC, &pParam, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Mousepad:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_STATIC, &pParam, &effectId);
		break;
	}
	data.EffectId.Data = effectId;
	data.Result = result;

#endif

	return data;
}

int UChromaSDKPluginBPLibrary::ChromaSDKSetEffect(const FChromaSDKGuid& effectId)
{
#if PLATFORM_WINDOWS
	return FChromaSDKPluginModule::Get().ChromaSDKSetEffect(effectId.Data);
#else
	return -1;
#endif
}

#if PLATFORM_WINDOWS
void UChromaSDKPluginBPLibrary::ToString(const RZEFFECTID& effectId, FString& effectString)
{
	unsigned long p0;
	int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

	p0 = effectId.Data1;
	p1 = effectId.Data2;
	p2 = effectId.Data3;
	p3 = effectId.Data4[0];
	p4 = effectId.Data4[1];
	p5 = effectId.Data4[2];
	p6 = effectId.Data4[3];
	p7 = effectId.Data4[4];
	p8 = effectId.Data4[5];
	p9 = effectId.Data4[6];
	p10 = effectId.Data4[7];

	char buffer[256];
	int err = sprintf_s(buffer, "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	effectString = buffer;

	//UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin::ToString EffectString=%s"), *effectString);
}

void UChromaSDKPluginBPLibrary::ToEffect(const FString& effectString, RZEFFECTID& effectId)
{
	unsigned long p0;
	int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

	int err = sscanf_s(TCHAR_TO_ANSI(*effectString), "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		&p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

	effectId.Data1 = p0;
	effectId.Data2 = p1;
	effectId.Data3 = p2;
	effectId.Data4[0] = p3;
	effectId.Data4[1] = p4;
	effectId.Data4[2] = p5;
	effectId.Data4[3] = p6;
	effectId.Data4[4] = p7;
	effectId.Data4[5] = p8;
	effectId.Data4[6] = p9;
	effectId.Data4[7] = p10;

	//UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin::ToEffect EffectString=%s"), *effectString);
}
#endif
