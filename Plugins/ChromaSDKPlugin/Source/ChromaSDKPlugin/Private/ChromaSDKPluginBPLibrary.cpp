// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

//#include "ChromaSDKPlugin.h" //(support 4.15 or below)___HACK_UE4_WANTS_MODULE_FIRST
#include "ChromaSDKPluginBPLibrary.h"
#include "ChromaSDKPlugin.h" //(support 4.16 or above)___HACK_UE4_WANTS_HEADER_FIRST

#include "ChromaSDKPluginAnimation1DObject.h"
#include "ChromaSDKPluginAnimation2DObject.h"
#include <string>

#if PLATFORM_WINDOWS

#include "AllowWindowsPlatformTypes.h" 

using namespace ChromaSDK;
using namespace std;

// keyboard map
std::map<EChromaSDKKeyboardKey, int> UChromaSDKPluginBPLibrary::_sKeyboardEnumMap =
	std::map<EChromaSDKKeyboardKey, int>();

// mouse map
std::map<EChromaSDKMouseLed, ChromaSDK::Mouse::RZLED2> UChromaSDKPluginBPLibrary::_sMouseEnumMap =
	std::map<EChromaSDKMouseLed, ChromaSDK::Mouse::RZLED2>();

// initialized
bool UChromaSDKPluginBPLibrary::_sInitialized = false;

#endif

