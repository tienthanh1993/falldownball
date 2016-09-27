#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;


class AudioManager 
{
public:
	static AudioManager *getInstance();
	static void disdroyInstance();

	void asynLoadAudioFile();

private:
	AudioManager();
	~AudioManager();
private:
	static AudioManager *m_singleton;
};

#endif // __AUDIO_MANAGER_H__
