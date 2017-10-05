// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UE4ChromaSDK.h"
#include "Runtime/Launch/Resources/Version.h"
#if (ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION > 14)
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#else
#include "GameFramework/GameMode.h"
#endif
#include "UE4ChromaSDKGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
//class UE4CHROMASDK_API AUE4ChromaSDKGameModeBase : public AGameMode //(support 4.12)___HACK_UE4_WANTS_GAME_MODE
class UE4CHROMASDK_API AUE4ChromaSDKGameModeBase : public AGameModeBase //(support above 4.12)___HACK_UE4_WANTS_BASE_GAME_MODE
{
	GENERATED_BODY()
};
