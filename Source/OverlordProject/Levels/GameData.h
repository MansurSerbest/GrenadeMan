#pragma once

class GameScene;

class GameData
{
public:
	static GameData& GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new GameData();
		}
		return *m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	int GetNrOfPlayers() const;
	void SetNrOfPlayers(int nrOfPlayers);
private:
	GameData();
private:
	static GameData *m_pInstance;
	int m_NrOfPlayers;
};
