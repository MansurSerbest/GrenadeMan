#include "../OverlordEngine/Base/stdafx.h"
#include "BombComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include "BombManagerComponent.h"
#include "../OverlordEngine/Components/ModelComponent.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "../OverlordEngine/Base/SoundManager.h"

BombComponent::BombComponent(GameObject *pOwner, int bombPower, std::vector<std::vector<Grid>>& grids, int rowIdx, int colIdx)
	:m_pBomberManOwner{pOwner}, m_Power{bombPower}, m_ExplodeTimer{4.0f},
	m_ExplodeTimeLeft{ m_ExplodeTimer }, m_IsExploded{false}, m_IsBlockingBomberman{false}, m_Grids{grids}, m_IsMoving{false}, m_Velocity{},
	m_CurrentColumn{colIdx}, m_CurrentRow{rowIdx}
{

}
BombComponent::~BombComponent()
{

}

void BombComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BombComponent::Update(const GameContext& gameContext)
{
	if (m_IsMoving)
	{
		MoveBomb();
	}

	XMFLOAT3 bombPos{ m_pGameObject->GetTransform()->GetWorldPosition() };
	int rowIdxBomb = int((roundf(bombPos.z) - m_Grids.size() / 2) * -1);
	int colIdxBomb = int(roundf(bombPos.x) + m_Grids[0].size() / 2);

	if (!m_IsBlockingBomberman)
	{
		XMFLOAT3 ownerPos{ m_pBomberManOwner->GetTransform()->GetWorldPosition() };
		int rowIdxBomberMan = int((roundf(ownerPos.z) - m_Grids.size() / 2) * -1);
		int colIdxBomberMan = int(roundf(ownerPos.x) + m_Grids[0].size() / 2);

		if ((rowIdxBomberMan != rowIdxBomb) || (colIdxBomberMan != colIdxBomb))
		{
			m_IsBlockingBomberman = true;
		}
	}

	m_ExplodeTimeLeft -= gameContext.pGameTime->GetElapsed();
	if (m_ExplodeTimeLeft <= 0.0f)
	{
		m_ExplodeTimeLeft = m_ExplodeTimer;
		Explode();
	}

}

void BombComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BombComponent::PunchBomb(XMFLOAT3 velocity)
{
	m_Velocity = velocity;
	m_IsMoving = true;
}

void BombComponent::Explode()
{
	m_Grids[m_CurrentRow][m_CurrentColumn].pBomb = nullptr; 
	// Put this in front of ExplodeBomb, because this first need to be set to null,
	// When explosionobject will call TriggerExplodeBomb, it will check if bomb in this gird is still valid. It should be null. Hence, put this
	// before ExplodeBomb below
	m_pBomberManOwner->GetComponent<BombManagerComponent>()->ExplodeBomb(m_pGameObject, m_CurrentRow, m_CurrentColumn);
	//m_pGameObject->GetScene()->RemoveChild(m_pGameObject, false);
	//m_pBomberManOwner->GetComponent<BombManagerComponent>()->PendToKill(m_pGameObject);
	m_pGameObject->GetScene()->PendToRemoveChild(m_pGameObject);
	PlayExplosionSound();

}

bool BombComponent::GetIsBlockingBomberman() const
{
	return m_IsBlockingBomberman;
}

void BombComponent::MoveBomb()
{
	TransformComponent *transComp = GetTransform();
	XMFLOAT3 originalPos{ transComp->GetWorldPosition() };
	XMFLOAT3 translatedPos{ originalPos.x + m_Velocity.x, originalPos.y + m_Velocity.y, originalPos.z + m_Velocity.z };
	transComp->Translate(translatedPos);

	int rowIdxOriginal = int((roundf(originalPos.z) - m_Grids.size() / 2) * -1);
	int colIdxOriginal = int(roundf(originalPos.x) + m_Grids[0].size() / 2);
	int rowIdxNew = int((roundf(translatedPos.z) - m_Grids.size() / 2) * -1);
	int colIdxNew = int(roundf(translatedPos.x) + m_Grids[0].size() / 2);
	m_CurrentRow = rowIdxNew;
	m_CurrentColumn = colIdxNew;

	GameObject *pBomb = m_pGameObject;
	GameObject *pPossibleOtherBomb = m_Grids[rowIdxNew][colIdxNew].pBomb;

	IsCollidingWithBoxAndBomb(rowIdxNew, colIdxNew, translatedPos);

	if (pPossibleOtherBomb != pBomb)
	{
		m_Grids[rowIdxNew][colIdxNew].pBomb = pBomb;
		m_Grids[rowIdxOriginal][colIdxOriginal].pBomb = nullptr;
	}
}

void BombComponent::IsCollidingWithBoxAndBomb(int rowIdx, int colIdx, XMFLOAT3 position)
{
	Grid currGrid = m_Grids[rowIdx][colIdx];
	float xSpeed{ m_Velocity.x };
	float zSpeed{ m_Velocity.z };

	if (xSpeed != 0.0f)
	{
		if (xSpeed < 0.0f)
		{
			if (position.x < currGrid.position.x && (m_Grids[rowIdx][colIdx - 1].pBomb || m_Grids[rowIdx][colIdx - 1].pBox))
			{
				GetTransform()->Translate(currGrid.position.x, position.y, currGrid.position.z);
				m_IsMoving = false;
			}
		}
		else
		{
			if (position.x > currGrid.position.x && (m_Grids[rowIdx][colIdx + 1].pBomb || m_Grids[rowIdx][colIdx + 1].pBox))
			{
				GetTransform()->Translate(currGrid.position.x, position.y, currGrid.position.z);
				m_IsMoving = false;
			}
		}
	}
	else if(zSpeed != 0.0f)
	{
		if (zSpeed < 0.0f)
		{
			if (position.z < currGrid.position.z && (m_Grids[rowIdx + 1][colIdx].pBomb || m_Grids[rowIdx + 1][colIdx].pBox))
			{
				GetTransform()->Translate(currGrid.position.x, position.y, currGrid.position.z);
				m_IsMoving = false;
			}
		}
		else
		{
			if (position.z > currGrid.position.z && (m_Grids[rowIdx - 1][colIdx].pBomb || m_Grids[rowIdx - 1][colIdx].pBox))
			{
				GetTransform()->Translate(currGrid.position.x, position.y, currGrid.position.z);
				m_IsMoving = false;
			}
		}
	}
}

void BombComponent::PlayExplosionSound()
{
	SoundManager *pSoundManager = SoundManager::GetInstance();
	pSoundManager->Play2DSound(0, 0, 0, false, "Resources/Sound/bombExplosion.wav");
}