UChromaSDKPluginBPLibrary::UChromaSDKPluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_WINDOWS
	// keyboard mapping
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_ESC] = Keyboard::RZKEY::RZKEY_ESC;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F1] = Keyboard::RZKEY::RZKEY_F1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F2] = Keyboard::RZKEY::RZKEY_F2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F3] = Keyboard::RZKEY::RZKEY_F3;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F4] = Keyboard::RZKEY::RZKEY_F4;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F5] = Keyboard::RZKEY::RZKEY_F5;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F6] = Keyboard::RZKEY::RZKEY_F6;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F7] = Keyboard::RZKEY::RZKEY_F7;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F8] = Keyboard::RZKEY::RZKEY_F8;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F9] = Keyboard::RZKEY::RZKEY_F9;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F10] = Keyboard::RZKEY::RZKEY_F10;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F11] = Keyboard::RZKEY::RZKEY_F11;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F12] = Keyboard::RZKEY::RZKEY_F12;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_1] = Keyboard::RZKEY::RZKEY_1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_2] = Keyboard::RZKEY::RZKEY_2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_3] = Keyboard::RZKEY::RZKEY_3;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_4] = Keyboard::RZKEY::RZKEY_4;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_5] = Keyboard::RZKEY::RZKEY_5;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_6] = Keyboard::RZKEY::RZKEY_6;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_7] = Keyboard::RZKEY::RZKEY_7;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_8] = Keyboard::RZKEY::RZKEY_8;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_9] = Keyboard::RZKEY::RZKEY_9;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_0] = Keyboard::RZKEY::RZKEY_0;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_A] = Keyboard::RZKEY::RZKEY_A;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_B] = Keyboard::RZKEY::RZKEY_B;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_C] = Keyboard::RZKEY::RZKEY_C;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_D] = Keyboard::RZKEY::RZKEY_D;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_E] = Keyboard::RZKEY::RZKEY_E;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_F] = Keyboard::RZKEY::RZKEY_F;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_G] = Keyboard::RZKEY::RZKEY_G;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_H] = Keyboard::RZKEY::RZKEY_H;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_I] = Keyboard::RZKEY::RZKEY_I;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_J] = Keyboard::RZKEY::RZKEY_J;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_K] = Keyboard::RZKEY::RZKEY_K;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_L] = Keyboard::RZKEY::RZKEY_L;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_M] = Keyboard::RZKEY::RZKEY_M;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_N] = Keyboard::RZKEY::RZKEY_N;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_O] = Keyboard::RZKEY::RZKEY_O;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_P] = Keyboard::RZKEY::RZKEY_P;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_Q] = Keyboard::RZKEY::RZKEY_Q;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_R] = Keyboard::RZKEY::RZKEY_R;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_S] = Keyboard::RZKEY::RZKEY_S;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_T] = Keyboard::RZKEY::RZKEY_T;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_U] = Keyboard::RZKEY::RZKEY_U;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_V] = Keyboard::RZKEY::RZKEY_V;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_W] = Keyboard::RZKEY::RZKEY_W;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_X] = Keyboard::RZKEY::RZKEY_X;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_Y] = Keyboard::RZKEY::RZKEY_Y;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_Z] = Keyboard::RZKEY::RZKEY_Z;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMLOCK] = Keyboard::RZKEY::RZKEY_NUMLOCK;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD0] = Keyboard::RZKEY::RZKEY_NUMPAD0;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD1] = Keyboard::RZKEY::RZKEY_NUMPAD1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD2] = Keyboard::RZKEY::RZKEY_NUMPAD2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD3] = Keyboard::RZKEY::RZKEY_NUMPAD3;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD4] = Keyboard::RZKEY::RZKEY_NUMPAD4;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD5] = Keyboard::RZKEY::RZKEY_NUMPAD5;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD6] = Keyboard::RZKEY::RZKEY_NUMPAD6;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD7] = Keyboard::RZKEY::RZKEY_NUMPAD7;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD8] = Keyboard::RZKEY::RZKEY_NUMPAD8;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD9] = Keyboard::RZKEY::RZKEY_NUMPAD9;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD_DIVIDE] = Keyboard::RZKEY::RZKEY_NUMPAD_DIVIDE;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD_MULTIPLY] = Keyboard::RZKEY::RZKEY_NUMPAD_MULTIPLY;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD_SUBTRACT] = Keyboard::RZKEY::RZKEY_NUMPAD_SUBTRACT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD_ADD] = Keyboard::RZKEY::RZKEY_NUMPAD_ADD;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD_ENTER] = Keyboard::RZKEY::RZKEY_NUMPAD_ENTER;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_NUMPAD_DECIMAL] = Keyboard::RZKEY::RZKEY_NUMPAD_DECIMAL;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_PRINTSCREEN] = Keyboard::RZKEY::RZKEY_PRINTSCREEN;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_SCROLL] = Keyboard::RZKEY::RZKEY_SCROLL;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_PAUSE] = Keyboard::RZKEY::RZKEY_PAUSE;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_INSERT] = Keyboard::RZKEY::RZKEY_INSERT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_HOME] = Keyboard::RZKEY::RZKEY_HOME;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_PAGEUP] = Keyboard::RZKEY::RZKEY_PAGEUP;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_DELETE] = Keyboard::RZKEY::RZKEY_DELETE;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_END] = Keyboard::RZKEY::RZKEY_END;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_PAGEDOWN] = Keyboard::RZKEY::RZKEY_PAGEDOWN;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_UP] = Keyboard::RZKEY::RZKEY_UP;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_LEFT] = Keyboard::RZKEY::RZKEY_LEFT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_DOWN] = Keyboard::RZKEY::RZKEY_DOWN;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_RIGHT] = Keyboard::RZKEY::RZKEY_RIGHT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_TAB] = Keyboard::RZKEY::RZKEY_TAB;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_CAPSLOCK] = Keyboard::RZKEY::RZKEY_CAPSLOCK;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_BACKSPACE] = Keyboard::RZKEY::RZKEY_BACKSPACE;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_ENTER] = Keyboard::RZKEY::RZKEY_ENTER;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_LCTRL] = Keyboard::RZKEY::RZKEY_LCTRL;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_LWIN] = Keyboard::RZKEY::RZKEY_LWIN;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_LALT] = Keyboard::RZKEY::RZKEY_LALT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_SPACE] = Keyboard::RZKEY::RZKEY_SPACE;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_RALT] = Keyboard::RZKEY::RZKEY_RALT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_FN] = Keyboard::RZKEY::RZKEY_FN;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_RMENU] = Keyboard::RZKEY::RZKEY_RMENU;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_RCTRL] = Keyboard::RZKEY::RZKEY_RCTRL;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_LSHIFT] = Keyboard::RZKEY::RZKEY_LSHIFT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_RSHIFT] = Keyboard::RZKEY::RZKEY_RSHIFT;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_MACRO1] = Keyboard::RZKEY::RZKEY_MACRO1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_MACRO2] = Keyboard::RZKEY::RZKEY_MACRO2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_MACRO3] = Keyboard::RZKEY::RZKEY_MACRO3;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_MACRO4] = Keyboard::RZKEY::RZKEY_MACRO4;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_MACRO5] = Keyboard::RZKEY::RZKEY_MACRO5;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_1] = Keyboard::RZKEY::RZKEY_OEM_1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_2] = Keyboard::RZKEY::RZKEY_OEM_2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_3] = Keyboard::RZKEY::RZKEY_OEM_3;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_4] = Keyboard::RZKEY::RZKEY_OEM_4;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_5] = Keyboard::RZKEY::RZKEY_OEM_5;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_6] = Keyboard::RZKEY::RZKEY_OEM_6;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_7] = Keyboard::RZKEY::RZKEY_OEM_7;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_8] = Keyboard::RZKEY::RZKEY_OEM_8;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_9] = Keyboard::RZKEY::RZKEY_OEM_9;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_10] = Keyboard::RZKEY::RZKEY_OEM_10;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_OEM_11] = Keyboard::RZKEY::RZKEY_OEM_11;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_EUR_1] = Keyboard::RZKEY::RZKEY_EUR_1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_EUR_2] = Keyboard::RZKEY::RZKEY_EUR_2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_JPN_1] = Keyboard::RZKEY::RZKEY_JPN_1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_JPN_2] = Keyboard::RZKEY::RZKEY_JPN_2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_JPN_3] = Keyboard::RZKEY::RZKEY_JPN_3;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_JPN_4] = Keyboard::RZKEY::RZKEY_JPN_4;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_JPN_5] = Keyboard::RZKEY::RZKEY_JPN_5;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_KOR_1] = Keyboard::RZKEY::RZKEY_KOR_1;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_KOR_2] = Keyboard::RZKEY::RZKEY_KOR_2;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_KOR_3] = Keyboard::RZKEY::RZKEY_KOR_3;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_KOR_4] = Keyboard::RZKEY::RZKEY_KOR_4;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_KOR_5] = Keyboard::RZKEY::RZKEY_KOR_5;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_KOR_6] = Keyboard::RZKEY::RZKEY_KOR_6;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_KOR_7] = Keyboard::RZKEY::RZKEY_KOR_7;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_LOGO] = Keyboard::RZLED::RZLED_LOGO;
	_sKeyboardEnumMap[EChromaSDKKeyboardKey::KK_INVALID] = Keyboard::RZKEY::RZKEY_INVALID;

	// mouse mapping
	_sMouseEnumMap[EChromaSDKMouseLed::ML_SCROLLWHEEL] = Mouse::RZLED2::RZLED2_SCROLLWHEEL;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LOGO] = Mouse::RZLED2::RZLED2_LOGO;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_BACKLIGHT] = Mouse::RZLED2::RZLED2_BACKLIGHT;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LEFT_SIDE1] = Mouse::RZLED2::RZLED2_LEFT_SIDE1;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LEFT_SIDE2] = Mouse::RZLED2::RZLED2_LEFT_SIDE2;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LEFT_SIDE3] = Mouse::RZLED2::RZLED2_LEFT_SIDE3;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LEFT_SIDE4] = Mouse::RZLED2::RZLED2_LEFT_SIDE4;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LEFT_SIDE5] = Mouse::RZLED2::RZLED2_LEFT_SIDE5;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LEFT_SIDE6] = Mouse::RZLED2::RZLED2_LEFT_SIDE6;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_LEFT_SIDE7] = Mouse::RZLED2::RZLED2_LEFT_SIDE7;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_BOTTOM1] = Mouse::RZLED2::RZLED2_BOTTOM1;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_BOTTOM2] = Mouse::RZLED2::RZLED2_BOTTOM2;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_BOTTOM3] = Mouse::RZLED2::RZLED2_BOTTOM3;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_BOTTOM4] = Mouse::RZLED2::RZLED2_BOTTOM4;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_BOTTOM5] = Mouse::RZLED2::RZLED2_BOTTOM5;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_RIGHT_SIDE1] = Mouse::RZLED2::RZLED2_RIGHT_SIDE1;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_RIGHT_SIDE2] = Mouse::RZLED2::RZLED2_RIGHT_SIDE2;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_RIGHT_SIDE3] = Mouse::RZLED2::RZLED2_RIGHT_SIDE3;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_RIGHT_SIDE4] = Mouse::RZLED2::RZLED2_RIGHT_SIDE4;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_RIGHT_SIDE5] = Mouse::RZLED2::RZLED2_RIGHT_SIDE5;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_RIGHT_SIDE6] = Mouse::RZLED2::RZLED2_RIGHT_SIDE6;
	_sMouseEnumMap[EChromaSDKMouseLed::ML_RIGHT_SIDE7] = Mouse::RZLED2::RZLED2_RIGHT_SIDE7;

	FModuleManager::LoadModuleChecked<FChromaSDKPluginModule>("ChromaSDKPlugin");
	ChromaSDKInit();
