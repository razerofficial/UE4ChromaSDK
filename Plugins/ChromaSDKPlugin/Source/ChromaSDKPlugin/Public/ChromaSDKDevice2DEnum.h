// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 17)
#include "ChromaSDKPlugin.h"
#endif
#include "Engine.h"
#include "ChromaSDKDevice2DEnum.generated.h"

UENUM(BlueprintType)
enum class EChromaSDKDevice2DEnum : uint8
{
	DE_Keyboard		UMETA(DisplayName = "Keyboard"),
	DE_Keypad		UMETA(DisplayName = "Keypad"),
	DE_Mouse		UMETA(DisplayName = "Mouse"),
};
