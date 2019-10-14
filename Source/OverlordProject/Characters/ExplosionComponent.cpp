#include "../../OverlordEngine/Base/stdafx.h"
#include "ExplosionComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include "BombManagerComponent.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "BombComponent.h"
#include "PickUpComponent.h"
#include "../OverlordEngine/Components/ModelComponent.h"
#include "../Materials/DiffuseMaterial.h"
#include <ctime>

ExplosionComponent::ExplosionComponent(GameObject *pOwner, Grid &currGrid)
	:m_LifeTime{1.0f}, m_LifeTimeLeft{m_LifeTime}, m_pBomberManOwner{ pOwner }, m_CurrentGrid{ currGrid }
{

}

ExplosionComponent::~ExplosionComponent()
{

}

GameObject* ExplosionComponent::GetBomberManOwner() const
{
	if (m_pBomberManOwner)
		return m_pBomberManOwner;

	return nullptr;
}

void ExplosionComponent::ResetLifeTimeLeft()
{
	m_LifeTimeLeft = m_LifeTime;
}

void ExplosionComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	bool isBoxExplode = ExplodeBox();
	if (!isBoxExplode)
		CheckCollisionWithPickup();
}

void ExplosionComponent::Update(const GameContext& gameContext)
{
	TriggerExplodeBomb();


	m_LifeTimeLeft -= gameContext.pGameTime->GetElapsed();
	if (m_LifeTimeLeft <= 0.0f)
	{
		m_LifeTimeLeft = m_LifeTime;
		m_pGameObject->GetScene()->PendToRemoveChild(m_pGameObject);
		m_CurrentGrid.pExplosion = nullptr;
	}


	
}

void ExplosionComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

bool ExplosionComponent::ExplodeBox()
{
	if (m_CurrentGrid.pBox)
	{
		//m_pGameObject->GetScene()->RemoveChild(m_CurrentGrid.pBox, true);
		m_pGameObject->GetScene()->PendToRemoveChild(m_CurrentGrid.pBox);
		m_CurrentGrid.pBox = nullptr;
		CreatePickUp();
		return true;
	}

	return false;
}

void ExplosionComponent::TriggerExplodeBomb()
{
	if (m_CurrentGrid.pBomb)
	{
		m_CurrentGrid.pBomb->GetComponent<BombComponent>()->Explode();
	}
}

void ExplosionComponent::CreatePickUp()
{
	srand((unsigned int)time(0) * (unsigned int)m_pGameObject);
	int randomNr = rand() % 6; // 3 Pickups possible, 50% chance for pickup
	if (randomNr < 3)
	{
		GameObject *pPickUp = new GameObject();
		TransformComponent *transformComponent = pPickUp->GetTransform();
		XMFLOAT3 pos = { m_CurrentGrid.position.x, m_CurrentGrid.position.y + 0.1f, m_CurrentGrid.position.z };
		transformComponent->Translate(pos);
		transformComponent->Rotate(45.0f, 0.0f, 0.0f);
		transformComponent->Scale(0.5f, 0.5f, 0.5f);
		GameScene *pScene = m_pGameObject->GetScene();

		PickUpComponent *pPickUpComponent = new PickUpComponent(randomNr, pos);
		pPickUp->AddComponent(pPickUpComponent);

		ModelComponent *pModelComponent = new ModelComponent(L"Resources/Meshes/PickUpPlane.ovm");
		pPickUp->AddComponent(pModelComponent);
		int materialIdOffset{ 11 };
		pModelComponent->SetMaterial(materialIdOffset + randomNr);

		m_CurrentGrid.pPickUp = pPickUp;
		pScene->PendToAddChild(pPickUp);
	}
}

void ExplosionComponent::CheckCollisionWithPickup()
{
	if (m_CurrentGrid.pPickUp)
	{
		m_pGameObject->GetScene()->PendToRemoveChild(m_CurrentGrid.pPickUp);
		m_CurrentGrid.pPickUp = nullptr;
	}
}
