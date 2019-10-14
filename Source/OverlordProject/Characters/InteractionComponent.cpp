#include "../../OverlordEngine/Base/stdafx.h"
#include "InteractionComponent.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "HealthAndScoreComponent.h"
#include "BombComponent.h"
#include "ExplosionComponent.h"
#include "PickUpComponent.h"
#include "MovementComponent.h"
#include "BombManagerComponent.h"
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include "../OverlordEngine/Base/SoundManager.h"
#include "../OverlordEngine/Components/ParticleEmitterComponent.h"
#include "PickUpParticleComponent.h"
#include "../OverlordEngine/Content/ContentManager.h"
#include "../OverlordEngine/Graphics/TextureData.h"
#include "../OverlordEngine/Components/ControllerComponent.h"
#include "InputComponent.h"

InteractionComponent::InteractionComponent(std::vector<std::vector<Grid>> &grids)
	:m_Grids{ grids }, m_IsImmuneToExplosion{ false }, m_ImmuneTimer{ 3.0f }, m_ImmuneTimeLeft{ m_ImmuneTimer },
	m_IsPickUpParticleCreated{ false }, m_PickUpParticles{}, m_CurrRowIdx{}, m_CurrColIdx{}
{

}
InteractionComponent::~InteractionComponent()
{

}
void InteractionComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	ContentManager::Load<TextureData>(L"Resources/Textures/Particle/SpeedPickUpParticle.png");
	ContentManager::Load<TextureData>(L"Resources/Textures/Particle/BombCapacityPickUpParticle.png");
	ContentManager::Load<TextureData>(L"Resources/Textures/Particle/BombPowerPickUpParticle.png");
}
void InteractionComponent::Update(const GameContext& gameContext)
{
	XMFLOAT3 bomberManPos{ GetTransform()->GetWorldPosition() };
	float roundedPosZ{ roundf(bomberManPos.z) };
	float rounderPosX{ roundf(bomberManPos.x) };
	int rowIdx = int((roundedPosZ - m_Grids.size() / 2) * -1);
	int colIdx = int(rounderPosX + m_Grids[0].size() / 2);
	m_CurrRowIdx = rowIdx;
	m_CurrColIdx = colIdx;

	if(!m_IsImmuneToExplosion)
		CheckCollisionWithExplosion(rowIdx, colIdx);

	if (m_IsImmuneToExplosion)
		UpdateImmuneTimer(gameContext);

	CheckCollisionWithPickUp(rowIdx, colIdx);
}
void InteractionComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void InteractionComponent::PunchBomb(const GameContext& gameContext)
{
	float kickDistanceTreshold{ 0.7f };
	float kickSpeed{ 3.0f };
	float deltaTime{ gameContext.pGameTime->GetElapsed() };

	MovementComponent::DirectionState directionState{};
	directionState = m_pGameObject->GetComponent<MovementComponent>()->GetDirectionState();

	XMFLOAT3 bomberManPos{ m_pGameObject->GetTransform()->GetWorldPosition() };
	int rowIdx = int((roundf(bomberManPos.z) - m_Grids.size() / 2) * -1);
	int colIdx = int(roundf(bomberManPos.x) + m_Grids[0].size() / 2);
	

	switch (directionState)
	{
	case MovementComponent::DirectionState::left:
	{
		GameObject *pBomb = m_Grids[rowIdx][colIdx - 1].pBomb;
		if(pBomb)
		{
			float distance = bomberManPos.x - pBomb->GetTransform()->GetWorldPosition().x;
			if (distance < kickDistanceTreshold)
			{
				XMFLOAT3 velocity{ -kickSpeed * deltaTime, 0.0f, 0.0f };
				pBomb->GetComponent<BombComponent>()->PunchBomb(velocity);
			}
		}
		break;
	}
	case MovementComponent::DirectionState::right:
	{
		GameObject *pBomb = m_Grids[rowIdx][colIdx + 1].pBomb;
		if (pBomb)
		{
			if (pBomb->GetTransform()->GetWorldPosition().x - bomberManPos.x < kickDistanceTreshold)
			{
				XMFLOAT3 velocity{ kickSpeed * deltaTime, 0.0f, 0.0f };
				pBomb->GetComponent<BombComponent>()->PunchBomb(velocity);
			}
		}
		break;
	}
	case MovementComponent::DirectionState::forward:
	{
		GameObject *pBomb = m_Grids[rowIdx - 1][colIdx].pBomb;
		if (pBomb)
		{
			if (pBomb->GetTransform()->GetWorldPosition().z - bomberManPos.z < kickDistanceTreshold)
			{
				XMFLOAT3 velocity{ 0.0f, 0.0f, kickSpeed * deltaTime};
				pBomb->GetComponent<BombComponent>()->PunchBomb(velocity);
			}
		}
		break;
	}
	case MovementComponent::DirectionState::backward:
	{
		GameObject *pBomb = m_Grids[rowIdx + 1][colIdx].pBomb;
		if (pBomb)
		{
			if (bomberManPos.z - pBomb->GetTransform()->GetWorldPosition().z < kickDistanceTreshold)
			{
				XMFLOAT3 velocity{ 0.0f, 0.0f, -kickSpeed * deltaTime };
				pBomb->GetComponent<BombComponent>()->PunchBomb(velocity);
			}
		}
		break;
	}
	}
}

void InteractionComponent::DeleteAllActiveParticles()
{
	auto pScene = m_pGameObject->GetScene();
	for (GameObject* pParticle : m_PickUpParticles)
	{
		pScene->RemoveChild(pParticle);
	}
	m_PickUpParticles.clear();
}

