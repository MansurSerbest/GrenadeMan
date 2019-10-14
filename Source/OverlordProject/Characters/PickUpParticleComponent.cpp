#include "../../OverlordEngine/Base/stdafx.h"
#include "PickUpParticleComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "../OverlordEngine/Components/ParticleEmitterComponent.h"
#include "InteractionComponent.h"

PickUpParticleComponent::PickUpParticleComponent(GameObject *pBomberMan)
	:m_ParticleLifeTime{ 0.5f }, m_ParticleLifeTimeLeft{ m_ParticleLifeTime }, m_IsFadingOut{ false }, m_pPickingBomberMan {
	pBomberMan
},
	m_FadeOutTime{1.0f}, m_FadeOutTimeLeft{ m_FadeOutTime }
{

}
PickUpParticleComponent::~PickUpParticleComponent()
{

}
void PickUpParticleComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void PickUpParticleComponent::Update(const GameContext& gameContext)
{
	//Update Position
	GetTransform()->Translate(m_pPickingBomberMan->GetTransform()->GetWorldPosition());
	//When Game is Reset, the bomberman is reset too. So m_pPickingBomberman is a pointer to the deleted bomberman. Fix this

	//Check Life Time
	if (!m_IsFadingOut)
	{
		m_ParticleLifeTimeLeft -= gameContext.pGameTime->GetElapsed();
		if (m_ParticleLifeTimeLeft <= 0.0f)
		{
			//m_ParticleLifeTimeLeft = m_ParticleLifeTime;
			//m_pGameObject->GetScene()->PendToRemoveChild(m_pGameObject);
			m_pGameObject->GetComponent<ParticleEmitterComponent>()->SetIsActive(false);
			// This will make the last active particle fade out first. This will be more graceful then suddenly disappearing particles
		}
	}
	else
	{
		m_FadeOutTimeLeft -= gameContext.pGameTime->GetElapsed();
		if (m_FadeOutTimeLeft < 0.0f)
		{
			m_pGameObject->GetScene()->PendToRemoveChild(m_pGameObject);
			m_pPickingBomberMan->GetComponent<InteractionComponent>()->DeleteParticle(m_pGameObject);
		}
	}
}
void PickUpParticleComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}