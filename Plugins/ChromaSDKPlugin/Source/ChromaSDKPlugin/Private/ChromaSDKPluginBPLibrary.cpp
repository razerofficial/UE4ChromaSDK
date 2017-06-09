// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPluginBPLibrary.h"
#include "ChromaSDKPlugin.h"

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