void InteractionComponent::DeleteParticle(GameObject* pParticle)
{
	auto particle = std::find(m_PickUpParticles.begin(), m_PickUpParticles.end(), pParticle);
	if (particle != m_PickUpParticles.end())
	{
		m_PickUpParticles.erase(particle);
	}
}

void InteractionComponent::CheckCollisionWithExplosion(int rowIdx, int colIdx)
{
	if (!m_pGameObject->GetComponent<HealthAndScoreComponent>()->GetIsDead())
	{

		if (m_Grids[rowIdx][colIdx].pExplosion)
		{
			m_IsImmuneToExplosion = true;
			m_pGameObject->GetComponent<HealthAndScoreComponent>()->AddHealth(-1.0f);
			auto pBombOwner = m_Grids[rowIdx][colIdx].pExplosion->GetComponent<ExplosionComponent>()->GetBomberManOwner();
			if (pBombOwner && (m_pGameObject != pBombOwner))
			{
				pBombOwner->GetComponent<HealthAndScoreComponent>()->AddScore(1.0f);
			}
		}
	}

}

void InteractionComponent::CheckCollisionWithPickUp(int rowIdx, int colIdx)
{
	GameObject *pPickUp = m_Grids[rowIdx][colIdx].pPickUp;
	if (pPickUp)
	{
		PickUpComponent::PickUpState pickupState = pPickUp->GetComponent<PickUpComponent>()->GetState();
		switch (pickupState)
		{
		case PickUpComponent::PickUpState::speed:
			m_pGameObject->GetComponent<MovementComponent>()->IncreaseSpeed(0.5f);
			break;
		case PickUpComponent::PickUpState::bombCapacity:
			m_pGameObject->GetComponent<BombManagerComponent>()->IncreaseBombCapacity(1);
			break;
		case PickUpComponent::PickUpState::bombPower:
			m_pGameObject->GetComponent<BombManagerComponent>()->IncreaseBombPower(1);
			break;
		}

		m_pGameObject->GetScene()->PendToRemoveChild(pPickUp);
		m_Grids[rowIdx][colIdx].pPickUp = nullptr;
		SoundManager::GetInstance()->Play2DSound(5, 5, 0, false, "Resources/Sound/PickUpSound.wav");
		SoundManager::GetInstance()->SetChannelVolume(5, 0.2f);

		CreatePickUpParticle(pickupState);
	}
}

void InteractionComponent::UpdateImmuneTimer(const GameContext& gameContext)
{
	m_ImmuneTimeLeft -= gameContext.pGameTime->GetElapsed();
	if (m_ImmuneTimeLeft <= 0.0f)
	{
		m_ImmuneTimeLeft = m_ImmuneTimer;
		m_IsImmuneToExplosion = false;
	}
}

void InteractionComponent::CreatePickUpParticle(PickUpComponent::PickUpState state)
{
	ParticleEmitterComponent * pParticleEmitterComp = nullptr;
	XMFLOAT4 color{};
	switch (state)
	{
	case PickUpComponent::PickUpState::speed:
		color = { 0.5f, 1.0f, 0.5f, 1.0f };
		pParticleEmitterComp = new ParticleEmitterComponent(L"Resources/Textures/Particle/BombCapacityPickUpParticle.png", 10);
		break;
	case PickUpComponent::PickUpState::bombCapacity:
		color = { 1.0f, 1.0f, 1.0f, 1.0f };
		pParticleEmitterComp = new ParticleEmitterComponent(L"Resources/Textures/Particle/BombCapacityPickUpParticle.png", 10);
		break;
	case PickUpComponent::PickUpState::bombPower:
		color = { 1.0f, 0.5f, 0.f, 1.0f };
		pParticleEmitterComp = new ParticleEmitterComponent(L"Resources/Textures/Particle/BombCapacityPickUpParticle.png", 10);
		break;
	}

	float scale{ 0.1f };
	pParticleEmitterComp->SetVelocity(XMFLOAT3(0.0f, 6.0f / scale, 0.0f));
	pParticleEmitterComp->SetMinSize(1.5f);
	pParticleEmitterComp->SetMaxSize(2.0f);
	pParticleEmitterComp->SetMinEnergy(0.4f);
	pParticleEmitterComp->SetMaxEnergy(0.5f);
	pParticleEmitterComp->SetMinSizeGrow(5.0f);
	pParticleEmitterComp->SetMaxSizeGrow(6.0f);
	pParticleEmitterComp->SetMinEmitterRange(0.0f);
	pParticleEmitterComp->SetMaxEmitterRange(0.6f);
	pParticleEmitterComp->SetColor(color);


	GameObject *pPickUpParticleObject = new GameObject();
	pPickUpParticleObject->GetTransform()->Translate(GetTransform()->GetWorldPosition());
	pPickUpParticleObject->GetTransform()->Scale(scale, scale, scale);
	pPickUpParticleObject->AddComponent(pParticleEmitterComp);
	pPickUpParticleObject->AddComponent(new PickUpParticleComponent(m_pGameObject));
	m_pGameObject->GetScene()->PendToAddChild(pPickUpParticleObject);

	m_PickUpParticles.push_back(pPickUpParticleObject);

}

void InteractionComponent::CheckCollisionWithKillBox(int rowIdx, int colIdx)
{
	if (m_IsKillMode)
	{
		if (m_CurrRowIdx == rowIdx && m_CurrColIdx == colIdx)
		{
			m_pGameObject->GetComponent<HealthAndScoreComponent>()->AddHealth(-1.0f);
			BaseComponent* toKillComp = m_pGameObject->GetComponent<ControllerComponent>();
			m_pGameObject->PendToRemoveComponent(toKillComp);
			m_pGameObject->GetComponent<InputComponent>()->SetEnableInput(false);
		}
	}
}

void InteractionComponent::ActiveIsKillMode()
{
	m_IsKillMode = true;
}