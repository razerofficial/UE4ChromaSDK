// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPlugin.h"
#include "Engine.h"
#include "ChromaSDKDeviceTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EChromaSDKDeviceTypeEnum : uint8
{
	DE_1D			UMETA(DisplayName = "1D"),
	DE_2D			UMETA(DisplayName = "2D")
};
