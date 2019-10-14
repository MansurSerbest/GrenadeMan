#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"
#include <deque>
#include "..\Levels\Level1.h"

class ModelComponent;
class ParticleEmitterComponent;
class GameObject;

class BombManagerComponent final : public BaseComponent
{
public:
	BombManagerComponent(std::vector<std::vector<Grid>>& grids, int playerIdx, bool isMaterialInitialized);
	~BombManagerComponent();

	void DropBomb(const GameContext& gameContext);
	void ExplodeBomb(GameObject* pBomb, int rowIdx, int colIdx);
	void PendToKill(GameObject *pObject);
	void IncreaseBombCapacity(unsigned int toAddCapacity);
	void IncreaseBombPower(int toAddBombPower);
private:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void PostDraw(const GameContext& gameContext) override;
	
	void EarlyInitializeMeshModelAndMaterial(const GameContext& gameContext);
	void ExpandExplosion(int rowIdx, int colIdx);
	void CreateExplosion(Grid &grid);
	void PlayDropBombSound();
private:
	std::vector<std::vector<Grid>>& m_Grids;
	unsigned int m_BombCapacity;
	int m_BombPower;
	unsigned int m_NrActiveBombs;
	std::deque<GameObject*> m_ActiveBombs;

	XMFLOAT4 m_ExplosionColor;

	std::vector<GameObject*> m_PendingToKillObjects;
	int m_PlayerIdx;
	int m_MaterialIdOffset;
	bool m_IsMaterialInitialized;
};


