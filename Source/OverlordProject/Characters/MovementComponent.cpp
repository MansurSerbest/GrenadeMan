#include "../../OverlordEngine/Base/stdafx.h"
#include "MovementComponent.h"
#include "../OverlordEngine/Base/InputManager.h"
#include "../OverlordEngine/Components/ControllerComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "../OverlordEngine/Components/ModelComponent.h"
#include "../OverlordEngine/Graphics/ModelAnimator.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "BombComponent.h"
#include "../OverlordEngine/Base/SoundManager.h"


MovementComponent::MovementComponent(std::vector<std::vector<Grid>>& grids)
	:m_CurrentState{}, m_PrevState{}, m_Speed {1.5f}, m_Grids{grids}, m_DirectionState{DirectionState::backward},
	m_PunchAnimationTimer{0.5f}, m_PunchAnimationTimeLeft{ m_PunchAnimationTimer },
	m_DeadAnimationTimer{0.6f}, m_DeadAnimationTimeLeft{ m_DeadAnimationTimer }
{

}
MovementComponent::~MovementComponent()
{

}

void MovementComponent::SetMovementState(MovementState state)
{
	m_CurrentState = state;
}

void MovementComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void MovementComponent::Update(const GameContext& gameContext)
{
	ControllerComponent *pController = m_pGameObject->GetComponent<ControllerComponent>();
	ModelAnimator *pModelAnimator = m_pGameObject->GetComponent<ModelComponent>()->GetAnimator();
	float deltaTime{ gameContext.pGameTime->GetElapsed() };
	pModelAnimator->SetAnimationSpeed(m_Speed);

	if (pModelAnimator)
	{
		switch (m_CurrentState)
		{
		case MovementComponent::MovementState::idle:
		{
			if (m_PrevState != m_CurrentState)
				pModelAnimator->SetAnimation(0);
			break;
		}
		case MovementComponent::MovementState::left:
		{
			if (m_PrevState != m_CurrentState)
				pModelAnimator->SetAnimation(1);
			GetTransform()->Rotate(XMFLOAT3{ 0.0f, 90.0f,  0.0f });
			m_DirectionState = DirectionState::left;

			if (!IsCollidingWithBomb(deltaTime))
			{
				if(pController)
					pController->Move(XMFLOAT3{ -m_Speed * deltaTime, 0.0f, 0.0f });
			}
			break;
		}
		case MovementComponent::MovementState::right:
		{
			if (m_PrevState != m_CurrentState)
				pModelAnimator->SetAnimation(1);
			GetTransform()->Rotate(XMFLOAT3{ 0.0f, -90.0f, 0.0f });
			m_DirectionState = DirectionState::right;

			if (!IsCollidingWithBomb(deltaTime))
			{
				if(pController)
					pController->Move(XMFLOAT3{ m_Speed * deltaTime, 0.0f, 0.0f });
			}
			break;
		}
		case MovementComponent::MovementState::forward:
		{
			if (m_PrevState != m_CurrentState)
				pModelAnimator->SetAnimation(1);
			GetTransform()->Rotate(XMFLOAT3{ 0.0f, 180.0f, 0.0f });
			m_DirectionState = DirectionState::forward;

			if (!IsCollidingWithBomb(deltaTime))
			{
				if(pController)
					pController->Move(XMFLOAT3{ 0.0f, 0.0f, m_Speed * deltaTime });
			}
			break;
		}
		case MovementComponent::MovementState::backward:
		{
			if (m_PrevState != m_CurrentState)
				pModelAnimator->SetAnimation(1);
			GetTransform()->Rotate(XMFLOAT3{ 0.0f, 0.0f, 0.0f });
			m_DirectionState = DirectionState::backward;

			if (!IsCollidingWithBomb(deltaTime))
			{
				if(pController)
					pController->Move(XMFLOAT3{ 0.0f, 0.0f, -m_Speed * deltaTime });
			}
			break;
		}
		case MovementComponent::MovementState::punch:
		{
			pModelAnimator->SetAnimationSpeed(2.0f);
			if (m_PrevState != m_CurrentState)
			{
				pModelAnimator->SetAnimation(2);
				PlayPunchSound();
			}

			m_PunchAnimationTimeLeft -= gameContext.pGameTime->GetElapsed();
			if (m_PunchAnimationTimeLeft < 0.0f)
			{
				m_PunchAnimationTimeLeft = m_PunchAnimationTimer;
				m_CurrentState = MovementComponent::MovementState::idle;
				pModelAnimator->SetAnimation(0);
			}

			break;
		}
		case MovementComponent::MovementState::dead:
		{
			pModelAnimator->SetAnimationSpeed(1.0f);
			if (m_PrevState != m_CurrentState)
			{
				pModelAnimator->SetAnimation(3);
			}
			m_DeadAnimationTimeLeft -= gameContext.pGameTime->GetElapsed();
			if (m_DeadAnimationTimeLeft < 0.0f)
			{
				m_DeadAnimationTimeLeft = m_DeadAnimationTimer;
				pModelAnimator->Pause();
			}
		}

		}
	}

	m_PrevState = m_CurrentState;
}
void MovementComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

