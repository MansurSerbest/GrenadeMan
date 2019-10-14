#pragma once
#include "../Helpers/Singleton.h"
#include <map>

class SoundManager: public Singleton<SoundManager>
{
public:
	SoundManager(void);
	~SoundManager(void);

	static bool ErrorCheck(FMOD_RESULT res);
	FMOD::System* GetSystem() const { return m_pFmodSystem; }

	void AddSound(UINT id, FMOD::Sound* pSound);
	void AddChannel(UINT id, FMOD::Channel* pChannel);
	FMOD::Sound* GetSound(UINT id);
	FMOD::Channel* GetChannel(UINT id);
	void Play2DSound(UINT soundId, UINT channelId, int loop, bool isPause, const char* filePath);
	void SetChannelVolume(UINT channelId, float volume);
	void Stop2DSound(UINT soundId, UINT channelId);
private:
	void Initialize();
	FMOD::System* m_pFmodSystem = nullptr;

	map<UINT, FMOD::Sound*> m_Sounds;
	map<UINT, FMOD::Channel*> m_Channels;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SoundManager(const SoundManager &obj) = delete;
	SoundManager& operator=(const SoundManager& obj) = delete;

};

