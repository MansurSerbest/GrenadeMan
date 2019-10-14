#include "../../OverlordEngine/Base/stdafx.h"
#include "GameData.h"

GameData *GameData::m_pInstance = nullptr;

GameData::GameData()
	:m_NrOfPlayers{4}
{

}

int GameData::GetNrOfPlayers() const
{
	return m_NrOfPlayers;
}

void GameData::SetNrOfPlayers(int nrOfPlayers)
{
	m_NrOfPlayers = nrOfPlayers;
}