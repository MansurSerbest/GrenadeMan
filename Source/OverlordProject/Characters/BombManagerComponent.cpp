#include "../OverlordEngine/Base/stdafx.h"
#include "BombManagerComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include "../OverlordEngine/Components/ModelComponent.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "../Materials/DiffuseMaterial.h"
#include "../OverlordEngine/Content/ContentManager.h"
#include "../OverlordEngine/Graphics/MeshFilter.h"
#include "BombComponent.h"
#include "../OverlordEngine/Components/ParticleEmitterComponent.h"
#include "ExplosionComponent.h"
#include "../OverlordEngine/Components/ColliderComponent.h"
#include "../OverlordEngine/Components/RigidBodyComponent.h"
#include "../OverlordEngine/Physx/PhysxProxy.h"
#include "../OverlordEngine/Physx/PhysxManager.h"
#include "MovementComponent.h"
#include "../OverlordEngine/Base/SoundManager.h"

BombManagerComponent::BombManagerComponent(std::vector<std::vector<Grid>>& grids, int playerIdx, bool isMaterialInitialized)
	:m_BombCapacity{1}, m_BombPower{1},
	m_Grids{ grids }, m_NrActiveBombs{0}, m_PlayerIdx{playerIdx}, m_MaterialIdOffset{7}, m_IsMaterialInitialized{ isMaterialInitialized }
{

}
BombManagerComponent::~BombManagerComponent()
{

}

void BombManagerComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	EarlyInitializeMeshModelAndMaterial(gameContext);
	
}
void BombManagerComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	for (auto pObject : m_PendingToKillObjects)
	{
		delete pObject;
		pObject = nullptr;
	}
	m_PendingToKillObjects.clear();
}
void BombManagerComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void BombManagerComponent::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BombManagerComponent::EarlyInitializeMeshModelAndMaterial(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//Mesh
	ContentManager::Load<MeshFilter>(L"Resources/Meshes/Bomb.ovm"); 
	// This is just to early initialize it together when bomberman is initialized
	// Allocating this mesh the first thime bomberman drops his first bomb will be too slow for a smooth gameplay experience

	//Color
	switch (m_PlayerIdx)
	{
	case 0:
		m_ExplosionColor = XMFLOAT4{ 0.0f, 0.2f, 1.0f, 1.0f };
		break;
	case 1:
		m_ExplosionColor = XMFLOAT4{ 0.2f, 0.2f, 0.2f, 1.0f };
		break;
	case 2:
		m_ExplosionColor = XMFLOAT4{ 1.0f, 1.0f, 0.2f, 1.0f };
		break;
	case 3: 
		m_ExplosionColor = XMFLOAT4{ 1.0f, 0.2f, 0.2f, 1.0f };
		break;
	}
}

void BombManagerComponent::DropBomb(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//if(m_ActiveBombs.size() < m_BombCapacity)
	if(m_NrActiveBombs < m_BombCapacity)
	{
		// Check if there's already a bomb in the current grid
		const XMFLOAT3 ownerPos{ m_pGameObject->GetTransform()->GetWorldPosition() };
		int rowIdx = int((roundf(ownerPos.z) - m_Grids.size() / 2) * -1);
		int colIdx = int(roundf(ownerPos.x) + m_Grids[0].size() / 2);

		if (!m_Grids[rowIdx][colIdx].pBomb)
		{
			//Drop Bomb
			GameScene *pScene = m_pGameObject->GetScene();
			GameObject *pBomb = new GameObject();
			TransformComponent *transformComp = pBomb->GetTransform();
			m_Grids[rowIdx][colIdx].pBomb = pBomb;

			transformComp->Translate(roundf(ownerPos.x), m_Grids[rowIdx][colIdx].position.y, roundf(ownerPos.z)); // Bomb needs to be centered in the grid!
			transformComp->Rotate(90.0f, 0.0f, 0.0f);
			float scale{ 0.07f };
			transformComp->Scale(scale, scale, scale);

			ModelComponent *pModelComp = new ModelComponent(L"Resources/Meshes/Bomb.ovm");
			pModelComp->SetMaterial(m_MaterialIdOffset + m_PlayerIdx);
			pBomb->AddComponent(pModelComp);
			pBomb->AddComponent(new BombComponent(m_pGameObject, m_BombPower, m_Grids, rowIdx, colIdx));

			pScene->PendToAddChild(pBomb);
			++m_NrActiveBombs;

			PlayDropBombSound();
		}
	}
}

void BombManagerComponent::PendToKill(GameObject *pObject)
{
	m_PendingToKillObjects.push_back(pObject);
}

void BombManagerComponent::IncreaseBombCapacity(unsigned int toAddCapacity)
{
	m_BombCapacity += toAddCapacity;
}
void BombManagerComponent::IncreaseBombPower(int toAddBombPower)
{
	m_BombPower += toAddBombPower;
}

