// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

//expose HMODULE
#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
#include "HideWindowsPlatformTypes.h"
#endif

class FChromaSDKPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#if PLATFORM_WINDOWS
	HMODULE _mLibraryChroma = nullptr;
#endif
};