#endif
}

bool UChromaSDKPluginBPLibrary::IsPlatformWindows()
{
#if PLATFORM_WINDOWS
	return true;
#else
	return false;
#endif
}

int UChromaSDKPluginBPLibrary::GetMaxLeds(const EChromaSDKDevice1DEnum& device)
{
#if PLATFORM_WINDOWS
	switch (device)
	{
	case EChromaSDKDevice1DEnum::DE_ChromaLink:
		return ChromaSDK::ChromaLink::MAX_LEDS;
	case EChromaSDKDevice1DEnum::DE_Headset:
		return ChromaSDK::Headset::MAX_LEDS;
	case EChromaSDKDevice1DEnum::DE_Mousepad:
		return ChromaSDK::Mousepad::MAX_LEDS;
	}
#endif
	return 0;
}

int UChromaSDKPluginBPLibrary::GetMaxRow(const EChromaSDKDevice2DEnum& device)
{
#if PLATFORM_WINDOWS
	switch (device)
	{
	case EChromaSDKDevice2DEnum::DE_Keyboard:
		return ChromaSDK::Keyboard::MAX_ROW;
	case EChromaSDKDevice2DEnum::DE_Keypad:
		return ChromaSDK::Keypad::MAX_ROW;
	case EChromaSDKDevice2DEnum::DE_Mouse:
		return ChromaSDK::Mouse::MAX_ROW;
	}
#endif
	return 0;
}

