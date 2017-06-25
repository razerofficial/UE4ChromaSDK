// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"
#include "HideWindowsPlatformTypes.h"
#include "ChromaSDKPluginTypes.generated.h"

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

UENUM(BlueprintType)
enum class EChromaSDKDeviceEnum : uint8
{
	DE_ChromaLink	UMETA(DisplayName = "ChromaLink"),
	DE_Headset		UMETA(DisplayName = "Headset"),
	DE_Keyboard		UMETA(DisplayName = "Keyboard"),
	DE_Keypad		UMETA(DisplayName = "Keypad"),
	DE_Mouse		UMETA(DisplayName = "Mouse"),
	DE_Mousepad		UMETA(DisplayName = "Mousepad")
};

UENUM(BlueprintType)
enum class EChromaSDKDevice1DEnum : uint8
{
	DE_ChromaLink	UMETA(DisplayName = "ChromaLink"),
	DE_Headset		UMETA(DisplayName = "Headset"),
	DE_Mousepad		UMETA(DisplayName = "Mousepad")
};

UENUM(BlueprintType)
enum class EChromaSDKDevice2DEnum : uint8
{
	DE_Keyboard		UMETA(DisplayName = "Keyboard"),
	DE_Keypad		UMETA(DisplayName = "Keypad"),
	DE_Mouse		UMETA(DisplayName = "Mouse"),
};

