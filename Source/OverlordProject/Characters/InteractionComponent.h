#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"
#include "..\Levels\Level1.h"
#include "MovementComponent.h"
#include "../../OverlordProject/Characters/PickUpComponent.h"
#include <list>

class InteractionComponent : public BaseComponent
{
public:
	InteractionComponent(std::vector<std::vector<Grid>> &grids);
	~InteractionComponent();
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	void PunchBomb(const GameContext& gameContext);
	void DeleteAllActiveParticles();
	void DeleteParticle(GameObject* pParticle);
	void ActiveIsKillMode();
	void CheckCollisionWithKillBox(int rowIdx, int colIdx);
private:
	void CheckCollisionWithExplosion(int rowIdx, int colIdx);
	void CheckCollisionWithPickUp(int rowIdx, int colIdx);
	void UpdateImmuneTimer(const GameContext& gameContext);
	void CreatePickUpParticle(PickUpComponent::PickUpState state);

private:
	std::vector<std::vector<Grid>> &m_Grids;
	std::list<GameObject*> m_PickUpParticles;
	bool m_IsImmuneToExplosion;
	float m_ImmuneTimer;
	float m_ImmuneTimeLeft;
	bool m_CanPunchBomb;
	int m_CurrRowIdx;
	int m_CurrColIdx;

	bool m_IsPickUpParticleCreated;
	bool m_IsKillMode;
};
