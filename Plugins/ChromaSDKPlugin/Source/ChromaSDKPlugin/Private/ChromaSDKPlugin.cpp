// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ChromaSDKPlugin.h"
#include "AnimationBase.h"
#include "Animation1D.h"
#include "Animation2D.h"
#include "ChromaThread.h"

#define LOCTEXT_NAMESPACE "FChromaSDKPluginModule"

#if PLATFORM_WINDOWS

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif

typedef unsigned char byte;
#define ANIMATION_VERSION 1

using namespace ChromaSDK;
using namespace ChromaSDK::ChromaLink;
using namespace ChromaSDK::Headset;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Mousepad;
using namespace std;

bool FChromaSDKPluginModule::ValidateGetProcAddress(bool condition, FString methodName)
{
	if (condition)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin failed to load %s!"), *methodName);
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin loaded %s."), *methodName);
	}
	return condition;
}

#endif

void FChromaSDKPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if PLATFORM_WINDOWS
	_mInitialized = false;
	_mAnimationId = 0;
	_mAnimationMapID.clear();
	_mAnimations.clear();
	_mPlayMap1D.clear();
	_mPlayMap2D.clear();

	_mLibraryChroma = LoadLibrary(CHROMASDKDLL);
	if (_mLibraryChroma == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin failed to load!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin loaded."));

	// GetProcAddress will throw 4191 because it's an unsafe type cast
#pragma warning(disable: 4191)
	_mMethodInit = (CHROMA_SDK_INIT)GetProcAddress(_mLibraryChroma, "Init");
	if (ValidateGetProcAddress(_mMethodInit == nullptr, FString("Init")))
	{
		return;
	}
	_mMethodUnInit = (CHROMA_SDK_UNINIT)GetProcAddress(_mLibraryChroma, "UnInit");
	if (ValidateGetProcAddress(_mMethodUnInit == nullptr, FString("UnInit")))
	{
		return;
	}
	_mMethodQueryDevice = (CHROMA_SDK_QUERY_DEVICE)GetProcAddress(_mLibraryChroma, "QueryDevice");
	if (ValidateGetProcAddress(_mMethodQueryDevice == nullptr, FString("QueryDevice")))
	{
		return;
	}
	_mMethodUnInit = (CHROMA_SDK_UNINIT)GetProcAddress(_mLibraryChroma, "UnInit");
	if (ValidateGetProcAddress(_mMethodUnInit == nullptr, FString("UnInit")))
	{
		return;
	}

	_mMethodCreateChromaLinkEffect = (CHROMA_SDK_CREATE_CHROMA_LINK_EFFECT)GetProcAddress(_mLibraryChroma, "CreateChromaLinkEffect");
	if (ValidateGetProcAddress(_mMethodCreateChromaLinkEffect == nullptr, FString("CreateChromaLinkEffect")))
	{
		return;
	}
	_mMethodCreateHeadsetEffect = (CHROMA_SDK_CREATE_HEADSET_EFFECT)GetProcAddress(_mLibraryChroma, "CreateHeadsetEffect");
	if (ValidateGetProcAddress(_mMethodCreateHeadsetEffect == nullptr, FString("CreateHeadsetEffect")))
	{
		return;
	}
	_mMethodCreateKeyboardEffect = (CHROMA_SDK_CREATE_KEYBOARD_EFFECT)GetProcAddress(_mLibraryChroma, "CreateKeyboardEffect");
	if (ValidateGetProcAddress(_mMethodCreateKeyboardEffect == nullptr, FString("CreateKeyboardEffect")))
	{
		return;
	}
	_mMethodCreateMouseEffect = (CHROMA_SDK_CREATE_MOUSE_EFFECT)GetProcAddress(_mLibraryChroma, "CreateMouseEffect");
	if (ValidateGetProcAddress(_mMethodCreateMouseEffect == nullptr, FString("CreateMouseEffect")))
	{
		return;
	}
	_mMethodCreateMousepadEffect = (CHROMA_SDK_CREATE_MOUSEPAD_EFFECT)GetProcAddress(_mLibraryChroma, "CreateMousepadEffect");
	if (ValidateGetProcAddress(_mMethodCreateMousepadEffect == nullptr, FString("CreateMousepadEffect")))
	{
		return;
	}
	_mMethodCreateKeypadEffect = (CHROMA_SDK_CREATE_KEYPAD_EFFECT)GetProcAddress(_mLibraryChroma, "CreateKeypadEffect");
	if (ValidateGetProcAddress(_mMethodCreateKeypadEffect == nullptr, FString("CreateKeypadEffect")))
	{
		return;
	}

	_mMethodCreateEffect = (CHROMA_SDK_CREATE_EFFECT)GetProcAddress(_mLibraryChroma, "CreateEffect");
	if (ValidateGetProcAddress(_mMethodCreateEffect == nullptr, FString("CreateEffect")))
	{
		return;
	}
	_mMethodSetEffect = (CHROMA_SDK_SET_EFFECT)GetProcAddress(_mLibraryChroma, "SetEffect");
	if (ValidateGetProcAddress(_mMethodSetEffect == nullptr, FString("SetEffect")))
	{
		return;
	}
	_mMethodDeleteEffect = (CHROMA_SDK_DELETE_EFFECT)GetProcAddress(_mLibraryChroma, "DeleteEffect");
	if (ValidateGetProcAddress(_mMethodDeleteEffect == nullptr, FString("DeleteEffect")))
	{
		return;
	}
#pragma warning(default: 4191)

	UChromaSDKPluginBPLibrary::ChromaSDKInit();

	ChromaThread::Instance()->Start();
#endif
}

void FChromaSDKPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
#if PLATFORM_WINDOWS
	ChromaThread::Instance()->Stop();

	UChromaSDKPluginBPLibrary::ChromaSDKUnInit();

	if (_mLibraryChroma)
	{
		FreeLibrary(_mLibraryChroma);
		_mLibraryChroma = nullptr;
	}

	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin unloaded."));
#endif
}

#if PLATFORM_WINDOWS

bool FChromaSDKPluginModule::IsInitialized()
{
	return _mInitialized;
}

int FChromaSDKPluginModule::ChromaSDKInit()
{
	if (_mMethodInit == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin Init method is not set!"));
		return -1;
	}

	int result = _mMethodInit();
	_mInitialized = true;
	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin [INITIALIZED] result=%d"), result);
	return result;
}

int FChromaSDKPluginModule::ChromaSDKUnInit()
{
	if (_mMethodUnInit == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin UnInit method is not set!"));
		return -1;
	}

	int result = _mMethodUnInit();
	_mInitialized = false;
	UE_LOG(LogTemp, Log, TEXT("ChromaSDKPlugin [UNINITIALIZED] result=%d"), result);
	return result;
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateEffect(RZDEVICEID deviceId, ChromaSDK::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateEffect(deviceId, effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateChromaLinkEffect(ChromaSDK::ChromaLink::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateChromaLinkEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateChromaLinkEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateChromaLinkEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateHeadsetEffect(ChromaSDK::Headset::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateHeadsetEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateHeadsetEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateHeadsetEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateKeyboardEffect(ChromaSDK::Keyboard::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateKeyboardEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateKeyboardEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateKeyboardEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateKeypadEffect(ChromaSDK::Keypad::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateKeypadEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateKeypadEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateKeypadEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateMouseEffect(ChromaSDK::Mouse::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateMouseEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateMouseEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateMouseEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKCreateMousepadEffect(ChromaSDK::Mousepad::EFFECT_TYPE effect, PRZPARAM pParam, RZEFFECTID* pEffectId)
{
	if (_mMethodCreateMousepadEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin CreateMousepadEffect method is not set!"));
		return -1;
	}

	return _mMethodCreateMousepadEffect(effect, pParam, pEffectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKSetEffect(RZEFFECTID effectId)
{
	if (_mMethodSetEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin SetEffect method is not set!"));
		return -1;
	}

	return _mMethodSetEffect(effectId);
}

RZRESULT FChromaSDKPluginModule::ChromaSDKDeleteEffect(RZEFFECTID effectId)
{
	if (_mMethodDeleteEffect == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ChromaSDKPlugin DeleteEffect method is not set!"));
		return -1;
	}

	return _mMethodDeleteEffect(effectId);
}

int FChromaSDKPluginModule::ToBGR(const FLinearColor& color)
{
	int red = color.R * 255;
	int green = color.G * 255;
	int blue = color.B * 255;
	return RGB(red, green, blue);
}

FLinearColor FChromaSDKPluginModule::ToLinearColor(int color)
{
	float red = GetRValue(color) / 255.0f;
	float green = GetGValue(color) / 255.0f;
	float blue = GetBValue(color) / 255.0f;
	return FLinearColor(red, green, blue, 1.0f);
}

int FChromaSDKPluginModule::GetMaxLeds(const EChromaSDKDevice1DEnum& device)
{
#if PLATFORM_WINDOWS
	switch (device)
	{
	case EChromaSDKDevice1DEnum::DE_ChromaLink:
		return ChromaSDK::ChromaLink::MAX_LEDS;
	case EChromaSDKDevice1DEnum::DE_Headset:
		return ChromaSDK::Headset::MAX_LEDS;
	case EChromaSDKDevice1DEnum::DE_Mousepad:
		return ChromaSDK::Mousepad::MAX_LEDS;
	}
#endif
	return 0;
}

int FChromaSDKPluginModule::GetMaxRow(const EChromaSDKDevice2DEnum& device)
{
#if PLATFORM_WINDOWS
	switch (device)
	{
	case EChromaSDKDevice2DEnum::DE_Keyboard:
		return ChromaSDK::Keyboard::MAX_ROW;
	case EChromaSDKDevice2DEnum::DE_Keypad:
		return ChromaSDK::Keypad::MAX_ROW;
	case EChromaSDKDevice2DEnum::DE_Mouse:
		return ChromaSDK::Mouse::MAX_ROW;
	}
#endif
	return 0;
}

int FChromaSDKPluginModule::GetMaxColumn(const EChromaSDKDevice2DEnum& device)
{
	int result = 0;
#if PLATFORM_WINDOWS
	switch (device)
	{
	case EChromaSDKDevice2DEnum::DE_Keyboard:
		return ChromaSDK::Keyboard::MAX_COLUMN;
	case EChromaSDKDevice2DEnum::DE_Keypad:
		return ChromaSDK::Keypad::MAX_COLUMN;
	case EChromaSDKDevice2DEnum::DE_Mouse:
		return ChromaSDK::Mouse::MAX_COLUMN;
	}
#endif
	return result;
}

int FChromaSDKPluginModule::OpenAnimation(const char* path)
{
	AnimationBase* animation = nullptr;

	//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: %s"), path);

	FILE* stream = nullptr;
	if (0 != fopen_s(&stream, path, "rb") ||
		stream == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Failed to open animation! %s"), *FString(UTF8_TO_TCHAR(path)));
		return -1;
	}

	long read = 0;
	long expectedRead = 1;
	long expectedSize = sizeof(byte);

	//version
	int version = 0;
	expectedSize = sizeof(int);
	read = fread(&version, expectedSize, 1, stream);
	if (read != expectedRead)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Failed to read version!"));
		std::fclose(stream);
		return -1;
	}
	if (version != ANIMATION_VERSION)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Unexpected Version!"));
		std::fclose(stream);
		return -1;
	}

	//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Version: %d"), version);

	//device
	byte device = 0;

	// device type
	byte deviceType = 0;
	expectedSize = sizeof(byte);
	read = fread(&deviceType, expectedSize, 1, stream);
	if (read != expectedRead)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Unexpected DeviceType!"));
		std::fclose(stream);
		return -1;
	}

	//device
	switch ((EChromaSDKDeviceTypeEnum)deviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: DeviceType: 1D"));
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: DeviceType: 2D"));
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Unexpected DeviceType!"));
		std::fclose(stream);
		return -1;
	}

	switch ((EChromaSDKDeviceTypeEnum)deviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		read = fread(&device, expectedSize, 1, stream);
		if (read != expectedRead)
		{
			UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Unexpected Device!"));
			std::fclose(stream);
			return -1;
		}
		else
		{
			switch ((EChromaSDKDevice1DEnum)device)
			{
			case EChromaSDKDevice1DEnum::DE_ChromaLink:
				//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Device: DE_ChromaLink"));
				break;
			case EChromaSDKDevice1DEnum::DE_Headset:
				//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Device: DE_Headset"));
				break;
			case EChromaSDKDevice1DEnum::DE_Mousepad:
				//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Device: DE_Mousepad"));
				break;
			}

			Animation1D* animation1D = new Animation1D();
			animation = animation1D;

			// device
			animation1D->SetDevice((EChromaSDKDevice1DEnum)device);

			//frame count
			int frameCount;

			expectedSize = sizeof(int);
			read = fread(&frameCount, expectedSize, 1, stream);
			if (read != expectedRead)
			{
				UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Error detected reading frame count!"));
				delete animation1D;
				std::fclose(stream);
				return -1;
			}
			else
			{
				vector<FChromaSDKColorFrame1D>& frames = animation1D->GetFrames();
				for (int index = 0; index < frameCount; ++index)
				{
					FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
					int maxLeds = GetMaxLeds((EChromaSDKDevice1DEnum)device);

					//duration
					float duration = 0.0f;
					expectedSize = sizeof(float);
					read = fread(&duration, expectedSize, 1, stream);
					if (read != expectedRead)
					{
						UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Error detected reading duration!"));
						delete animation1D;
						std::fclose(stream);
						return -1;
					}
					else
					{
						if (duration < 0.1f)
						{
							duration = 0.1f;
						}
						frame.Duration = duration;

						// colors
						expectedSize = sizeof(int);
						for (int i = 0; i < maxLeds; ++i)
						{
							int color = 0;
							read = fread(&color, expectedSize, 1, stream);
							if (read != expectedRead)
							{
								UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Error detected reading color!"));
								delete animation1D;
								std::fclose(stream);
								return -1;
							}
							else
							{
								frame.Colors.Add(FChromaSDKPluginModule::ToLinearColor(color));
							}
						}
						if (index == 0)
						{
							frames[0] = frame;
						}
						else
						{
							frames.push_back(frame);
						}
					}
				}
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		read = fread(&device, expectedSize, 1, stream);
		if (read != expectedRead)
		{
			UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Unexpected Device!"));
			std::fclose(stream);
			return -1;
		}
		else
		{
			switch ((EChromaSDKDevice2DEnum)device)
			{
			case EChromaSDKDevice2DEnum::DE_Keyboard:
				//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Device: DE_Keyboard"));
				break;
			case EChromaSDKDevice2DEnum::DE_Keypad:
				//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Device: DE_Keypad"));
				break;
			case EChromaSDKDevice2DEnum::DE_Mouse:
				//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Device: DE_Mouse"));
				break;
			}

			Animation2D* animation2D = new Animation2D();
			animation = animation2D;

			//device
			animation2D->SetDevice((EChromaSDKDevice2DEnum)device);

			//frame count
			int frameCount;

			expectedSize = sizeof(int);
			read = fread(&frameCount, expectedSize, 1, stream);
			if (read != expectedRead)
			{
				UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Error detected reading frame count!"));
				delete animation2D;
				std::fclose(stream);
				return -1;
			}
			else
			{
				vector<FChromaSDKColorFrame2D>& frames = animation2D->GetFrames();
				for (int index = 0; index < frameCount; ++index)
				{
					FChromaSDKColorFrame2D frame = FChromaSDKColorFrame2D();
					int maxRow = GetMaxRow((EChromaSDKDevice2DEnum)device);
					int maxColumn = GetMaxColumn((EChromaSDKDevice2DEnum)device);

					//duration
					float duration = 0.0f;
					expectedSize = sizeof(float);
					read = fread(&duration, expectedSize, 1, stream);
					if (read != expectedRead)
					{
						UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Error detected reading duration!"));
						delete animation2D;
						std::fclose(stream);
						return -1;
					}
					else
					{
						if (duration < 0.1f)
						{
							duration = 0.1f;	
						}
						frame.Duration = duration;

						// colors
						expectedSize = sizeof(int);
						for (int i = 0; i < maxRow; ++i)
						{
							FChromaSDKColors row = FChromaSDKColors();
							for (int j = 0; j < maxColumn; ++j)
							{
								int color = 0;
								read = fread(&color, expectedSize, 1, stream);
								if (read != expectedRead)
								{
									UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Error detected reading color!"));
									delete animation2D;
									std::fclose(stream);
									return -1;
								}
								else
								{
									row.Colors.Add(FChromaSDKPluginModule::ToLinearColor(color));
								}
							}
							frame.Colors.Add(row);
						}
						if (index == 0)
						{
							frames[0] = frame;
						}
						else
						{
							frames.push_back(frame);
						}
					}
				}
			}
		}
		break;
	}

	std::fclose(stream);

	if (animation == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenAnimation: Animation is null! name=%s"), *FString(UTF8_TO_TCHAR(path)));
		return -1;
	}

	//UE_LOG(LogTemp, Log, TEXT("OpenAnimation: Loaded %s"), *FString(UTF8_TO_TCHAR(path)));
	animation->SetName(path);
	int id = _mAnimationId;
	_mAnimations[id] = animation;
	++_mAnimationId;
	_mAnimationMapID[path] = id;
	return id;
}

int FChromaSDKPluginModule::GetAnimation(const char* path)
{
	for (std::map<string, int>::iterator it = _mAnimationMapID.begin(); it != _mAnimationMapID.end(); ++it)
	{
		const string& item = (*it).first;
		if (item.compare(path) == 0) {
			return (*it).second;
		}
	}
	return OpenAnimation(path);
}

void FChromaSDKPluginModule::StopAnimationByType(int animationId, AnimationBase* animation)
{
	if (animation == nullptr)
	{
		return;
	}

	switch (animation->GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			Animation1D* animation1D = (Animation1D*)(animation);
			EChromaSDKDevice1DEnum device = animation1D->GetDevice();
			if (_mPlayMap1D.find(device) != _mPlayMap1D.end())
			{
				int prevAnimation = _mPlayMap1D[device];
				if (prevAnimation != -1)
				{
					StopAnimation(prevAnimation);
				}
			}
			_mPlayMap1D[device] = animationId;
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			Animation2D* animation2D = (Animation2D*)(animation);
			EChromaSDKDevice2DEnum device = animation2D->GetDevice();
			if (_mPlayMap2D.find(device) != _mPlayMap2D.end())
			{
				int prevAnimation = _mPlayMap2D[device];
				if (prevAnimation != -1)
				{
					StopAnimation(prevAnimation);
				}
			}
			_mPlayMap2D[device] = animationId;
		}
		break;
	}
}

void FChromaSDKPluginModule::PlayAnimation(int animationId, bool loop)
{
	if (_mAnimations.find(animationId) != _mAnimations.end())
	{
		AnimationBase* animation = _mAnimations[animationId];
		if (animation == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayAnimation: Animation is null! id=%d"), animationId);
			return;
		}
		StopAnimationByType(animationId, animation);
		UE_LOG(LogTemp, Log, TEXT("PlayAnimation: %s"), *FString(UTF8_TO_TCHAR(animation->GetName().c_str())));
		animation->Play(loop);
	}
}

void FChromaSDKPluginModule::PlayAnimation(const char* path, bool loop)
{
	int animationId = GetAnimation(path);
	if (animationId < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayAnimation: Animation not found! %s"), *FString(UTF8_TO_TCHAR(path)));
		return;
	}
	PlayAnimation(animationId, loop);
}

void FChromaSDKPluginModule::StopAnimation(int animationId)
{
	if (_mAnimations.find(animationId) != _mAnimations.end())
	{
		AnimationBase* animation = _mAnimations[animationId];
		if (animation == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("StopAnimation: Animation is null! id=%d"), animationId);
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("StopAnimation: %s"), *FString(UTF8_TO_TCHAR(animation->GetName().c_str())));
		animation->Stop();
	}
}

void FChromaSDKPluginModule::StopAnimation(const char* path)
{
	int animationId = GetAnimation(path);
	if (animationId < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("StopAnimation: Animation not found! %s"), *FString(UTF8_TO_TCHAR(path)));
		return;
	}
	StopAnimation(animationId);
}

bool FChromaSDKPluginModule::IsAnimationPlaying(int animationId)
{
	if (_mAnimations.find(animationId) != _mAnimations.end())
	{
		AnimationBase* animation = _mAnimations[animationId];
		if (animation == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("IsAnimationPlaying: Animation is null! id=%d"), animationId);
			return false;
		}
		return animation->IsPlaying();
	}
	return false;
}

bool FChromaSDKPluginModule::IsAnimationPlaying(const char* path)
{
	int animationId = GetAnimation(path);
	if (animationId < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("IsAnimationPlaying: Animation not found! %s"), *FString(UTF8_TO_TCHAR(path)));
		return false;
	}
	return IsAnimationPlaying(animationId);
}

#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FChromaSDKPluginModule, ChromaSDKPlugin)
