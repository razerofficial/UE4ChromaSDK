// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 17)
#include "ChromaSDKPlugin.h"
#endif
#include "Engine.h"
#include "ChromaSDKDevice1DEnum.generated.h"

UENUM(BlueprintType)
enum class EChromaSDKDevice1DEnum : uint8
{
	DE_ChromaLink	UMETA(DisplayName = "ChromaLink"),
	DE_Headset		UMETA(DisplayName = "Headset"),
	DE_Mousepad		UMETA(DisplayName = "Mousepad")
};
