#include "AudioManager.h"

AudioManager *AudioManager::m_singleton = nullptr;

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{
	disdroyInstance();
}

AudioManager *AudioManager::getInstance()
{
	if (m_singleton == nullptr)
	{
		m_singleton = new AudioManager;
	}

	return m_singleton;
	
}

void AudioManager::disdroyInstance()
{
	CC_SAFE_DELETE(m_singleton);
	m_singleton = nullptr;
}

void AudioManager::asynLoadAudioFile()
{
    
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bg.mp3");
}

