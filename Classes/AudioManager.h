#pragma once

#include "cocos2d.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace experimental;

class AudioManager
{
public:
	static AudioManager &GetInstance();

	enum AudioType
	{
		Bg,
		Btn,

	};

private:
	AudioManager();
	~AudioManager();

public:
	void PlayAudio(const AudioType& type);
	void MuteAudio(bool isMute);

	void Release();

public:
	static bool isMute;

public:
	static AudioManager *instance;
};