bool MovementComponent::IsCollidingWithBomb(float deltaTime)
{
	XMFLOAT3 bomberManPos{ m_pGameObject->GetTransform()->GetWorldPosition() };
	float roundedPosZ{ roundf(bomberManPos.z) };
	float rounderPosX{ roundf(bomberManPos.x) };
	int realRowIdx = int((roundedPosZ - m_Grids.size() / 2) * -1);
	int realColIdx = int(rounderPosX + m_Grids[0].size() / 2);
	float killBoxTreshold{ 0.9f };

	switch (m_CurrentState)
	{
		case MovementComponent::MovementState::left:
		{
			XMFLOAT3 testPos{ bomberManPos.x - m_Speed * deltaTime, bomberManPos.y, bomberManPos.z };
			int rowIdx = int((roundf(testPos.z) - m_Grids.size() / 2) * -1);
			int colIdx = int(roundf(testPos.x) + m_Grids[0].size() / 2);
			GameObject *pBomb = m_Grids[rowIdx][colIdx].pBomb;
			if (pBomb)
			{
				if (pBomb->GetComponent<BombComponent>()->GetIsBlockingBomberman() && (!m_Grids[realRowIdx][realColIdx].pBomb))
				{
					return true;
				}
			}
			if (m_Grids[realRowIdx][realColIdx - 1].isKillBox)
			{
				if (bomberManPos.x - m_Grids[realRowIdx][realColIdx - 1].position.x < killBoxTreshold)
					return true;
			}
			break;
		}
		case MovementComponent::MovementState::right:
		{
			XMFLOAT3 testPos{ bomberManPos.x + m_Speed * deltaTime, bomberManPos.y, bomberManPos.z };
			int rowIdx = int((roundf(testPos.z) - m_Grids.size() / 2) * -1);
			int colIdx = int(roundf(testPos.x) + m_Grids[0].size() / 2);
			GameObject *pBomb = m_Grids[rowIdx][colIdx].pBomb;
			if (pBomb)
			{
				if (pBomb->GetComponent<BombComponent>()->GetIsBlockingBomberman() && (!m_Grids[realRowIdx][realColIdx].pBomb))
				{
					return true;
				}
			}
			if (m_Grids[realRowIdx][realColIdx + 1].isKillBox)
			{
				if (m_Grids[realRowIdx][realColIdx + 1].position.x - bomberManPos.x < killBoxTreshold)
					return true;
			}
			break;
		}
		case MovementComponent::MovementState::forward:
		{
			XMFLOAT3 testPos{ bomberManPos.x, bomberManPos.y, bomberManPos.z + m_Speed * deltaTime };
			int rowIdx = int((roundf(testPos.z) - m_Grids.size() / 2) * -1);
			int colIdx = int(roundf(testPos.x) + m_Grids[0].size() / 2);
			GameObject *pBomb = m_Grids[rowIdx][colIdx].pBomb;
			if (pBomb)
			{
				if (pBomb->GetComponent<BombComponent>()->GetIsBlockingBomberman() && (!m_Grids[realRowIdx][realColIdx].pBomb))
				{
					return true;
				}
			}
			if (m_Grids[realRowIdx - 1][realColIdx].isKillBox)
			{
				if (m_Grids[realRowIdx - 1][realColIdx].position.z - bomberManPos.z < killBoxTreshold)
					return true;
			}
			break;
		}
		case MovementComponent::MovementState::backward:
		{
			XMFLOAT3 testPos{ bomberManPos.x, bomberManPos.y, bomberManPos.z - m_Speed * deltaTime };
			int rowIdx = int((roundf(testPos.z) - m_Grids.size() / 2) * -1);
			int colIdx = int(roundf(testPos.x) + m_Grids[0].size() / 2);
			GameObject *pBomb = m_Grids[rowIdx][colIdx].pBomb;
			if (pBomb)
			{
				if (pBomb->GetComponent<BombComponent>()->GetIsBlockingBomberman() && (!m_Grids[realRowIdx][realColIdx].pBomb))
				{
					return true;
				}
			}
			if (m_Grids[realRowIdx + 1][realColIdx].isKillBox)
			{
				if (bomberManPos.z - m_Grids[realRowIdx + 1][realColIdx].position.z < killBoxTreshold)
					return true;
			}
		}
	}
	return false;
}