int UChromaSDKPluginBPLibrary::GetMaxColumn(const EChromaSDKDevice2DEnum& device)
{
	int result = 0;
#if PLATFORM_WINDOWS
	switch (device)
	{
	case EChromaSDKDevice2DEnum::DE_Keyboard:
		return ChromaSDK::Keyboard::MAX_COLUMN;
	case EChromaSDKDevice2DEnum::DE_Keypad:
		return ChromaSDK::Keypad::MAX_COLUMN;
	case EChromaSDKDevice2DEnum::DE_Mouse:
		return ChromaSDK::Mouse::MAX_COLUMN;
	}
#endif
	return result;
}

TArray<FLinearColor> UChromaSDKPluginBPLibrary::CreateColors1D(const EChromaSDKDevice1DEnum& device)
{
	TArray<FLinearColor> colors = TArray<FLinearColor>();
#if PLATFORM_WINDOWS
	int elements = GetMaxLeds(device);
	for (int i = 0; i < elements; ++i)
	{
		FLinearColor color = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
		colors.Add(color);
	}
#endif
	return colors;
}

TArray<FChromaSDKColors> UChromaSDKPluginBPLibrary::CreateColors2D(const EChromaSDKDevice2DEnum& device)
{
	TArray<FChromaSDKColors> result = TArray<FChromaSDKColors>();
#if PLATFORM_WINDOWS
	int maxRows = GetMaxRow(device);
	int maxColumns = GetMaxColumn(device);
	for (int i = 0; i < maxRows; ++i)
	{
		FChromaSDKColors row = FChromaSDKColors();
		for (int j = 0; j < maxColumns; ++j)
		{
			FLinearColor color = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
			row.Colors.Add(color);
		}
		result.Add(row);
	}
#endif
	return result;
}

TArray<FLinearColor> UChromaSDKPluginBPLibrary::CreateRandomColors1D(const EChromaSDKDevice1DEnum& device)
{
	TArray<FLinearColor> colors = TArray<FLinearColor>();
#if PLATFORM_WINDOWS
	int elements = GetMaxLeds(device);
	for (int i = 0; i < elements; ++i)
	{
		float red = FMath::RandRange(0.0f, 1.0f);
		float green = FMath::RandRange(0.0f, 1.0f);
		float blue = FMath::RandRange(0.0f, 1.0f);
		float alpha = 1.0f;
		FLinearColor color = FLinearColor(red, green, blue, alpha);
		colors.Add(color);
	}
#endif
	return colors;
}

