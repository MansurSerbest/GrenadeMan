#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"
#include "..\Levels\Level1.h"

class GameObject;

class ExplosionComponent : public BaseComponent
{
public:
	ExplosionComponent(GameObject *pOwner, Grid &currGrid);
	~ExplosionComponent();

	GameObject* GetBomberManOwner() const;
	void ResetLifeTimeLeft();
private:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	bool ExplodeBox();
	void TriggerExplodeBomb();
	void CreatePickUp();
	void CheckCollisionWithPickup();

private:
	float m_LifeTime;
	float m_LifeTimeLeft;
	GameObject *m_pBomberManOwner;
	Grid &m_CurrentGrid;
};
