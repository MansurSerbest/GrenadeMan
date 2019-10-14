#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"

class GameObject;

class PickUpParticleComponent : public BaseComponent
{
public:
	PickUpParticleComponent(GameObject *pBomberMan);
	~PickUpParticleComponent();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
private:
	float m_ParticleLifeTime;
	float m_ParticleLifeTimeLeft;
	bool m_IsFadingOut;
	GameObject *m_pPickingBomberMan;

	float m_FadeOutTime;
	float m_FadeOutTimeLeft;
};