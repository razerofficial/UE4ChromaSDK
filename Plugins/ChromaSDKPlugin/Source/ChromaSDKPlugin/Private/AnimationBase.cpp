#//include "ChromaSDKPlugin.h" //(support 4.15 or below)___HACK_UE4_WANTS_MODULE_FIRST
#include "AnimationBase.h"
#include "ChromaSDKPlugin.h" //(support 4.16 or above)___HACK_UE4_WANTS_HEADER_FIRST

using namespace ChromaSDK;
using namespace std;

AnimationBase::AnimationBase()
{
	_mCurrentFrame = 0;
	_mIsPlaying = false;
	_mTime = 0.0f;
}

const string& AnimationBase::GetName()
{
	return _mName;
}

void AnimationBase::SetName(const std::string& name)
{
	_mName = name;
}

int AnimationBase::GetCurrentFrame()
{
	return _mCurrentFrame;
}

void AnimationBase::SetCurrentFrame(int index)
{
	if (index < 0 ||
		index >= GetFrameCount())
	{
		index = 0;
	}
	_mCurrentFrame = index;
}

bool AnimationBase::IsPlaying()
{
	return _mIsPlaying;
}