TArray<FChromaSDKColors> UChromaSDKPluginBPLibrary::CreateRandomColors2D(const EChromaSDKDevice2DEnum& device)
{
	TArray<FChromaSDKColors> result = TArray<FChromaSDKColors>();
#if PLATFORM_WINDOWS
	int maxRows = GetMaxRow(device);
	int maxColumns = GetMaxColumn(device);
	for (int i = 0; i < maxRows; ++i)
	{
		FChromaSDKColors row = FChromaSDKColors();
		for (int j = 0; j < maxColumns; ++j)
		{
			float red = FMath::RandRange(0.0f, 1.0f);
			float green = FMath::RandRange(0.0f, 1.0f);
			float blue = FMath::RandRange(0.0f, 1.0f);
			float alpha = 1.0f;
			FLinearColor color = FLinearColor(red, green, blue, alpha);
			row.Colors.Add(color);
		}
		result.Add(row);
	}
#endif
	return result;
}

const TArray<FChromaSDKColors>& UChromaSDKPluginBPLibrary::SetKeyboardKeyColor(const EChromaSDKKeyboardKey& key, const FLinearColor& color, TArray<FChromaSDKColors>& colors)
{
#if PLATFORM_WINDOWS
	int maxRow = ChromaSDK::Keyboard::MAX_ROW;
	int maxColumn = ChromaSDK::Keyboard::MAX_COLUMN;
	if (maxRow != colors.Num() ||
		colors.Num() == 0 ||
		maxColumn != colors[0].Colors.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::SetKeyboardKeyColor Array size mismatch row: %d==%d column: %d==%d!"),
			maxRow,
			colors.Num(),
			maxColumn,
			colors.Num() > 0 ? colors[0].Colors.Num() : 0);
		return colors;
	}

	int rzkey = _sKeyboardEnumMap[key];
	if (rzkey != ChromaSDK::Keyboard::RZKEY::RZKEY_INVALID)
	{
		colors[HIBYTE(rzkey)].Colors[LOBYTE(rzkey)] = color;
	}
#endif
	return colors;
}

const TArray<FChromaSDKColors>& UChromaSDKPluginBPLibrary::SetMouseLedColor(const EChromaSDKMouseLed& led, const FLinearColor& color, TArray<FChromaSDKColors>& colors)
{
#if PLATFORM_WINDOWS
	int maxRow = ChromaSDK::Mouse::MAX_ROW;
	int maxColumn = ChromaSDK::Mouse::MAX_COLUMN;
	if (maxRow != colors.Num() ||
		colors.Num() == 0 ||
		maxColumn != colors[0].Colors.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::SetMouseLedColor Array size mismatch row: %d==%d column: %d==%d!"),
			maxRow,
			colors.Num(),
			maxColumn,
			colors.Num() > 0 ? colors[0].Colors.Num() : 0);
		return colors;
	}

	ChromaSDK::Mouse::RZLED2 rzled = _sMouseEnumMap[led];
	colors[HIBYTE(rzled)].Colors[LOBYTE(rzled)] = color;
#endif
	return colors;
}

int UChromaSDKPluginBPLibrary::ChromaSDKInit()
{
#if PLATFORM_WINDOWS
	// Init the SDK
	int result = FChromaSDKPluginModule::Get().ChromaSDKInit();
	_sInitialized = result == 0;
	return result;
#else
	return -1;
#endif
}

int UChromaSDKPluginBPLibrary::ChromaSDKUnInit()
{
#if PLATFORM_WINDOWS
	// unload any 1D animation effects
	for (TObjectIterator<UChromaSDKPluginAnimation1DObject> iterator; iterator; ++iterator)
	{
		UChromaSDKPluginAnimation1DObject* item = *iterator;
		if (item != nullptr &&
			item->IsLoaded())
		{
			item->Unload();
		}
	}

	// unload any 2D animation effects
	for (TObjectIterator<UChromaSDKPluginAnimation2DObject> iterator; iterator; ++iterator)
	{
		UChromaSDKPluginAnimation2DObject* item = *iterator;
		if (item != nullptr &&
			item->IsLoaded())
		{
			item->Unload();
		}
	}

	// UnInit the SDK
	int result = FChromaSDKPluginModule::Get().ChromaSDKUnInit();
	if (result == 0)
	{
		_sInitialized = false;
	}
	return result;
#else
	return -1;
#endif
}

