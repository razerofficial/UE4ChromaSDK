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

FChromaSDKColors UChromaSDKPluginBPLibrary::CreateRandomColors1D(int elements)
{
	FChromaSDKColors result = FChromaSDKColors();
	for (int i = 0; i < elements; ++i)
	{
		float red = FMath::RandRange(0.0f, 1.0f);
		float green = FMath::RandRange(0.0f, 1.0f);
		float blue = FMath::RandRange(0.0f, 1.0f);
		FLinearColor color = FLinearColor(red, green, blue);
		result.Colors.Add(color);
	}
	return result;
}

TArray<FChromaSDKColors> UChromaSDKPluginBPLibrary::CreateRandomColors2D(int maxRows, int maxColumns)
{
	TArray<FChromaSDKColors> result = TArray<FChromaSDKColors>();
	for (int i = 0; i < maxRows; ++i)
	{
		FChromaSDKColors row = FChromaSDKColors();
		for (int j = 0; j < maxColumns; ++j)
		{
			float red = FMath::RandRange(0.0f, 1.0f);
			float green = FMath::RandRange(0.0f, 1.0f);
			float blue = FMath::RandRange(0.0f, 1.0f);
			FLinearColor color = FLinearColor(red, green, blue);
			row.Colors.Add(color);
		}
		result.Add(row);
	}
	return result;
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

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectNone(const EChromaSDKDeviceEnum& device)
{
	FChromaSDKEffectResult data = FChromaSDKEffectResult();

#if PLATFORM_WINDOWS

	int result = 0;
	RZEFFECTID effectId = RZEFFECTID();
	switch (device)
	{
	case EChromaSDKDeviceEnum::DE_ChromaLink:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateChromaLinkEffect(ChromaSDK::ChromaLink::CHROMA_NONE, NULL, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Headset:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateHeadsetEffect(ChromaSDK::Headset::CHROMA_NONE, NULL, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Keyboard:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, NULL, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Keypad:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeypadEffect(ChromaSDK::Keypad::CHROMA_NONE, NULL, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Mouse:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateMouseEffect(ChromaSDK::Mouse::CHROMA_NONE, NULL, &effectId);
		break;
	case EChromaSDKDeviceEnum::DE_Mousepad:
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_NONE, NULL, &effectId);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectNone Unsupported device used!"));
		break;
	}
	data.EffectId.Data = effectId;
	data.Result = result;

#endif

	return data;
}

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectStatic(const EChromaSDKDeviceEnum& device, const FLinearColor& color)
{
	FChromaSDKEffectResult data = FChromaSDKEffectResult();

#if PLATFORM_WINDOWS

	//UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin:: Color R=%f G=%f B=%f"), color.R, color.G, color.B);	
	int red = color.R * 255;
	int green = color.G * 255;
	int blue = color.B * 255;
	
	int result = 0;
	RZEFFECTID effectId = RZEFFECTID();
	switch (device)
	{
	case EChromaSDKDeviceEnum::DE_ChromaLink:
		{
			ChromaSDK::ChromaLink::STATIC_EFFECT_TYPE pParam = {};
			pParam.Color = RGB(red, green, blue);
			result = FChromaSDKPluginModule::Get().ChromaSDKCreateChromaLinkEffect(ChromaSDK::ChromaLink::CHROMA_STATIC, &pParam, &effectId);
		}
		break;
	case EChromaSDKDeviceEnum::DE_Headset:
		{
			ChromaSDK::Headset::STATIC_EFFECT_TYPE pParam = {};
			pParam.Color = RGB(red, green, blue);
			result = FChromaSDKPluginModule::Get().ChromaSDKCreateHeadsetEffect(ChromaSDK::Headset::CHROMA_STATIC, &pParam, &effectId);
		}
		break;
	case EChromaSDKDeviceEnum::DE_Keyboard:
		{
			ChromaSDK::Keyboard::STATIC_EFFECT_TYPE pParam = {};
			pParam.Color = RGB(red, green, blue);
			result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &pParam, &effectId);
		}
		break;
	case EChromaSDKDeviceEnum::DE_Keypad:
		{
			ChromaSDK::Keypad::STATIC_EFFECT_TYPE pParam = {};
			pParam.Color = RGB(red, green, blue);
			result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeypadEffect(ChromaSDK::Keypad::CHROMA_STATIC, &pParam, &effectId);
		}
		break;
	case EChromaSDKDeviceEnum::DE_Mouse:
		{
			ChromaSDK::Mouse::STATIC_EFFECT_TYPE pParam = {};
			pParam.Color = RGB(red, green, blue);
			pParam.LEDId = ChromaSDK::Mouse::RZLED_ALL;
			result = FChromaSDKPluginModule::Get().ChromaSDKCreateMouseEffect(ChromaSDK::Mouse::CHROMA_STATIC, &pParam, &effectId);
		}
		break;
	case EChromaSDKDeviceEnum::DE_Mousepad:
		{
			ChromaSDK::Mousepad::STATIC_EFFECT_TYPE pParam = {};
			pParam.Color = RGB(red, green, blue);
			result = FChromaSDKPluginModule::Get().ChromaSDKCreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_STATIC, &pParam, &effectId);
		}
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectStatic Unsupported device used!"));
		break;
	}
	data.EffectId.Data = effectId;
	data.Result = result;

#endif

	return data;
}

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom1D(const EChromaSDKDevice1DEnum& device, const FChromaSDKColors& colors)
{
	FChromaSDKEffectResult data = FChromaSDKEffectResult();

#if PLATFORM_WINDOWS

	int result = 0;
	RZEFFECTID effectId = RZEFFECTID();
	int maxLeds = 0;
	switch (device)
	{
	case EChromaSDKDevice1DEnum::DE_ChromaLink:
	{
		maxLeds = ChromaSDK::ChromaLink::MAX_LEDS;
		if (maxLeds != colors.Colors.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom1D Array size mismatch elements: %d==%d!"),
				maxLeds,
				colors.Colors.Num());
			break;
		}
		ChromaSDK::ChromaLink::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxLeds; i++)
		{
			const FLinearColor& color = colors.Colors[i];
			int red = color.R * 255;
			int green = color.G * 255;
			int blue = color.B * 255;
			pParam.Color[i] = RGB(red, green, blue);
		}
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateChromaLinkEffect(ChromaSDK::ChromaLink::CHROMA_CUSTOM, &pParam, &effectId);
	}
	break;
	case EChromaSDKDevice1DEnum::DE_Headset:
	{
		maxLeds = ChromaSDK::Headset::MAX_LEDS;
		if (maxLeds != colors.Colors.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom1D Array size mismatch elements: %d==%d!"),
				maxLeds,
				colors.Colors.Num());
			break;
		}
		ChromaSDK::Headset::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxLeds; i++)
		{
			const FLinearColor& color = colors.Colors[i];
			int red = color.R * 255;
			int green = color.G * 255;
			int blue = color.B * 255;
			pParam.Color[i] = RGB(red, green, blue);
		}
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateHeadsetEffect(ChromaSDK::Headset::CHROMA_CUSTOM, &pParam, &effectId);
	}
	break;
	case EChromaSDKDevice1DEnum::DE_Mousepad:
	{
		maxLeds = ChromaSDK::Mousepad::MAX_LEDS;
		if (maxLeds != colors.Colors.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom1D Array size mismatch elements: %d==%d!"),
				maxLeds,
				colors.Colors.Num());
			break;
		}
		ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxLeds; i++)
		{
			const FLinearColor& color = colors.Colors[i];
			int red = color.R * 255;
			int green = color.G * 255;
			int blue = color.B * 255;
			pParam.Color[i] = RGB(red, green, blue);
		}
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateMousepadEffect(ChromaSDK::Mousepad::CHROMA_CUSTOM, &pParam, &effectId);
	}
	break;
	default:
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom1D Unsupported device used!"));
		break;
	}
	data.EffectId.Data = effectId;
	data.Result = result;

