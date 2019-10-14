//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "SoundManager.h"

using namespace FMOD;

SoundManager::SoundManager(void)
{
	Initialize();
}


SoundManager::~SoundManager(void)
{
	if(m_pFmodSystem)
	{
		m_pFmodSystem->release();
	}
}

bool SoundManager::ErrorCheck(FMOD_RESULT res)
{
	if(res != FMOD_OK)
	{
		wstringstream strstr;
		strstr << L"FMOD error! \n[" << res << L"] "<< FMOD_ErrorString(res) << endl;
		Logger::LogError(strstr.str());
		return false;
	}

	return true;
}

void SoundManager::Initialize()
{
	FMOD_RESULT result;
	unsigned int version;
	int numdrivers;
	
	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&m_pFmodSystem);
	ErrorCheck(result);
	result = m_pFmodSystem->getVersion(&version);
	ErrorCheck(result);
	
	if (version < FMOD_VERSION)
	{
		Logger::LogFormat(LogLevel::Error, L"SoundManager Initialization Failed!\n\nYou are using an old version of FMOD %08x. This program requires %08x\n",
		version, FMOD_VERSION);
		return;
	}

	result = m_pFmodSystem->getNumDrivers(&numdrivers);
	ErrorCheck(result);
	if (numdrivers == 0)
	{
		result = m_pFmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ErrorCheck(result);
	}
	else
	{
		result = m_pFmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);
		ErrorCheck(result);
	}

}

void SoundManager::AddSound(UINT id, FMOD::Sound* pSound)
{
	m_Sounds[id] = pSound;
}

void SoundManager::AddChannel(UINT id, FMOD::Channel* pChannel)
{
	m_Channels[id] = pChannel;
}
FMOD::Sound* SoundManager::GetSound(UINT id)
{
	return m_Sounds[id];
}
FMOD::Channel* SoundManager::GetChannel(UINT id)
{
	return m_Channels[id];
}
void SoundManager::Play2DSound(UINT soundId, UINT channelId, int loop, bool isPause, const char* filePath)
{
	FMOD_RESULT result{};
	FMOD::Sound* pSound = m_Sounds[soundId];
	FMOD::Channel *pChannel = m_Channels[channelId];
	if (!pSound)
	{
		result = m_pFmodSystem->createSound(filePath, FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &pSound);
		SoundManager::ErrorCheck(result);
		m_Sounds[soundId] = pSound;
	}
	pSound->setLoopCount(loop);
	m_pFmodSystem->playSound(pSound, nullptr, isPause, &pChannel);
	m_Channels[channelId] = pChannel;

}

void SoundManager::SetChannelVolume(UINT channelId, float volume)
{
	m_Channels[channelId]->setVolume(volume);
}

void SoundManager::Stop2DSound(UINT soundId, UINT channelId)
{
	m_Sounds[soundId]->release();
	m_Sounds.erase(soundId);
	UNREFERENCED_PARAMETER(channelId);
}