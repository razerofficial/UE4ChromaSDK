// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE4ChromaSDK.h"
#include "Runtime/Launch/Resources/Version.h"
#if (ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION > 12)
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#else
#include "GameFramework/GameMode.h"
#endif
#include "UE4ChromaSDKGameModeBase.generated.h"

#if (ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION > 12)
// (support above 4.12)
// class UE4CHROMASDK_API AUE4ChromaSDKGameModeBase : public AGameModeBase
#else
#pragma message("UE4ChromaSDKGameModeBase.h: Use UE4CHROMASDK_API AUE4ChromaSDKGameModeBase : public AGameMode")
// (support 4.12 or below)
// class UE4CHROMASDK_API AUE4ChromaSDKGameModeBase : public AGameMode
#endif

/**
 * 
 */
UCLASS()
class UE4CHROMASDK_API AUE4ChromaSDKGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
};