MovementComponent::DirectionState MovementComponent::GetDirectionState() const
{
	return m_DirectionState;
}

void MovementComponent::IncreaseSpeed(float toAddSpeed)
{
	m_Speed += toAddSpeed;
}

void MovementComponent::MoveLeft()
{
	if(m_CurrentState != MovementComponent::MovementState::punch 
		&& m_CurrentState != MovementComponent::MovementState::dead)
		m_CurrentState = MovementComponent::MovementState::left;
}
void MovementComponent::MoveRight()
{
	if (m_CurrentState != MovementComponent::MovementState::punch
		&& m_CurrentState != MovementComponent::MovementState::dead)
		m_CurrentState = MovementComponent::MovementState::right;
}
void MovementComponent::MoveForward()
{
	if (m_CurrentState != MovementComponent::MovementState::punch
		&& m_CurrentState != MovementComponent::MovementState::dead)
		m_CurrentState = MovementComponent::MovementState::forward;
}
void MovementComponent::MoveBackward()
{
	if (m_CurrentState != MovementComponent::MovementState::punch
		&& m_CurrentState != MovementComponent::MovementState::dead)
		m_CurrentState = MovementComponent::MovementState::backward;
}
void MovementComponent::MoveIdle()
{
	if (m_CurrentState != MovementComponent::MovementState::punch
		&& m_CurrentState != MovementComponent::MovementState::dead)
		m_CurrentState = MovementComponent::MovementState::idle;
}
void MovementComponent::MovePunch()
{
	m_CurrentState = MovementComponent::MovementState::punch;
}
void MovementComponent::MoveDead()
{
	m_CurrentState = MovementComponent::MovementState::dead;
}
void MovementComponent::PlayPunchSound()
{
	SoundManager *pSoundManager = SoundManager::GetInstance();
	pSoundManager->Play2DSound(1, 1, false, false, "Resources/Sound/punchBomb.wav");
	//auto pSoundSystem = SoundManager::GetInstance()->GetSystem();
	//FMOD_RESULT result;
	//FMOD::Sound* pPunchSound;
	//result = pSoundSystem->createSound("Resources/Sound/punchBomb.wav", FMOD_2D, nullptr, &pPunchSound);
	//SoundManager::ErrorCheck(result);
	//FMOD::Channel *m_pChannel;
	//pSoundSystem->playSound(pPunchSound, nullptr, false, &m_pChannel);
}