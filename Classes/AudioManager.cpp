#include "AudioManager.h"

AudioManager * AudioManager::instance = nullptr;
bool AudioManager::isMute = false;

AudioManager & AudioManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new AudioManager();
	}

	return *instance;
}

AudioManager::AudioManager()
{
	AudioEngine::preload("Audio/bg.mp3");
	AudioEngine::preload("Audio/btn.mp3");
}

AudioManager::~AudioManager()
{
	Release();
}

void AudioManager::PlayAudio(const AudioType & type)
{
	switch (type)
	{
	case Bg:
		 AudioEngine::play2d("Audio/bg.mp3", true);
		break;
	case Btn:
		if (!isMute) {
			AudioEngine::play2d("Audio/btn.mp3");
		}
		break;
	}
}

void AudioManager::MuteAudio(bool isMute)
{
	this->isMute = isMute;
	if (isMute) {
		AudioEngine::pauseAll();
	}else {
		AudioEngine::resumeAll();
	}
}

void AudioManager::Release()
{
	AudioEngine::end();
	AudioEngine::uncacheAll();
	if (instance != nullptr)
		delete instance;
}