void BombManagerComponent::ExplodeBomb(GameObject* pBomb, int rowIdx, int colIdx)
{
	UNREFERENCED_PARAMETER(pBomb);
	--m_NrActiveBombs;

	Grid& grid = m_Grids[rowIdx][colIdx];
	//if (!grid.pExplosion)
	//{
		CreateExplosion(grid);
	//}

	ExpandExplosion(rowIdx, colIdx);
}

void BombManagerComponent::ExpandExplosion(int rowIdx, int colIdx)
{
	
	bool blockLeftExpansion{ false };
	bool blockRightExpansion{ false };
	bool blockUpExpansion{ false };
	bool blockDownExpansion{ false };

	for (int offset{ 1 }; offset <= m_BombPower; ++offset)
	{
		//Check Left
		if (!blockLeftExpansion)
		{
			if (!m_Grids[rowIdx][colIdx - offset].isWall)
			{
				m_Grids[rowIdx][colIdx - offset].pBox ? blockLeftExpansion = true : blockLeftExpansion = false;/* Put this before CreateExpandingExplosion Because when you call
				CreateExpandingExplosion, you initialize the explosionComponent of the object you created. There it will set the pBox pointer  of the current grid to nullptr.
				That's why, if you call this condition currGrid.pBox after CreateExpandingExplosion, you will always get nullptr because it is destroyed by the Explosion already*/
				CreateExplosion(m_Grids[rowIdx][colIdx - offset]);
			}
			else
			{
				blockLeftExpansion = true;
			}
		}
		//Check Right
		if (!blockRightExpansion)
		{
			if (!m_Grids[rowIdx][colIdx + offset].isWall)
			{
				m_Grids[rowIdx][colIdx + offset].pBox ? blockRightExpansion = true : blockRightExpansion = false;
				CreateExplosion(m_Grids[rowIdx][colIdx + offset]);
			}
			else
			{
				blockRightExpansion = true;
			}
		}
		//Check Up
		if (!blockUpExpansion)
		{
			if (!m_Grids[rowIdx - offset][colIdx].isWall)
			{
				m_Grids[rowIdx - offset][colIdx].pBox ? blockUpExpansion = true : blockUpExpansion = false;
				CreateExplosion(m_Grids[rowIdx - offset][colIdx]);
			}
			else
			{
				blockUpExpansion = true;
			}
		}
		//Check Down
		if (!blockDownExpansion)
		{
			if (!m_Grids[rowIdx + offset][colIdx].isWall)
			{
				m_Grids[rowIdx + offset][colIdx].pBox ? blockDownExpansion = true : blockDownExpansion = false;
				CreateExplosion(m_Grids[rowIdx + offset][colIdx]);
			}
			else
			{
				blockDownExpansion = true;
			}
		}
	}
}

void BombManagerComponent::CreateExplosion(Grid &grid)
{
	// If in grid is already explosion, don't create an extra explosion!!!. There will exist an object you don't have acces to via the Grid Object.
	// If you create an explosion in the grid where already an explosion exists, you will assign the pExplosion to the new explosion. What happens to the 
	// other explosion? Right, you don't have acces to it anymore via the grid. That's why in the Reset() function in Level1, you don't delete all the explosions

	if (!grid.pExplosion)
	{
		//Particle Emitter
		GameObject *pExplosionObject = new GameObject();
		XMFLOAT3 explosionPos = grid.position;
		float scale = 0.1f;
		pExplosionObject->GetTransform()->Translate(explosionPos.x, explosionPos.y, explosionPos.z);
		pExplosionObject->GetTransform()->Scale(scale, scale, scale);

		ParticleEmitterComponent *pParticleEmitterComponent = new ParticleEmitterComponent(L"./Resources/Textures/Bomb/Particles/ExplosionParticle2.png", 60);
		pParticleEmitterComponent->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		pParticleEmitterComponent->SetMinSize(1.5f);
		pParticleEmitterComponent->SetMaxSize(2.0f);
		pParticleEmitterComponent->SetMinEnergy(0.4f);
		pParticleEmitterComponent->SetMaxEnergy(0.5f);
		pParticleEmitterComponent->SetMinSizeGrow(5.0f);
		pParticleEmitterComponent->SetMaxSizeGrow(6.0f);
		pParticleEmitterComponent->SetMinEmitterRange(0.0f);
		pParticleEmitterComponent->SetMaxEmitterRange(0.6f);
		pParticleEmitterComponent->SetColor(m_ExplosionColor);
		pExplosionObject->AddComponent(pParticleEmitterComponent);
		//ExplosionComponent *explosionComponent = new ExplosionComponent(m_pGameObject);
		pExplosionObject->AddComponent(new ExplosionComponent(m_pGameObject, grid));
		m_pGameObject->GetScene()->PendToAddChild(pExplosionObject);
		grid.pExplosion = pExplosionObject;
	}
	else
	{
		grid.pExplosion->GetComponent<ExplosionComponent>()->ResetLifeTimeLeft();
	}
}

void BombManagerComponent::PlayDropBombSound()
{
	SoundManager *pSoundManager = SoundManager::GetInstance();
	pSoundManager->Play2DSound(2, 2, false, false, "Resources/Sound/dropBomb.wav");

}

