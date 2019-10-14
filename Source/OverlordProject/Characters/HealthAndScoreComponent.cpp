#include "../../OverlordEngine/Base/stdafx.h"
#include "HealthAndScoreComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "../Characters/InputComponent.h"
#include "../OverlordEngine/Components/ControllerComponent.h"

HealthAndScoreComponent::HealthAndScoreComponent()
	:m_Health{1}, m_Score{0}, m_IsDead{false}
{

}
HealthAndScoreComponent::~HealthAndScoreComponent()
{

}
void HealthAndScoreComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void HealthAndScoreComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (m_Health <= 0.0f && !m_IsDead)
	{
		m_pGameObject->GetComponent<MovementComponent>()->MoveDead();
		m_pGameObject->GetComponent<InputComponent>()->SetEnableInput(false);
		BaseComponent* toKillComp = m_pGameObject->GetComponent<ControllerComponent>();
		if(toKillComp)
			m_pGameObject->PendToRemoveComponent(toKillComp);
		m_IsDead = true;
	}
}
void HealthAndScoreComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void HealthAndScoreComponent::AddScore(float score)
{
	m_Score += score;
}
float HealthAndScoreComponent::GetScore() const
{
	return m_Score;
}
void HealthAndScoreComponent::AddHealth(float health)
{
	m_Health += health;
}
float HealthAndScoreComponent::GetHealth() const
{
	return m_Health;
}
bool HealthAndScoreComponent::GetIsDead() const
{
	return m_IsDead;
}