UENUM(BlueprintType)
enum class EChromaSDKKeyboardKey : uint8
{
	KK_ESC                           UMETA(DisplayName = "Esc (VK_ESCAPE)"),
	KK_F1                            UMETA(DisplayName = "F1 (VK_F1)"),
	KK_F2                            UMETA(DisplayName = "F2 (VK_F2)"),
	KK_F3                            UMETA(DisplayName = "F3 (VK_F3)"),
	KK_F4                            UMETA(DisplayName = "F4 (VK_F4)"),
	KK_F5                            UMETA(DisplayName = "F5 (VK_F5)"),
	KK_F6                            UMETA(DisplayName = "F6 (VK_F6)"),
	KK_F7                            UMETA(DisplayName = "F7 (VK_F7)"),
	KK_F8                            UMETA(DisplayName = "F8 (VK_F8)"),
	KK_F9                            UMETA(DisplayName = "F9 (VK_F9)"),
	KK_F10                           UMETA(DisplayName = "F10 (VK_F10)"),
	KK_F11                           UMETA(DisplayName = "F11 (VK_F11)"),
	KK_F12                           UMETA(DisplayName = "F12 (VK_F12)"),
	KK_1                             UMETA(DisplayName = "1 (VK_1)"),
	KK_2                             UMETA(DisplayName = "2 (VK_2)"),
	KK_3                             UMETA(DisplayName = "3 (VK_3)"),
	KK_4                             UMETA(DisplayName = "4 (VK_4)"),
	KK_5                             UMETA(DisplayName = "5 (VK_5)"),
	KK_6                             UMETA(DisplayName = "6 (VK_6)"),
	KK_7                             UMETA(DisplayName = "7 (VK_7)"),
	KK_8                             UMETA(DisplayName = "8 (VK_8)"),
	KK_9                             UMETA(DisplayName = "9 (VK_9)"),
	KK_0                             UMETA(DisplayName = "0 (VK_0)"),
	KK_A                             UMETA(DisplayName = "A (VK_A)"),
	KK_B                             UMETA(DisplayName = "B (VK_B)"),
	KK_C                             UMETA(DisplayName = "C (VK_C)"),
	KK_D                             UMETA(DisplayName = "D (VK_D)"),
	KK_E                             UMETA(DisplayName = "E (VK_E)"),
	KK_F                             UMETA(DisplayName = "F (VK_F)"),
	KK_G                             UMETA(DisplayName = "G (VK_G)"),
	KK_H                             UMETA(DisplayName = "H (VK_H)"),
	KK_I                             UMETA(DisplayName = "I (VK_I)"),
	KK_J                             UMETA(DisplayName = "J (VK_J)"),
	KK_K                             UMETA(DisplayName = "K (VK_K)"),
	KK_L                             UMETA(DisplayName = "L (VK_L)"),
	KK_M                             UMETA(DisplayName = "M (VK_M)"),
	KK_N                             UMETA(DisplayName = "N (VK_N)"),
	KK_O                             UMETA(DisplayName = "O (VK_O)"),
	KK_P                             UMETA(DisplayName = "P (VK_P)"),
	KK_Q                             UMETA(DisplayName = "Q (VK_Q)"),
	KK_R                             UMETA(DisplayName = "R (VK_R)"),
	KK_S                             UMETA(DisplayName = "S (VK_S)"),
	KK_T                             UMETA(DisplayName = "T (VK_T)"),
	KK_U                             UMETA(DisplayName = "U (VK_U)"),
	KK_V                             UMETA(DisplayName = "V (VK_V)"),
	KK_W                             UMETA(DisplayName = "W (VK_W)"),
	KK_X                             UMETA(DisplayName = "X (VK_X)"),
	KK_Y                             UMETA(DisplayName = "Y (VK_Y)"),
	KK_Z                             UMETA(DisplayName = "Z (VK_Z)"),
	KK_NUMLOCK                       UMETA(DisplayName = "Numlock (VK_NUMLOCK)"),
	KK_NUMPAD0                       UMETA(DisplayName = "Numpad 0 (VK_NUMPAD0)"),
	KK_NUMPAD1                       UMETA(DisplayName = "Numpad 1 (VK_NUMPAD1)"),
	KK_NUMPAD2                       UMETA(DisplayName = "Numpad 2 (VK_NUMPAD2)"),
	KK_NUMPAD3                       UMETA(DisplayName = "Numpad 3 (VK_NUMPAD3)"),
	KK_NUMPAD4                       UMETA(DisplayName = "Numpad 4 (VK_NUMPAD4)"),
	KK_NUMPAD5                       UMETA(DisplayName = "Numpad 5 (VK_NUMPAD5)"),
	KK_NUMPAD6                       UMETA(DisplayName = "Numpad 6 (VK_NUMPAD6)"),
	KK_NUMPAD7                       UMETA(DisplayName = "Numpad 7 (VK_NUMPAD7)"),
	KK_NUMPAD8                       UMETA(DisplayName = "Numpad 8 (VK_NUMPAD8)"),
	KK_NUMPAD9                       UMETA(DisplayName = "Numpad 9 (VK_ NUMPAD9)"),
	KK_NUMPAD_DIVIDE                 UMETA(DisplayName = "Divide (VK_DIVIDE)"),
	KK_NUMPAD_MULTIPLY               UMETA(DisplayName = "Multiply (VK_MULTIPLY)"),
	KK_NUMPAD_SUBTRACT               UMETA(DisplayName = "Subtract (VK_SUBTRACT)"),
	KK_NUMPAD_ADD                    UMETA(DisplayName = "Add (VK_ADD)"),
	KK_NUMPAD_ENTER                  UMETA(DisplayName = "Enter (VK_RETURN - Extended)"),
	KK_NUMPAD_DECIMAL                UMETA(DisplayName = "Decimal (VK_DECIMAL)"),
	KK_PRINTSCREEN                   UMETA(DisplayName = "Print Screen (VK_PRINT)"),
	KK_SCROLL                        UMETA(DisplayName = "Scroll Lock (VK_SCROLL)"),
	KK_PAUSE                         UMETA(DisplayName = "Pause (VK_PAUSE)"),
	KK_INSERT                        UMETA(DisplayName = "Insert (VK_INSERT)"),
	KK_HOME                          UMETA(DisplayName = "Home (VK_HOME)"),
	KK_PAGEUP                        UMETA(DisplayName = "Page Up (VK_PRIOR)"),
	KK_DELETE                        UMETA(DisplayName = "Delete (VK_DELETE)"),
	KK_END                           UMETA(DisplayName = "End (VK_END)"),
	KK_PAGEDOWN                      UMETA(DisplayName = "Page Down (VK_NEXT)"),
	KK_UP                            UMETA(DisplayName = "Up (VK_UP)"),
	KK_LEFT                          UMETA(DisplayName = "Left (VK_LEFT)"),
	KK_DOWN                          UMETA(DisplayName = "Down (VK_DOWN)"),
	KK_RIGHT                         UMETA(DisplayName = "Right (VK_RIGHT)"),
	KK_TAB                           UMETA(DisplayName = "Tab (VK_TAB)"),
	KK_CAPSLOCK                      UMETA(DisplayName = "Caps Lock(VK_CAPITAL)"),
	KK_BACKSPACE                     UMETA(DisplayName = "Backspace (VK_BACK)"),
	KK_ENTER                         UMETA(DisplayName = "Enter (VK_RETURN)"),
	KK_LCTRL                         UMETA(DisplayName = "Left Control(VK_LCONTROL)"),
	KK_LWIN                          UMETA(DisplayName = "Left Window (VK_LWIN)"),
	KK_LALT                          UMETA(DisplayName = "Left Alt (VK_LMENU)"),
	KK_SPACE                         UMETA(DisplayName = "Spacebar (VK_SPACE)"),
	KK_RALT                          UMETA(DisplayName = "Right Alt (VK_RMENU)"),
	KK_FN                            UMETA(DisplayName = "Function key"),
	KK_RMENU                         UMETA(DisplayName = "Right Menu (VK_APPS)"),
	KK_RCTRL                         UMETA(DisplayName = "Right Control (VK_RCONTROL)"),
	KK_LSHIFT                        UMETA(DisplayName = "Left Shift (VK_LSHIFT)"),
	KK_RSHIFT                        UMETA(DisplayName = "Right Shift (VK_RSHIFT)"),
	KK_MACRO1                        UMETA(DisplayName = "Macro Key 1"),
	KK_MACRO2                        UMETA(DisplayName = "Macro Key 2"),
	KK_MACRO3                        UMETA(DisplayName = "Macro Key 3"),
	KK_MACRO4                        UMETA(DisplayName = "Macro Key 4"),
	KK_MACRO5                        UMETA(DisplayName = "Macro Key 5"),
	KK_OEM_1                         UMETA(DisplayName = "~ (tilde/半角/全角) (VK_OEM_3)"),
	KK_OEM_2                         UMETA(DisplayName = "-- (minus) (VK_OEM_MINUS)"),
	KK_OEM_3                         UMETA(DisplayName = "= (equal) (VK_OEM_PLUS)"),
	KK_OEM_4                         UMETA(DisplayName = "[ (left sqaure bracket) (VK_OEM_4)"),
	KK_OEM_5                         UMETA(DisplayName = "] (right square bracket) (VK_OEM_6)"),
	KK_OEM_6                         UMETA(DisplayName = "\ (backslash) (VK_OEM_5)"),
	KK_OEM_7                         UMETA(DisplayName = "; (semi-colon) (VK_OEM_1)"),
	KK_OEM_8                         UMETA(DisplayName = "' (apostrophe) (VK_OEM_7)"),
	KK_OEM_9                         UMETA(DisplayName = ", (comma) (VK_OEM_COMMA)"),
	KK_OEM_10                        UMETA(DisplayName = ". (period) (VK_OEM_PERIOD)"),
	KK_OEM_11                        UMETA(DisplayName = "/ (forward slash) (VK_OEM_2)"),
	KK_EUR_1                         UMETA(DisplayName = ""#" (VK_OEM_5)"),
	KK_EUR_2                         UMETA(DisplayName = "\ (VK_OEM_102)"),
	KK_JPN_1                         UMETA(DisplayName = "¥ (0xFF)"),
	KK_JPN_2                         UMETA(DisplayName = "\ (0xC1)"),
	KK_JPN_3                         UMETA(DisplayName = "無変換 (VK_OEM_PA1)"),
	KK_JPN_4                         UMETA(DisplayName = "変換 (0xFF)"),
	KK_JPN_5                         UMETA(DisplayName = "ひらがな/カタカナ (0xFF)"),
	KK_KOR_1                         UMETA(DisplayName = "| (0xFF)"),
	KK_KOR_2                         UMETA(DisplayName = "(VK_OEM_5)"),
	KK_KOR_3                         UMETA(DisplayName = "(VK_OEM_102)"),
	KK_KOR_4                         UMETA(DisplayName = "(0xC1)"),
	KK_KOR_5                         UMETA(DisplayName = "(VK_OEM_PA1)"),
	KK_KOR_6                         UMETA(DisplayName = "한/영 (0xFF)"),
	KK_KOR_7                         UMETA(DisplayName = "(0xFF)"),
	KK_INVALID                       UMETA(DisplayName = "Invalid keys.")
};

UENUM(BlueprintType)
enum class EChromaSDKMouseLed : uint8
{
	ML_SCROLLWHEEL		UMETA(DisplayName = "Scroll Wheel LED"),
	ML_LOGO				UMETA(DisplayName = "Logo LED"),
	ML_BACKLIGHT		UMETA(DisplayName = "Backlight LED"),
	ML_LEFT_SIDE1		UMETA(DisplayName = "Left LED 1"),
	ML_LEFT_SIDE2		UMETA(DisplayName = "Left LED 2"),
	ML_LEFT_SIDE3		UMETA(DisplayName = "Left LED 3"),
	ML_LEFT_SIDE4		UMETA(DisplayName = "Left LED 4"),
	ML_LEFT_SIDE5		UMETA(DisplayName = "Left LED 5"),
	ML_LEFT_SIDE6		UMETA(DisplayName = "Left LED 6"),
	ML_LEFT_SIDE7		UMETA(DisplayName = "Left LED 7"),
	ML_BOTTOM1			UMETA(DisplayName = "Bottom LED 1"),
	ML_BOTTOM2			UMETA(DisplayName = "Bottom LED 2"),
	ML_BOTTOM3			UMETA(DisplayName = "Bottom LED 3"),
	ML_BOTTOM4			UMETA(DisplayName = "Bottom LED 4"),
	ML_BOTTOM5			UMETA(DisplayName = "Bottom LED 5"),
	ML_RIGHT_SIDE1		UMETA(DisplayName = "Right LED 1"),
	ML_RIGHT_SIDE2		UMETA(DisplayName = "Right LED 2"),
	ML_RIGHT_SIDE3		UMETA(DisplayName = "Right LED 3"),
	ML_RIGHT_SIDE4		UMETA(DisplayName = "Right LED 4"),
	ML_RIGHT_SIDE5		UMETA(DisplayName = "Right LED 5"),
	ML_RIGHT_SIDE6		UMETA(DisplayName = "Right LED 6"),
	ML_RIGHT_SIDE7		UMETA(DisplayName = "Right LED 7")
};

USTRUCT(BlueprintType)
struct FChromaSDKGuid
{
	GENERATED_BODY()

#if PLATFORM_WINDOWS
	RZEFFECTID Data;
#endif

	//Constructor
	FChromaSDKGuid()
	{
#if PLATFORM_WINDOWS
		Data = RZEFFECTID();
		Data.Data1 = 0;
		Data.Data2 = 0;
		Data.Data3 = 0;
		for (int i = 0; i < 8; ++i)
		{
			Data.Data4[i] = 0;
		}
#endif
	}
};


USTRUCT(BlueprintType)
struct FChromaSDKEffectResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int Result;

	UPROPERTY(BlueprintReadOnly)
	FChromaSDKGuid EffectId;

	//Constructor
	FChromaSDKEffectResult()
	{
		Result = 0;
		EffectId = FChromaSDKGuid();
	}
};

USTRUCT(BlueprintType)
struct FChromaSDKColors
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FLinearColor> Colors;

	//Constructor
	FChromaSDKColors()
	{
	}
};

USTRUCT(BlueprintType)
struct FChromaSDKColorFrame1D
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FLinearColor> Colors;

	//Constructor
	FChromaSDKColorFrame1D()
	{
	}
};

USTRUCT(BlueprintType)
struct FChromaSDKColorFrame2D
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FChromaSDKColors> Colors;

	//Constructor
	FChromaSDKColorFrame2D()
	{
	}
};