bool UChromaSDKPluginBPLibrary::IsInitialized()
{
#if PLATFORM_WINDOWS
	return _sInitialized;
#else
	return false;
#endif
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

FChromaSDKEffectResult UChromaSDKPluginBPLibrary::ChromaSDKCreateEffectCustom1D(const EChromaSDKDevice1DEnum& device, const TArray<FLinearColor>& colors)
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
		if (maxLeds != colors.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom1D Array size mismatch elements: %d==%d!"),
				maxLeds,
				colors.Num());
			break;
		}
		ChromaSDK::ChromaLink::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxLeds; i++)
		{
			const FLinearColor& color = colors[i];
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
		if (maxLeds != colors.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom1D Array size mismatch elements: %d==%d!"),
				maxLeds,
				colors.Num());
			break;
		}
		ChromaSDK::Headset::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxLeds; i++)
		{
			const FLinearColor& color = colors[i];
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
		if (maxLeds != colors.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin::ChromaSDKCreateEffectCustom1D Array size mismatch elements: %d==%d!"),
				maxLeds,
				colors.Num());
			break;
		}
		ChromaSDK::Mousepad::CUSTOM_EFFECT_TYPE pParam = {};
		for (int i = 0; i < maxLeds; i++)
		{
			const FLinearColor& color = colors[i];
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

void UChromaSDKPluginBPLibrary::PlayAnimation(const FString& animationName, bool loop)
{
#if PLATFORM_WINDOWS
	FString path = FPaths::GameContentDir();
	path += animationName + ".chroma";
	//UE_LOG(LogTemp, Log, TEXT("PlayAnimation: %s"), *path);
	const char* pathArg = TCHAR_TO_ANSI(*path);
	FChromaSDKPluginModule::Get().PlayAnimationName(pathArg, loop);
#endif
}

void UChromaSDKPluginBPLibrary::StopAnimation(const FString& animationName)
{
#if PLATFORM_WINDOWS
	FString path = FPaths::GameContentDir();
	path += animationName + ".chroma";
	//UE_LOG(LogTemp, Log, TEXT("StopAnimation: %s"), *path);
	const char* pathArg = TCHAR_TO_ANSI(*path);
	FChromaSDKPluginModule::Get().StopAnimationName(pathArg);
#endif
}

bool UChromaSDKPluginBPLibrary::IsAnimationPlaying(const FString& animationName)
{
#if PLATFORM_WINDOWS
	FString path = FPaths::GameContentDir();
	path += animationName + ".chroma";
	//UE_LOG(LogTemp, Log, TEXT("IsAnimationPlaying: %s"), *path);
	const char* pathArg = TCHAR_TO_ANSI(*path);
	return FChromaSDKPluginModule::Get().IsAnimationPlayingName(pathArg);
#else
	return false;
#endif
}

void UChromaSDKPluginBPLibrary::PlayAnimations(const TArray<FString>& animationNames, bool loop)
{
	for (int i = 0; i < animationNames.Num(); ++i)
	{
		PlayAnimation(animationNames[i], loop);
	}
}

void UChromaSDKPluginBPLibrary::StopAnimations(const TArray<FString>& animationNames)
{
	for (int i = 0; i < animationNames.Num(); ++i)
	{
		StopAnimation(animationNames[i]);
	}
}

void UChromaSDKPluginBPLibrary::PlayAnimationComposite(const FString& animationName, bool loop)
{
	PlayAnimation(animationName + "_ChromaLink", loop);
	PlayAnimation(animationName + "_Headset", loop);
	PlayAnimation(animationName + "_Keyboard", loop);
	PlayAnimation(animationName + "_Keypad", loop);
	PlayAnimation(animationName + "_Mouse", loop);
	PlayAnimation(animationName + "_Mousepad", loop);
}

void UChromaSDKPluginBPLibrary::StopAnimationComposite(const FString& animationName)
{
	StopAnimation(animationName + "_ChromaLink");
	StopAnimation(animationName + "_Headset");
	StopAnimation(animationName + "_Keyboard");
	StopAnimation(animationName + "_Keypad");
	StopAnimation(animationName + "_Mouse");
	StopAnimation(animationName + "_Mousepad");
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

#include "HideWindowsPlatformTypes.h"

#endif
