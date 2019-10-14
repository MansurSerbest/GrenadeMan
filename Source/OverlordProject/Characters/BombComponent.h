#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"
#include "..\Levels\Level1.h"

class GameObject;

class BombComponent : public BaseComponent
{
public:
	BombComponent(GameObject *pOwner, int bombPower, std::vector<std::vector<Grid>>& grids, int rowIdx, int colIdx);
	~BombComponent();

	bool GetIsBlockingBomberman() const;
	void PunchBomb(XMFLOAT3 velocity);
	void Explode();
private:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	void MoveBomb();
	void IsCollidingWithBoxAndBomb(int rowIdx, int colIdx, XMFLOAT3 position);
	void PlayExplosionSound();
private:
	std::vector<std::vector<Grid>>& m_Grids;
	GameObject * m_pBomberManOwner;
	int m_Power;
	float m_ExplodeTimer;
	float m_ExplodeTimeLeft;
	bool m_IsExploded;
	bool m_IsBlockingBomberman;
	bool m_IsMoving;
	XMFLOAT3 m_Velocity;

	int m_CurrentColumn;
	int m_CurrentRow;
};