#endif

	return data;
}

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom2D(const EChromaSDKDevice2DEnum& device, const TArray<FChromaSDKColors>& colors)
{
	FChromaSDKEffectResult data = FChromaSDKEffectResult();

#if PLATFORM_WINDOWS

	int result = 0;
	RZEFFECTID effectId = RZEFFECTID();
	int maxRow = 0;
	int maxColumn = 0;
	switch (device)
	{
	case EChromaSDKDevice2DEnum::DE_Keyboard:
	{
		maxRow = ChromaSDK::Keyboard::MAX_ROW;
		maxColumn = ChromaSDK::Keyboard::MAX_COLUMN;
		if (maxRow != colors.Num() ||
			(colors.Num() > 0 &&
			maxColumn != colors[0].Colors.Num()))
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom2D Array size mismatch row: %d==%d column: %d==%d!"),
				maxRow,
				colors.Num(),
				maxColumn,
				colors.Num() > 0 ? colors[0].Colors.Num() : 0);
			break;
		}
		ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxRow; i++)
		{
			const FChromaSDKColors& row = colors[i];
			for (int j = 0; j < maxColumn; j++)
			{
				const FLinearColor& color = row.Colors[j];
				int red = color.R * 255;
				int green = color.G * 255;
				int blue = color.B * 255;
				pParam.Color[i][j] = RGB(red, green, blue);
			}
		}
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &pParam, &effectId);
	}
	break;
	case EChromaSDKDevice2DEnum::DE_Keypad:
	{
		maxRow = ChromaSDK::Keypad::MAX_ROW;
		maxColumn = ChromaSDK::Keypad::MAX_COLUMN;
		if (maxRow != colors.Num() ||
			(colors.Num() > 0 &&
			maxColumn != colors[0].Colors.Num()))
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom2D Array size mismatch row: %d==%d column: %d==%d!"),
				maxRow,
				colors.Num(),
				maxColumn,
				colors.Num() > 0 ? colors[0].Colors.Num() : 0);
			break;
		}
		ChromaSDK::Keypad::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxRow; i++)
		{
			const FChromaSDKColors& row = colors[i];
			for (int j = 0; j < maxColumn; j++)
			{
				const FLinearColor& color = row.Colors[j];
				int red = color.R * 255;
				int green = color.G * 255;
				int blue = color.B * 255;
				pParam.Color[i][j] = RGB(red, green, blue);
			}
		}
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateKeypadEffect(ChromaSDK::Keypad::CHROMA_CUSTOM, &pParam, &effectId);
	}
	break;
	case EChromaSDKDevice2DEnum::DE_Mouse:
	{
		maxRow = ChromaSDK::Mouse::MAX_ROW;
		maxColumn = ChromaSDK::Mouse::MAX_COLUMN;
		if (maxRow != colors.Num() ||
			(colors.Num() > 0 &&
			maxColumn != colors[0].Colors.Num()))
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom2D Array size mismatch row: %d==%d column: %d==%d!"),
				maxRow,
				colors.Num(),
				maxColumn,
				colors.Num() > 0 ? colors[0].Colors.Num() : 0);
			break;
		}
		ChromaSDK::Mouse::CUSTOM_EFFECT_TYPE2 pParam = {};
		for (int i = 0; i < maxRow; i++)
		{
			const FChromaSDKColors& row = colors[i];
			for (int j = 0; j < maxColumn; j++)
			{
				const FLinearColor& color = row.Colors[j];
				int red = color.R * 255;
				int green = color.G * 255;
				int blue = color.B * 255;
				pParam.Color[i][j] = RGB(red, green, blue);
			}
		}
		result = FChromaSDKPluginModule::Get().ChromaSDKCreateMouseEffect(ChromaSDK::Mouse::CHROMA_CUSTOM2, &pParam, &effectId);
	}
	break;
	default:
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom2D Unsupported device used!"));
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

int UChromaSDKPluginBPLibrary::ChromaSDKDeleteEffect(const FChromaSDKGuid& effectId)
{
#if PLATFORM_WINDOWS
	return FChromaSDKPluginModule::Get().ChromaSDKDeleteEffect(effectId.Data);
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
