// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 17)
#include "ChromaSDKPlugin.h"
#endif
#include "Engine.h"
#include "ChromaSDKDeviceTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EChromaSDKDeviceTypeEnum : uint8
{
	DE_1D			UMETA(DisplayName = "1D"),
	DE_2D			UMETA(DisplayName = "2D")
};
