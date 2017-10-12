// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChromaSDKPlugin.h"
#include "Engine.h"
#include "ChromaSDKPluginTypes.h"
#if PLATFORM_WINDOWS
#include <map>
#endif
#include "ChromaSDKPluginBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UCLASS()
class CHROMASDKPLUGIN_API UChromaSDKPluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsPlatformWindows", Keywords = "Returns true on the Windows Platform"), Category = "ChromaSDK")
	static bool IsPlatformWindows();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMaxLeds", Keywords = "Get the max led size for the device"), Category = "ChromaSDK")
	static int GetMaxLeds(const EChromaSDKDevice1DEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMaxRow", Keywords = "Get the max row size for the device"), Category = "ChromaSDK")
	static int GetMaxRow(const EChromaSDKDevice2DEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMaxColumn", Keywords = "Get the max column size for the device"), Category = "ChromaSDK")
	static int GetMaxColumn(const EChromaSDKDevice2DEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateColors1D", Keywords = "Create a blank one-dimensional color array"), Category = "ChromaSDK")
	static TArray<FLinearColor> CreateColors1D(const EChromaSDKDevice1DEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateColors2D", Keywords = "Create a blank two-dimensional color array"), Category = "ChromaSDK")
	static TArray<FChromaSDKColors> CreateColors2D(const EChromaSDKDevice2DEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateRandomColors1D", Keywords = "Create a random one-dimensional color array"), Category = "ChromaSDK")
	static TArray<FLinearColor> CreateRandomColors1D(const EChromaSDKDevice1DEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateRandomColors2D", Keywords = "Create a random two-dimensional color array"), Category = "ChromaSDK")
	static TArray<FChromaSDKColors> CreateRandomColors2D(const EChromaSDKDevice2DEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetKeyboardKeyColor", Keywords = "Set the key to the supplied color"), Category = "ChromaSDK")
	static const TArray<FChromaSDKColors>& SetKeyboardKeyColor(const EChromaSDKKeyboardKey& key, const FLinearColor& color, UPARAM(ref) TArray<FChromaSDKColors>& colors);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetMouseLedColor", Keywords = "Set the led to the supplied color"), Category = "ChromaSDK")
	static const TArray<FChromaSDKColors>& SetMouseLedColor(const EChromaSDKMouseLed& led, const FLinearColor& color, UPARAM(ref) TArray<FChromaSDKColors>& colors);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsInitialized", Keywords = "Return true if the blueprint library is initialized"), Category = "ChromaSDK")
	static bool IsInitialized();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Init", Keywords = "Initialize the ChromaSDK"), Category = "ChromaSDK")
	static int ChromaSDKInit();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnInit", Keywords = "Uninitialize the ChromaSDK"), Category = "ChromaSDK")
	static int ChromaSDKUnInit();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DebugGetString", Keywords = "Get Debug String From Effect Id"), Category = "ChromaSDK")
	static FString DebugToString(const FChromaSDKGuid& effectId);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateEffectNone", Keywords = "Clear the color effect"), Category = "ChromaSDK")
	static FChromaSDKEffectResult ChromaSDKCreateEffectNone(const EChromaSDKDeviceEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateEffectStatic", Keywords = "Create a static color effect"), Category = "ChromaSDK")
	static FChromaSDKEffectResult ChromaSDKCreateEffectStatic(const EChromaSDKDeviceEnum& device, const FLinearColor& color);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateEffectCustom1D", Keywords = "Create a custom color effect using one-dimensional array"), Category = "ChromaSDK")
	static FChromaSDKEffectResult ChromaSDKCreateEffectCustom1D(const EChromaSDKDevice1DEnum& device, const TArray<FLinearColor>& colors);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateEffectCustom2D", Keywords = "Create a custom color effect using two-dimensional array"), Category = "ChromaSDK")
	static FChromaSDKEffectResult ChromaSDKCreateEffectCustom2D(const EChromaSDKDevice2DEnum& device, const TArray<FChromaSDKColors>& colors);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetEffect", Keywords = "Set Effect with Effect Id"), Category = "ChromaSDK")
	static int ChromaSDKSetEffect(const FChromaSDKGuid& effectId);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DeleteEffect", Keywords = "Delete Effect with Effect Id"), Category = "ChromaSDK")
	static int ChromaSDKDeleteEffect(const FChromaSDKGuid& effectId);

	// library methods

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAnimationId", Keywords = "Get the animation id"), Category = "ChromaSDK")
	static int GetAnimationId(const FString& animationName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LoadAnimation", Keywords = "Load the *.chroma Animation"), Category = "ChromaSDK")
	static void LoadAnimation(const int animationId);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LoadAnimationName", Keywords = "Load the *.chroma Animation"), Category = "ChromaSDK")
	static void LoadAnimationName(const FString& animationName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CloseAnimation", Keywords = "Close the *.chroma Animation"), Category = "ChromaSDK")
	static void CloseAnimation(const int animationId);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CloseAnimationName", Keywords = "Close the *.chroma Animation"), Category = "ChromaSDK")
	static void CloseAnimationName(const FString& animationName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnloadAnimation", Keywords = "Unload the *.chroma Animation"), Category = "ChromaSDK")
	static void UnloadAnimation(const int animationId);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnloadAnimationName", Keywords = "Unload the *.chroma Animation"), Category = "ChromaSDK")
	static void UnloadAnimationName(const FString& animationName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PlayAnimation", Keywords = "Play the *.chroma Animation"), Category = "ChromaSDK")
	static void PlayAnimation(const FString& animationName, bool loop);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopAnimation", Keywords = "Stop the *.chroma Animation"), Category = "ChromaSDK")
	static void StopAnimation(const FString& animationName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopAnimationType", Keywords = "Stop playing the Animation type"), Category = "ChromaSDK")
	static void StopAnimationType(const EChromaSDKDeviceEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsAnimationPlaying", Keywords = "Is the *.chroma Animation playing?"), Category = "ChromaSDK")
	static bool IsAnimationPlaying(const FString& animationName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsAnimationTypePlaying", Keywords = "Is the Animation type playing?"), Category = "ChromaSDK")
	static bool IsAnimationTypePlaying(const EChromaSDKDeviceEnum& device);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PlayAnimations", Keywords = "Play an array of *.chroma Animations"), Category = "ChromaSDK")
	static void PlayAnimations(const TArray<FString>& animationNames, bool loop);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopAnimations", Keywords = "Stop an array *.chroma Animations"), Category = "ChromaSDK")
	static void StopAnimations(const TArray<FString>& animationNames);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PlayAnimationComposite", Keywords = "Play a set of *_Device.chroma Animations"), Category = "ChromaSDK")
	static void PlayAnimationComposite(const FString& animationName, bool loop);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StopAnimationComposite", Keywords = "Stop a set of *_Device.chroma Animations"), Category = "ChromaSDK")
	static void StopAnimationComposite(const FString& animationName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetKeyColor", Keywords = "Set the animation frame's key to the supplied color"), Category = "ChromaSDK")
	static void SetKeyColor(int animationId, int frameIndex, const EChromaSDKKeyboardKey& key, const FLinearColor& color);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetKeyColorName", Keywords = "Set the animation frame's key to the supplied color"), Category = "ChromaSDK")
	static void SetKeyColorName(const FString& animationName, const int frameIndex, const EChromaSDKKeyboardKey& key, const FLinearColor& color);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetFrameCount", Keywords = "Get the .chroma animation frame count"), Category = "ChromaSDK")
	static int GetFrameCount(const int animationId);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetFrameCountName", Keywords = "Get the .chroma animation frame count"), Category = "ChromaSDK")
	static int GetFrameCountName(const FString& animationName);

#if PLATFORM_WINDOWS
private:
	static void ToString(const RZEFFECTID& effectId, FString& effectString);
	static void ToEffect(const FString& effectString, RZEFFECTID& effectId);
	static std::map<EChromaSDKKeyboardKey, int> _sKeyboardEnumMap;
	static std::map<EChromaSDKMouseLed, ChromaSDK::Mouse::RZLED2> _sMouseEnumMap;

	static bool _sInitialized;
#endif
};
