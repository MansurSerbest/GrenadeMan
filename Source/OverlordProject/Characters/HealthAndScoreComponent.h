#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"
#include "..\Levels\Level1.h"
#include "MovementComponent.h"

class HealthAndScoreComponent : public BaseComponent
{
public:
	HealthAndScoreComponent();
	~HealthAndScoreComponent();
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;

	void AddScore(float score);
	float GetScore() const;
	void AddHealth(float health);
	float GetHealth() const;
	bool GetIsDead() const;

private:
	float m_Health;
	float m_Score;
	Grid m_Grid;
	bool m_IsDead;
};
