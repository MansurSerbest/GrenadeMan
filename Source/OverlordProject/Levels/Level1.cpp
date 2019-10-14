#include "../../OverlordEngine/Base/stdafx.h"
#include "Level1.h"
#include "../../OverlordEngine/Physx/PhysxManager.h"
#include "../../OverlordEngine/Physx/PhysxProxy.h"
#include "../../OverlordEngine/Scenegraph/GameObject.h"
#include "../../OverlordEngine/Components/Components.h"
#include "Graphics\Material.h"
#include "../Materials/DiffuseMaterial.h"
#include "../../OverlordEngine/Base/MaterialManager.h"
#include "../../OverlordEngine/Prefabs/FixedCamera.h"
#include "../../OverlordEngine/Scenegraph/SceneManager.h"
#include "../../OverlordEngine/Graphics/TextRenderer.h"
#include "../../OverlordEngine/Graphics/SpriteFont.h"
#include "../../OverlordEngine/Content/ContentManager.h"
#include "../../OverlordEngine/Graphics/ModelAnimator.h"
#include "../Materials/SkinnedDiffuseMaterial.h"
#include "../../OverlordEngine/Components/ControllerComponent.h"
#include "../Characters/InputComponent.h"
#include "../Characters/MovementComponent.h"
#include "../OverlordEngine/Components/RigidBodyComponent.h"
#include "../OverlordEngine/Components/ColliderComponent.h"
#include "../OverlordEngine/Prefabs/FixedCamera.h"
#include "../OverlordEngine/Components/CameraComponent.h"
#include "../Camera/BomberManCamera.h"
#include "../OverlordEngine/Diagnostics/DebugRenderer.h"
#include "../OverlordProject/Characters/BombManagerComponent.h"
#include "../Characters/HealthAndScoreComponent.h"
#include "../Characters/InteractionComponent.h"
#include "GameData.h"
#include "../../OverlordEngine/Base/SoundManager.h"
#include "../../OverlordEngine/Components/SpriteComponent.h"
#include "../../OverlordEngine/Base/OverlordGame.h"
#include "StartMenu.h"
#include "../Materials/PostBlur.h"
#include "../../OverlordEngine/Components/ParticleEmitterComponent.h"
#include <sstream>
#include "../Materials/PostTimer.h"

Level1::Level1()
	:GameScene(L"Level1")
	,m_pExteriorWall{nullptr}, m_GridSize{1.0f}, m_GridWidth{15}, m_GridLength{11}
	, m_pScoreSpriteFont{ nullptr }, m_pBomberManCamera{ nullptr }, m_pGroundPlane{ nullptr }, m_pGroundGrid{ nullptr }, m_BomberMans{}
	, m_CurrLevelState{}, m_PrevLevelState{}, m_InputIdOffset{ 100 }, m_IsInitialized{ false },
	m_BomberManHUD{}, m_pTimerHUD{nullptr}, m_LevelTimer{120.0f}, m_LevelTimeLeft{ m_LevelTimer },
	m_pPauseMenuBackground{nullptr}, m_pRestartButton{ nullptr }, m_pMainMenuButton{ nullptr }, m_pExitButton{ nullptr },
	m_EndGameDelayTimer{1.0f}, m_EndGameDelayTimeLeft{ m_EndGameDelayTimer }, m_IsDelayStart{}, m_WinnerBomberManIdxs{},
	m_KillStartTime{ 30.0f }, m_BoxSpawnTimer{ m_KillStartTime  / 117.0f}, m_BoxSpawnTimerLeft{ m_BoxSpawnTimer },
	m_CurrKillRow{1}, m_CurrKillCol{1}, m_KillBoxDirectionState{}, m_CycleCount{}

{
	
}
Level1::~Level1()
{

}

void Level1::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	if (!m_IsInitialized)
	{
		//Physics
		GetPhysxProxy()->EnablePhysxDebugRendering(true);
		DebugRenderer::SetDebugRenderer(false);

		//Object Initializions
		InitializeMaterials(gameContext);
		InitializeGroundPlane(gameContext);
		InitializeGrids(gameContext);
		InitializeBomberMans(gameContext);
		InitializeHUD(gameContext);
		InitializePickUpAssets(gameContext);

		//Set up Camera
		m_pBomberManCamera = new BomberManCamera();
		PendToAddChild(m_pBomberManCamera);

		//Set up Fonts
		m_pScoreSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/Fonts/CalangoRevi_32.fnt");

		//Play Sound
		PlayBackGroundMusic();

		//Initialize input
		InitalizePauseInputs(gameContext);

		//Initialize PauseMenu
		InitializePauseObjects(gameContext);
	}
}

void Level1::Update(const GameContext& gameContext)
{
	switch (m_CurrLevelState)
	{
		case Level1::LevelState::play:
		{
			UpdatTimer(gameContext);
			CheckPauseLevel(gameContext);
			CheckBombermansHealth(gameContext);
			break;
		}
		case Level1::LevelState::pause:
		{
			ProcessPauseMenuInput(gameContext);
			break;
		}
		case Level1::LevelState::killMode:
		{
			UpdatTimer(gameContext);
			CheckPauseLevel(gameContext);
			CheckBombermansHealth(gameContext);
			DropKillBoxes(gameContext);
			break;
		}
		case Level1::LevelState::end:
		{
			ProcessEndMenuInput(gameContext);
			break;
		}
	}
}

void Level1::Draw(const GameContext& gameContext)
{

	switch (m_CurrLevelState)
	{
	case Level1::LevelState::play:
		DrawBomberManScores(gameContext);
		DrawTimerText(gameContext);
		break;
	case Level1::LevelState::killMode:
		DrawBomberManScores(gameContext);
		DrawTimerText(gameContext);
		break;
	case Level1::LevelState::pause:
		break;
	case Level1::LevelState::end:
		DrawWinnerInfo(gameContext);
		break;
	default:
		break;
	}

}

void Level1::SceneActivated()
{
	if (m_IsInitialized)
	{
		Reset(GetGameContext());
	}
	else
	{
		m_IsInitialized = true;
	}
}

void Level1::InitializeMaterials(const GameContext& gameContext)
{
	//Initialize Material
	DiffuseMaterial *wallMaterial = new DiffuseMaterial();
	wallMaterial->SetDiffuseTexture(L"Resources/Textures/Box/handpaintedwall2.png");
	gameContext.pMaterialManager->AddMaterial(wallMaterial, 0);

	DiffuseMaterial *boxMaterial = new DiffuseMaterial();
	boxMaterial->SetDiffuseTexture(L"Resources/Textures/Box/BoxTex1.jpg");
	gameContext.pMaterialManager->AddMaterial(boxMaterial, 1);

	DiffuseMaterial *groundMaterial = new DiffuseMaterial();
	groundMaterial->SetDiffuseTexture(L"Resources/Textures/Box/FloorWood.png");
	gameContext.pMaterialManager->AddMaterial(groundMaterial, 2);

	DiffuseMaterial *groundGridMaterial = new DiffuseMaterial();
	groundGridMaterial->SetDiffuseTexture(L"Resources/Textures/Box/GroundGrid.png");
	gameContext.pMaterialManager->AddMaterial(groundGridMaterial, 20);

	DiffuseMaterial *killBoxMaterial = new DiffuseMaterial();
	killBoxMaterial->SetDiffuseTexture(L"Resources/Textures/Box/KillBox.png");
	gameContext.pMaterialManager->AddMaterial(killBoxMaterial, 21);

	//MaterialID 3-6 : BomberMan SkinnedDiffuseMaterial
	int materialOffset{ 3 };
	for (int playerIdx{ 0 }; playerIdx < 4; ++playerIdx)
	{
		int MaterialIdx{ materialOffset + playerIdx };
		SkinnedDiffuseMaterial *bombermanMaterial = new SkinnedDiffuseMaterial();
		std::wstring filePath{ L"Resources/Textures/Character/CharText" };
		filePath.append(std::to_wstring(playerIdx) + L".png");
		bombermanMaterial->SetDiffuseTexture(filePath);
		gameContext.pMaterialManager->AddMaterial(bombermanMaterial, MaterialIdx);
	}

	//Bomb Materials
	int bombMaterialIdOffset{ 7 };
	for (int bombIdx{ 0 }; bombIdx < 4; ++bombIdx)
	{
		DiffuseMaterial *bombMaterial = new DiffuseMaterial();
		std::wstring filePath{ L"Resources/Textures/Bomb/BombDiffuse" };
		filePath.append(std::to_wstring(bombIdx) + L".png");
		bombMaterial->SetDiffuseTexture(filePath);
		gameContext.pMaterialManager->AddMaterial(bombMaterial, bombMaterialIdOffset + bombIdx);
	}

	//Post Process
	gameContext.pMaterialManager->AddMaterial_PP(new PostBlur(), 0);
	gameContext.pMaterialManager->AddMaterial_PP(new PostTimer(), 1);
	AddPostProcessingMaterial(1);
}

void Level1::InitializeGroundPlane(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto defaultMaterial = physX->createMaterial(0.5f, 0.5f, 1.0f);

	m_pGroundPlane = new GameObject();
	m_pGroundPlane->AddComponent(new RigidBodyComponent(true));
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	m_pGroundPlane->AddComponent(new ColliderComponent(geom, *defaultMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	ModelComponent *modComp = new ModelComponent(L"Resources/Meshes/UnitPlane.ovm");
	modComp->SetMaterial(2);
	m_pGroundPlane->AddComponent(modComp);
	m_pGroundPlane->GetTransform()->Scale(3.0f, 1.0f, 3.0f);
	PendToAddChild(m_pGroundPlane);

	m_pGroundGrid = new GameObject();
	modComp = new ModelComponent(L"Resources/Meshes/UnitPlane.ovm");
	modComp->SetMaterial(20);
	m_pGroundGrid->AddComponent(modComp);
	m_pGroundGrid->GetTransform()->Translate(0.0f, 0.01f, 0.0f);
	m_pGroundGrid->GetTransform()->Scale(1.5f, 1.0f, 1.1f);
	PendToAddChild(m_pGroundGrid);
}

void Level1::InitializeGrids(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	float initPosX{-m_GridWidth / 2.0f + m_GridSize / 2.0f};
	float initPosZ{m_GridLength / 2.0f - m_GridSize / 2.0f};

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = physX->createMaterial(0.5f, 0.5f, 1.0f);

	//Init all grids ass walls
	m_Grids.resize((unsigned int)m_GridLength);
	for (int row{}; row < m_GridLength; ++row)
	{
		for (int col{}; col < m_GridWidth; ++col)
		{
			Grid grid;
			//Box
			GameObject *box = new GameObject();
			//Position
			grid.position = XMFLOAT3{ initPosX + col * m_GridSize, m_GridSize / 2.0f, initPosZ - row * m_GridSize };
			box->GetTransform()->Translate(initPosX + col * m_GridSize, m_GridSize / 2.0f, initPosZ - row * m_GridSize);
			//Components
			ModelComponent *pModelComponent = new ModelComponent(L"Resources/Meshes/Box4.ovm");
			box->AddComponent(pModelComponent);
			pModelComponent->SetMaterial(0);
			RigidBodyComponent *pRidigBodyComponent = new RigidBodyComponent(true);
			box->AddComponent(pRidigBodyComponent);
			float boxScale{0.9f};
			PxConvexMesh *convexMesh = ContentManager::Load<PxConvexMesh>(L"Resources/Meshes/Box4.ovpc");
			std::shared_ptr<PxGeometry> geom = std::make_shared<PxConvexMeshGeometry>(convexMesh, PxMeshScale(boxScale));
			ColliderComponent *pColliderComponent = new ColliderComponent(geom, *pDefaultMaterial);
			box->AddComponent(pColliderComponent);
			box->GetTransform()->Scale(boxScale, boxScale, boxScale);
			PendToAddChild(box);
			grid.pBox = box;
			//Flag:: is wall or not?
			grid.isWall = true;
			m_Grids[row].push_back(grid);
		}
	}

	//Initialize all the boxes
	for (int col{1}; col < m_GridWidth - 1; ++col)
	{
		m_Grids[1][col].isWall = false;
		m_Grids[1][col].pBox->GetComponent<ModelComponent>()->SetMaterial(1);
	}
	for (int col{ 1 }; col < m_GridWidth - 1; ++col)
	{
		m_Grids[m_GridLength - 2][col].isWall = false;
		m_Grids[m_GridLength - 2][col].pBox->GetComponent<ModelComponent>()->SetMaterial(1);
	}
	for (int row{ 1 }; row < m_GridLength - 1; ++row)
	{
		m_Grids[row][1].isWall = false;
		m_Grids[row][1].pBox->GetComponent<ModelComponent>()->SetMaterial(1);
	}
	for (int row{ 1 }; row < m_GridLength - 1; ++row)
	{
		m_Grids[row][m_GridWidth - 2].isWall = false;
		m_Grids[row][m_GridWidth - 2].pBox->GetComponent<ModelComponent>()->SetMaterial(1);
	}
	for (int row{2}; row < m_GridLength - 2; ++row)
		for (int col{ 2 }; col < m_GridWidth - 2; ++col)
		{
			if (!(row % 2 == 0 && col % 2 == 0))
			{
				m_Grids[row][col].isWall = false;
				m_Grids[row][col].pBox->GetComponent<ModelComponent>()->SetMaterial(1);
			}
		}

	//Initialize all the empty spaces
	PendToRemoveChild(m_Grids[1][1].pBox);
	m_Grids[1][1].pBox = nullptr;
	PendToRemoveChild(m_Grids[1][2].pBox);
	m_Grids[1][2].pBox = nullptr;
	PendToRemoveChild(m_Grids[2][1].pBox);
	m_Grids[2][1].pBox = nullptr;

	PendToRemoveChild(m_Grids[1][m_GridWidth - 2].pBox);
	m_Grids[1][m_GridWidth - 2].pBox = nullptr;
	PendToRemoveChild(m_Grids[1][m_GridWidth - 3].pBox);
	m_Grids[1][m_GridWidth - 3].pBox = nullptr;
	PendToRemoveChild(m_Grids[2][m_GridWidth - 2].pBox);
	m_Grids[2][m_GridWidth - 2].pBox = nullptr;

	PendToRemoveChild(m_Grids[m_GridLength - 2][1].pBox);
	m_Grids[m_GridLength - 2][1].pBox = nullptr;
	PendToRemoveChild(m_Grids[m_GridLength - 2][2].pBox);
	m_Grids[m_GridLength - 2][2].pBox = nullptr;
	PendToRemoveChild(m_Grids[m_GridLength - 3][1].pBox);
	m_Grids[m_GridLength - 3][1].pBox = nullptr;

	PendToRemoveChild(m_Grids[m_GridLength - 2][m_GridWidth - 2].pBox);
	m_Grids[m_GridLength - 2][m_GridWidth - 2].pBox = nullptr;
	PendToRemoveChild(m_Grids[m_GridLength - 2][m_GridWidth - 3].pBox);
	m_Grids[m_GridLength - 2][m_GridWidth - 3].pBox = nullptr;
	PendToRemoveChild(m_Grids[m_GridLength - 3][m_GridWidth - 2].pBox);
	m_Grids[m_GridLength - 3][m_GridWidth - 2].pBox = nullptr;
}

void Level1::InitializePickUpAssets(const GameContext& gameContext)
{
	DiffuseMaterial *pPickUpMaterialSpeed = new DiffuseMaterial();
	pPickUpMaterialSpeed->SetDiffuseTexture(L"Resources/Textures/HUD/SpeedUpTex.png");
	gameContext.pMaterialManager->AddMaterial(pPickUpMaterialSpeed, 11);

	DiffuseMaterial *pPickUpMaterialBombCapacity = new DiffuseMaterial();
	pPickUpMaterialBombCapacity->SetDiffuseTexture(L"Resources/Textures/HUD/BombCapacityTex.png");
	gameContext.pMaterialManager->AddMaterial(pPickUpMaterialBombCapacity, 12);

	DiffuseMaterial *pPickUpMaterialBombPower = new DiffuseMaterial();
	pPickUpMaterialBombPower->SetDiffuseTexture(L"Resources/Textures/HUD/BombPowerUpTex.png");
	gameContext.pMaterialManager->AddMaterial(pPickUpMaterialBombPower, 13);

	ContentManager::Load<MeshFilter>(L"Resources/Meshes/UnitPlane.ovm");

}

void Level1::InitializeBomberMans(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GameData &gameData = GameData::GetInstance();
	int nrOfPlayers{ gameData.GetNrOfPlayers() };
	for (int i{}; i < nrOfPlayers; ++i)
		m_WinnerBomberManIdxs.push_back(true);

	m_BomberMans.reserve(nrOfPlayers);
	for (int playerIdx{ 0 }; playerIdx < nrOfPlayers; playerIdx++)
	{
		GameObject *pBomberMan = new GameObject();

		int MaterialIdx{ 3 + playerIdx };	
		ModelComponent *pModelComponent = new ModelComponent(L"Resources/Meshes/Character/Character2.ovm");
		pModelComponent->SetMaterial(MaterialIdx);
		pBomberMan->AddComponent(pModelComponent);


		XMFLOAT3 initPos{};
		pBomberMan->GetTransform()->Scale(0.08f, 0.08f, 0.08f);
		switch (playerIdx)
		{
		case 0:
			initPos = m_Grids[1][1].position;
			break;
		case 1:
			initPos = m_Grids[1][m_GridWidth - 2].position;
			break;
		case 2:
			initPos = m_Grids[m_GridLength - 2][1].position;
			break;
		case 3:
			initPos = m_Grids[m_GridLength - 2][m_GridWidth - 2].position;
			break;
		}


		PxMaterial *pControllerMaterial = PhysxManager::GetInstance()->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);
		//ControllerComponent *pControllerComponent = new ControllerComponent(pControllerMaterial, 0.53f, 0.4f, L"Bomberman", PxCapsuleClimbingMode::eCONSTRAINED);
		ControllerComponent *pControllerComponent = new ControllerComponent(pControllerMaterial, 0.4f, 0.4f, L"Bomberman", PxCapsuleClimbingMode::eCONSTRAINED);
		pControllerComponent->SetCollisionGroup(Group0);

		pBomberMan->AddComponent(new InputComponent(playerIdx));
		pBomberMan->AddComponent(new MovementComponent(m_Grids));
		pBomberMan->AddComponent(new BombManagerComponent(m_Grids, playerIdx, m_IsInitialized));
		pBomberMan->AddComponent(new HealthAndScoreComponent());
		pBomberMan->AddComponent(new InteractionComponent(m_Grids));

		pBomberMan->AddComponent(pControllerComponent);
		PendToAddChild(pBomberMan);

		pControllerComponent->SetPivot(XMFLOAT3{ 0.0f, -0.5f, 0.0f });
		pControllerComponent->Translate(initPos.x, 0.7f, initPos.z);

		m_BomberMans.push_back(pBomberMan);
	}
}

void Level1::InitializeHUD(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	int nrOfPlayers{ GameData::GetInstance().GetNrOfPlayers() };
	const GameSettings &gameSettings = OverlordGame::GetGameSettings();
	int windowWidth{ gameSettings.Window.Width };
	int windowHeight{ gameSettings.Window.Height };
	float scale{ 0.3f };

	for (int playerIdx{ 0 }; playerIdx < nrOfPlayers; ++playerIdx)
	{

		float OffsetX{ 300.0f  * scale };
		float OffsetY{ 500.0f  * scale };

		switch (playerIdx)
		{
			case 0:
			{
				GameObject *pPlayerHUD1 = new GameObject();
				SpriteComponent *pSpriteComponent = new SpriteComponent(L"Resources/Textures/HUD/Player1InGame.png", XMFLOAT2{ 0.5f, 0.5f });
				pPlayerHUD1->AddComponent(pSpriteComponent);
				PendToAddChild(pPlayerHUD1);
				pPlayerHUD1->GetTransform()->Translate(OffsetX, OffsetY, 0.1f);
				pPlayerHUD1->GetTransform()->Scale(scale, scale, scale);
				m_BomberManHUD.push_back(pPlayerHUD1);
				break;
			}
			case 1:
			{
				GameObject *pPlayerHUD2 = new GameObject();
				SpriteComponent *pSpriteComponent = new SpriteComponent(L"Resources/Textures/HUD/Player2InGame.png", XMFLOAT2{ 0.5f, 0.5f });
				pPlayerHUD2->AddComponent(pSpriteComponent);
				PendToAddChild(pPlayerHUD2);
				pPlayerHUD2->GetTransform()->Translate(windowWidth - OffsetX, OffsetY, 0.1f);
				pPlayerHUD2->GetTransform()->Scale(scale, scale, scale);
				m_BomberManHUD.push_back(pPlayerHUD2);
				break;
			}
			case 2:
			{
				GameObject *pPlayerHUD3 = new GameObject();
				SpriteComponent *pSpriteComponent = new SpriteComponent(L"Resources/Textures/HUD/Player3InGame.png", XMFLOAT2{ 0.5f, 0.5f });
				pPlayerHUD3->AddComponent(pSpriteComponent);
				PendToAddChild(pPlayerHUD3);
				pPlayerHUD3->GetTransform()->Translate(OffsetX, windowHeight - OffsetY, 0.1f);
				pPlayerHUD3->GetTransform()->Scale(scale, scale, scale);
				m_BomberManHUD.push_back(pPlayerHUD3);
				break;
			}
			case 3:
			{
				GameObject *pPlayerHUD4 = new GameObject();
				SpriteComponent *pSpriteComponent = new SpriteComponent(L"Resources/Textures/HUD/Player4InGame.png", XMFLOAT2{ 0.5f, 0.5f });
				pPlayerHUD4->AddComponent(pSpriteComponent);
				PendToAddChild(pPlayerHUD4);
				pPlayerHUD4->GetTransform()->Translate(windowWidth - OffsetX, windowHeight - OffsetY, 0.1f);
				pPlayerHUD4->GetTransform()->Scale(scale, scale, scale);
				m_BomberManHUD.push_back(pPlayerHUD4);
				break;
			}
		}
	}

	//Initialize Timer Sprite
	float timerHUDScale{ 0.5f };
	m_pTimerHUD = new GameObject();
	SpriteComponent *pSpriteComponet = new SpriteComponent(L"Resources/Textures/HUD/TimerBar.png", XMFLOAT2{ 0.5f, 0.25f });
	m_pTimerHUD->AddComponent(pSpriteComponet);
	m_pTimerHUD->GetTransform()->Translate(windowWidth / 2.0f, 30.0f, 0.1f);
	m_pTimerHUD->GetTransform()->Scale(timerHUDScale, timerHUDScale, 1.0f);
	PendToAddChild(m_pTimerHUD);
}

void Level1::PlayBackGroundMusic()
{
	SoundManager *pSoundManager = SoundManager::GetInstance();
	pSoundManager->Play2DSound(3, 3, -1, false, "Resources/Sound/PlayBackgroundMusic.mp3");
	pSoundManager->SetChannelVolume(3, 0.05f);
}

void Level1::Reset(const GameContext& gameContext)
{
	m_IsDelayStart = false;

	//PostProcess
	RemovePostProcessingMaterial(0);
	RemovePostProcessingMaterial(1);
	dynamic_cast<PostTimer*>(gameContext.pMaterialManager->GetMaterial_PP(1))->SetIntensity(0.0f);

	//Rest Timer
	m_LevelTimeLeft = m_LevelTimer;

	//ResetBomberman
	m_WinnerBomberManIdxs.clear();
	for (GameObject* pBomberMan : m_BomberMans)
	{
		pBomberMan->GetComponent<InteractionComponent>()->DeleteAllActiveParticles();
		RemoveChild(pBomberMan);
	}
	m_BomberMans.clear();
	InitializeBomberMans(gameContext);

	//Reset HUD
	for (GameObject* pBomberManHUD : m_BomberManHUD)
	{
		RemoveChild(pBomberManHUD);
	}
	m_BomberManHUD.clear();
	InitializeHUD(gameContext);

	//Reset Grid
	for (std::vector<Grid> &gridRow : m_Grids)
		for(Grid &grid : gridRow)
		{
			grid.isWall = false;
			grid.isKillBox = false;
			if (grid.pBomb)
			{
				RemoveChild(grid.pBomb);
				grid.pBomb = nullptr;
			}
			if (grid.pBox)
			{
				RemoveChild(grid.pBox);
				grid.pBox = nullptr;
			}
			if (grid.pExplosion)
			{
				RemoveChild(grid.pExplosion);
				grid.pExplosion = nullptr;
			}
			if (grid.pPickUp)
			{
				RemoveChild(grid.pPickUp);
				grid.pPickUp = nullptr;
			}
			grid.position = {};
		}
	m_Grids.clear();
	InitializeGrids(gameContext);

	//Reset Level State
	m_CurrLevelState = LevelState::play;
	m_IsPaused = false;

	SetPauseMenuVisuals();
	SetEndMenuVisuals(false);

	PlayBackGroundMusic();

	//Reset KillMode Variables
	m_BoxSpawnTimerLeft = m_BoxSpawnTimer;
	m_CurrKillRow = 1;
	m_CurrKillCol = 1;
	m_KillBoxDirectionState = KillBoxDirectionState::right;
	m_CycleCount = 0;
}

void Level1::ExitLevel(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	SoundManager::GetInstance()->Stop2DSound(3, 3);
}

void Level1::UpdatTimer(const GameContext& gameContext)
{
	m_LevelTimeLeft -= gameContext.pGameTime->GetElapsed();

	switch (m_CurrLevelState)
	{
	case Level1::LevelState::play:
		{
			//Timer Red Post Process Effect
			float beginPoint{ m_LevelTimer / 2.0f };
			if (m_LevelTimeLeft < beginPoint)
			{
				auto PostTimerMat = dynamic_cast<PostTimer*>(gameContext.pMaterialManager->GetMaterial_PP(1));
				if (PostTimerMat)
					PostTimerMat->SetIntensity(1.0f - (m_LevelTimeLeft / beginPoint));
			}

			//Check kill time reached
			if (m_LevelTimeLeft < m_KillStartTime)
			{
				for (GameObject *pBomberman : m_BomberMans)
				{
					pBomberman->GetComponent<InteractionComponent>()->ActiveIsKillMode();
				}
				m_CurrLevelState = LevelState::killMode;
				m_PrevLevelState = LevelState::play;
			}

			//Check 1 sec delay after end game is reached (timer or bomberman dead)
			if (m_IsDelayStart)
			{
				m_EndGameDelayTimeLeft -= gameContext.pGameTime->GetElapsed();
				if (m_EndGameDelayTimeLeft <= 0.0f)
				{
					m_EndGameDelayTimeLeft = m_EndGameDelayTimer;

					m_CurrLevelState = LevelState::end;
					SetEndMenuVisuals(true);
					m_LevelTimeLeft = m_LevelTimer;
					m_IsPaused = true;
				}
			}
			break;
		}
	case Level1::LevelState::pause:
		break;
	case Level1::LevelState::killMode:
		{
			//Timer Red Post Process Effect
			float beginPoint{ m_LevelTimer / 2.0f };
			if (m_LevelTimeLeft < beginPoint)
			{
				auto PostTimerMat = dynamic_cast<PostTimer*>(gameContext.pMaterialManager->GetMaterial_PP(1));
				if (PostTimerMat)
					PostTimerMat->SetIntensity(1.0f - (m_LevelTimeLeft / beginPoint));
			}
			//Check End of Level Timer

			if (m_LevelTimeLeft < 0.0f)
			{
				m_CurrLevelState = LevelState::end;
				SetEndMenuVisuals(true);
				m_LevelTimeLeft = m_LevelTimer;
				m_IsPaused = true;
			}

			//Check 1 sec delay after end game is reached (timer or bomberman dead)
			if (m_IsDelayStart)
			{
				m_EndGameDelayTimeLeft -= gameContext.pGameTime->GetElapsed();
				if (m_EndGameDelayTimeLeft <= 0.0f)
				{
					m_EndGameDelayTimeLeft = m_EndGameDelayTimer;

					m_CurrLevelState = LevelState::end;
					SetEndMenuVisuals(true);
					m_LevelTimeLeft = m_LevelTimer;
					m_IsPaused = true;
				}
			}
			break;
		}
	case Level1::LevelState::end:
		break;
	}






}

void Level1::DrawBomberManScores(const GameContext& gameContext)
{
	if (!m_IsPaused)
	{
		UNREFERENCED_PARAMETER(gameContext);
		float scale{ 0.3f };
		float OffsetX{ 300.0f  * scale };
		float OffsetY{ 500.0f  * scale };
		float textOffsetX{ 20.0f *scale };
		float textOffsetY{ 40.0f *scale };
		int nrOfPlayers{ GameData::GetInstance().GetNrOfPlayers() };
		const GameSettings &gameSettings = OverlordGame::GetGameSettings();
		int windowWidth{ gameSettings.Window.Width };
		int windowHeight{ gameSettings.Window.Height };


		for (int playerIdx{ 0 }; playerIdx < nrOfPlayers; ++playerIdx)
		{

			switch (playerIdx)
			{
			case 0:
			{
				XMFLOAT4 color = { 0.2f, 0.2f, 1.0f, 1.0f };
				int score = (int)m_BomberMans[playerIdx]->GetComponent<HealthAndScoreComponent>()->GetScore();
				TextRenderer::GetInstance()->DrawText(m_pScoreSpriteFont, std::to_wstring(score),
					XMFLOAT2{ OffsetX + textOffsetX, OffsetY + textOffsetY }, color);
				break;
			}
			case 1:
			{

				XMFLOAT4 color = { 0.2f, 0.2f, 0.2f, 1.0f };
				int score = (int)m_BomberMans[playerIdx]->GetComponent<HealthAndScoreComponent>()->GetScore();
				TextRenderer::GetInstance()->DrawText(m_pScoreSpriteFont, std::to_wstring(score),
					XMFLOAT2{ windowWidth - OffsetX - 10 * textOffsetX, OffsetY + textOffsetY }, color);
				break;
			}
			case 2:
			{
				XMFLOAT4 color = { 1.0f, 1.0f, 0.2f, 1.0f };
				int score = (int)m_BomberMans[playerIdx]->GetComponent<HealthAndScoreComponent>()->GetScore();
				TextRenderer::GetInstance()->DrawText(m_pScoreSpriteFont, std::to_wstring(score),
					XMFLOAT2{ OffsetX + textOffsetX, windowHeight - OffsetY + textOffsetY }, color);
				break;
			}
			case 3:
			{
				XMFLOAT4 color = { 1.0f, 0.2f, 0.2f, 1.0f };
				int score = (int)m_BomberMans[playerIdx]->GetComponent<HealthAndScoreComponent>()->GetScore();
				TextRenderer::GetInstance()->DrawText(m_pScoreSpriteFont, std::to_wstring(score),
					XMFLOAT2{ windowWidth - OffsetX - 10 * textOffsetX, windowHeight - OffsetY + textOffsetY }, color);
				break;
			}
			}
		}
	}

}

void Level1::DrawTimerText(const GameContext& gameContext)
{
	if (!m_IsPaused)
	{
		UNREFERENCED_PARAMETER(gameContext);
		float xOffset{ -23.0f };
		float yOffset{ -5.0f };
		std::wstring time{ };
		std::wstringstream wsStream;
		wsStream.width(3);
		wsStream.fill('0');
		wsStream << std::to_wstring((int)m_LevelTimeLeft);
		getline(wsStream, time);
		
		XMFLOAT4 color = { 0.43f, 0.13f, 0.03f, 1.0f };
		XMFLOAT3 pos{ m_pTimerHUD->GetTransform()->GetWorldPosition() };
		TextRenderer::GetInstance()->DrawText(m_pScoreSpriteFont, time
			, XMFLOAT2{ pos.x + xOffset, pos.y + yOffset }, color);
	}
}

void Level1::CheckBombermansHealth(const GameContext &gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	int counter{};
	for (size_t playerIdx{}; playerIdx < m_BomberMans.size(); ++playerIdx)
	{
		if (m_BomberMans[playerIdx]->GetComponent<HealthAndScoreComponent>()->GetHealth() > 0.0f)
		{
			++counter;			
		}
		else
		{
			m_WinnerBomberManIdxs[playerIdx] = false;
		}
	}

	if (GameData::GetInstance().GetNrOfPlayers() == 1)
	{
		if (counter == 0)
			m_IsDelayStart = true;
	}
	else if(counter == 1 || counter == 0)
		m_IsDelayStart = true;
}

void Level1::InitalizePauseInputs(const GameContext& gameContext)
{
	auto input = gameContext.pInput;
	int nrOfPlayers{ GameData::GetInstance().GetNrOfPlayers() };
	int nrOfButtons{ 4 };

	for (int playerIdx{ 0 }; playerIdx < nrOfPlayers; ++playerIdx)
	{
		switch (playerIdx)
		{
		case 0:
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::pause,
				InputTriggerState::Pressed, 0x50, -1, XINPUT_GAMEPAD_START, GamepadIndex::PlayerOne });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::up,
				InputTriggerState::Pressed, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex::PlayerOne });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::down,
				InputTriggerState::Pressed, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex::PlayerOne });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::select,
				InputTriggerState::Pressed, VK_RETURN, -1, XINPUT_GAMEPAD_A, GamepadIndex::PlayerOne });
			break;
		case 1:
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::pause,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_START, GamepadIndex::PlayerTwo });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::up,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex::PlayerTwo });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::down,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex::PlayerTwo });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::select,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_A, GamepadIndex::PlayerTwo });
			break;
		case 2:
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::pause,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_START, GamepadIndex::PlayerThree });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::up,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex::PlayerThree });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::down,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex::PlayerThree });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::select,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_A, GamepadIndex::PlayerThree });
			break;
		case 3:
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::pause,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_START, GamepadIndex::PlayerFour });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::up,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex::PlayerFour });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::down,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex::PlayerFour });
			input->AddInputAction(InputAction{ m_InputIdOffset + playerIdx * nrOfButtons + (int)PauseInputId::select,
				InputTriggerState::Pressed, -1, -1, XINPUT_GAMEPAD_A, GamepadIndex::PlayerFour });
			break;
		}


	}
}

void Level1::CheckPauseLevel(const GameContext& gameContext)
{
	int nrOfPlayers{ GameData::GetInstance().GetNrOfPlayers() };
	int nrOfInputId{ 4 };

	for (int playerIdx{ 0 }; playerIdx < nrOfPlayers; ++playerIdx)
	{
		if (gameContext.pInput->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::pause))
		{
			m_IsPaused = !m_IsPaused;
			m_PrevLevelState = m_CurrLevelState;
			m_CurrLevelState = LevelState::pause;
			m_CurrentPauseButtonIndex = 0;
			std::cout << "PAUSE" << std::endl;

			SetPauseMenuVisuals();

			GiveVisualFeedBack();
			AddPostProcessingMaterial(0);
		}
	}
}

void Level1::InitializePauseObjects(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	const GameSettings &gameSettings = OverlordGame::GetGameSettings();
	float windowWidth{ (float)gameSettings.Window.Width };
	float windowHeight{ (float)gameSettings.Window.Height };
	float xOffset{ windowWidth / 2.0f };
	float yOffset{ windowHeight / 4.0f };
	

	m_pPauseMenuBackground = new GameObject();
	SpriteComponent *spriteComponent = new SpriteComponent(L"Resources/Textures/HUD/IngamePauseBackGround.png", XMFLOAT2{ 0.5f , 0.5f });
	m_pPauseMenuBackground->AddComponent(spriteComponent);
	PendToAddChild(m_pPauseMenuBackground);
	m_pPauseMenuBackground->GetTransform()->Scale(0.5f, 0.5f, 1.0f);
	m_pPauseMenuBackground->GetTransform()->Translate(windowWidth / 2.0f, windowHeight / 2.0f, 0.9f);
	spriteComponent->SetIsVisible(false);

	float scale{ 0.75f };
	float buttonsSpaceBetween{ 200.0f * scale };
	m_pRestartButton = new GameObject();
	spriteComponent = new SpriteComponent(L"Resources/Textures/HUD/RestartButton.png", XMFLOAT2{ 0.5f , 0.25f });
	m_pRestartButton->AddComponent(spriteComponent);
	PendToAddChild(m_pRestartButton);
	m_pRestartButton->GetTransform()->Translate(xOffset, yOffset, 0.8f);
	m_pRestartButton->GetTransform()->Scale(scale, scale, 1.0f);
	spriteComponent->SetIsVisible(false);

	m_pMainMenuButton = new GameObject();
	spriteComponent = new SpriteComponent(L"Resources/Textures/HUD/MainMenuButton.png", XMFLOAT2{ 0.5f , 0.25f });
	m_pMainMenuButton->AddComponent(spriteComponent);
	PendToAddChild(m_pMainMenuButton);
	m_pMainMenuButton->GetTransform()->Translate(xOffset, yOffset + buttonsSpaceBetween, 0.7f);
	m_pMainMenuButton->GetTransform()->Scale(scale, scale, 1.0f);
	spriteComponent->SetIsVisible(false);

	m_pExitButton = new GameObject();
	spriteComponent = new SpriteComponent(L"Resources/Textures/HUD/ExitButton.dds", XMFLOAT2{ 0.5f , 0.25f });
	m_pExitButton->AddComponent(spriteComponent);
	PendToAddChild(m_pExitButton);
	m_pExitButton->GetTransform()->Translate(xOffset, yOffset + 2 * buttonsSpaceBetween, 0.6f);
	m_pExitButton->GetTransform()->Scale(scale, scale, 1.0f);
	spriteComponent->SetIsVisible(false);

	m_pPlayButton = new GameObject();
	spriteComponent = new SpriteComponent(L"Resources/Textures/HUD/PlayButton.png", XMFLOAT2{ 0.5f , 0.25f });
	m_pPlayButton->AddComponent(spriteComponent);
	PendToAddChild(m_pPlayButton);
	m_pPlayButton->GetTransform()->Translate(xOffset, yOffset, 0.8f);
	m_pPlayButton->GetTransform()->Scale(scale, scale, 1.0f);
	spriteComponent->SetIsVisible(false);

	m_CurrentPauseButtonIndex = 0;
}

void Level1::GiveVisualFeedBack()
{

	switch (m_CurrLevelState)
	{
		case Level1::LevelState::pause:
		{
			switch (m_CurrentPauseButtonIndex)
			{
			case 0:
				m_pRestartButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/RestartButtonFeedBack.png");
				m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButton.png");
				m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButton.png");
				break;
			case 1:
				m_pRestartButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/RestartButton.png");
				m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButtonFeedBack.png");
				m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButton.png");
				break;
			case 2:
				m_pRestartButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/RestartButton.png");
				m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButton.png");
				m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButtonFeedBack.png");
				break;
			}
			break;
		}

		case Level1::LevelState::end:
		{
			switch (m_CurrentPauseButtonIndex)
			{
			case 0:
				m_pPlayButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/PlayButtonFeedBack.png");
				m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButton.png");
				m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButton.png");
				break;
			case 1:
				m_pPlayButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/PlayButton.png");
				m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButtonFeedBack.png");
				m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButton.png");
				break;
			case 2:
				m_pPlayButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/PlayButton.png");
				m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButton.png");
				m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButtonFeedBack.png");
				break;
			}
			break;
		}
	}
}

void Level1::ProcessPauseMenuInput(const GameContext& gameContext)
{
	auto input = gameContext.pInput;
	int nrOfButtons{ 3 };

	int nrOfPlayers{ GameData::GetInstance().GetNrOfPlayers() };
	int nrOfInputId{ 4 };

	for (int playerIdx{ 0 }; playerIdx < nrOfPlayers; ++playerIdx)
	{
		if (input->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::pause))
		{
			m_IsPaused = false;
			m_PrevLevelState == LevelState::play? m_CurrLevelState = LevelState::play : m_CurrLevelState = LevelState::killMode;
			m_PrevLevelState = LevelState::pause;
			SetPauseMenuVisuals();
		}
		else if (input->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::up))
		{
			m_CurrentPauseButtonIndex--;
			if (m_CurrentPauseButtonIndex < 0)
				m_CurrentPauseButtonIndex = nrOfButtons - 1;
			GiveVisualFeedBack();
		}
		else if (input->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::down))
		{
			++m_CurrentPauseButtonIndex %= nrOfButtons;
			GiveVisualFeedBack();
		}
		else if (input->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::select))
		{
			switch (m_CurrentPauseButtonIndex)
			{
			case 0:
				//m_IsPaused = false;
				//m_LevelState = LevelState::play;
				//SetPauseMenuVisuals();
				SoundManager::GetInstance()->Stop2DSound(3, 3);
				Reset(gameContext);
				break;
			case 1:
				SceneManager::GetInstance()->SetActiveGameScene(L"StartMenu");
				SoundManager::GetInstance()->Stop2DSound(3, 3);
				break;
			case 2:
				PostQuitMessage(0);
				break;
			}
		}
	}
}

void Level1::SetPauseMenuVisuals()
{
	m_pPauseMenuBackground->GetComponent<SpriteComponent>()->SetIsVisible(m_IsPaused);
	m_pRestartButton->GetComponent<SpriteComponent>()->SetIsVisible(m_IsPaused);
	m_pMainMenuButton->GetComponent<SpriteComponent>()->SetIsVisible(m_IsPaused);
	m_pExitButton->GetComponent<SpriteComponent>()->SetIsVisible(m_IsPaused);
	m_pTimerHUD->GetComponent<SpriteComponent>()->SetIsVisible(!m_IsPaused);
	for (GameObject *pBomberManHUD : m_BomberManHUD)
	{
		pBomberManHUD->GetComponent<SpriteComponent>()->SetIsVisible(!m_IsPaused);
	}
	if (!m_IsPaused)
	{
		RemovePostProcessingMaterial(0);
		AddPostProcessingMaterial(1);
	}
	else
	{
		RemovePostProcessingMaterial(1);
	}

}

void Level1::DropKillBoxes(const GameContext& gameContext)
{
	int rowBeginIdx{ 1 };
	int rowEndIdx{ 9 };
	int colBeginIdx{ 1 };
	int colEndIdx{ 13 };

	m_BoxSpawnTimerLeft -= gameContext.pGameTime->GetElapsed();
	if (m_BoxSpawnTimerLeft <= 0.0f)
	{
		m_BoxSpawnTimerLeft = m_BoxSpawnTimer;
		Grid &grid = m_Grids[m_CurrKillRow][m_CurrKillCol];
		GameObject *pBox = grid.pBox;
		if (pBox)
		{
			pBox->GetComponent<ModelComponent>()->SetMaterial(21);
			grid.isKillBox = true;
			grid.isWall = true;
			if (grid.pPickUp)
			{
				PendToRemoveChild(grid.pPickUp);
				grid.pPickUp = nullptr;
			}

		}
		else
		{
			//auto physX = PhysxManager::GetInstance()->GetPhysics();
			//auto pDefaultMaterial = physX->createMaterial(0.5f, 0.5f, 1.0f);
			GameObject *pKillBox = new GameObject();

			ModelComponent *pModelComponent = new ModelComponent(L"Resources/Meshes/Box4.ovm");
			pKillBox->AddComponent(pModelComponent);
			pModelComponent->SetMaterial(21);

			//RigidBodyComponent *pRidigBodyComponent = new RigidBodyComponent(true);
			//pKillBox->AddComponent(pRidigBodyComponent);

			float boxScale{ 0.9f };
			//PxConvexMesh *convexMesh = ContentManager::Load<PxConvexMesh>(L"Resources/Meshes/Box4.ovpc");
			//std::shared_ptr<PxGeometry> geom = std::make_shared<PxConvexMeshGeometry>(convexMesh, PxMeshScale(boxScale));
			//ColliderComponent *pColliderComponent = new ColliderComponent(geom, *pDefaultMaterial);
			//pKillBox->AddComponent(pColliderComponent);

			pKillBox->GetTransform()->Scale(boxScale, boxScale, boxScale);
			pKillBox->GetTransform()->Translate(grid.position);
			PendToAddChild(pKillBox);
			grid.pBox = pKillBox;
			grid.isKillBox = true;
			grid.isWall = true;

			if (grid.pPickUp)
			{
				PendToRemoveChild(grid.pPickUp);
				grid.pPickUp = nullptr;
			}

			for (size_t playerIdx{}; playerIdx < m_BomberMans.size(); ++playerIdx)
			{
				m_BomberMans[playerIdx]->GetComponent<InteractionComponent>()->CheckCollisionWithKillBox(m_CurrKillRow, m_CurrKillCol);
			}
		}

		switch (m_KillBoxDirectionState)
		{
		case Level1::KillBoxDirectionState::right:
			if (++m_CurrKillCol == (colEndIdx + 1 - m_CycleCount))
			{
				--m_CurrKillCol;
				m_KillBoxDirectionState = KillBoxDirectionState::down;
			}
			break;
		case Level1::KillBoxDirectionState::down:
			if (++m_CurrKillRow == (rowEndIdx + 1 - m_CycleCount))
			{
				--m_CurrKillRow;
				m_KillBoxDirectionState = KillBoxDirectionState::left;
			}
			break;
		case Level1::KillBoxDirectionState::left:
			if (--m_CurrKillCol == (colBeginIdx - 1 + m_CycleCount))
			{
				++m_CurrKillCol;
				m_KillBoxDirectionState = KillBoxDirectionState::up;
			}
			break;
		case Level1::KillBoxDirectionState::up:
			if (--m_CurrKillRow == (rowBeginIdx + m_CycleCount))
			{
				m_CycleCount++;
				++m_CurrKillRow;
				m_KillBoxDirectionState = KillBoxDirectionState::right;
			}
			break;
		}
	}
}

void Level1::ProcessEndMenuInput(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto input = gameContext.pInput;
	int nrOfButtons{ 3 };
	int nrOfInputId{ 4 };
	int nrOfPlayers = GameData::GetInstance().GetNrOfPlayers();

	for (int playerIdx{ 0 }; playerIdx < nrOfPlayers; ++playerIdx)
	{
		if (input->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::up))
		{
			m_CurrentPauseButtonIndex--;
			if (m_CurrentPauseButtonIndex < 0)
				m_CurrentPauseButtonIndex = nrOfButtons - 1;
			GiveVisualFeedBack();
		}
		if (input->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::down))
		{
			++m_CurrentPauseButtonIndex %= nrOfButtons;
			GiveVisualFeedBack();
		}
		if (input->IsActionTriggered(m_InputIdOffset + playerIdx * nrOfInputId + (int)PauseInputId::select))
		{
			switch (m_CurrentPauseButtonIndex)
			{
			case 0:
				SoundManager::GetInstance()->Stop2DSound(3, 3);
				Reset(gameContext);
				break;
			case 1:
				SceneManager::GetInstance()->SetActiveGameScene(L"StartMenu");
				SoundManager::GetInstance()->Stop2DSound(3, 3);
				break;
			case 2:
				PostQuitMessage(0);
				break;
			}
		}
	}

}

void Level1::SetEndMenuVisuals(bool isVisible)
{
	if (isVisible)
	{
		AddPostProcessingMaterial(0);
		RemovePostProcessingMaterial(1);
	}
	else
	{
		AddPostProcessingMaterial(1);
	}
	m_pPauseMenuBackground->GetComponent<SpriteComponent>()->SetIsVisible(isVisible);
	m_pPlayButton->GetComponent<SpriteComponent>()->SetIsVisible(isVisible);
	m_pMainMenuButton->GetComponent<SpriteComponent>()->SetIsVisible(isVisible);
	m_pExitButton->GetComponent<SpriteComponent>()->SetIsVisible(isVisible);
	for (GameObject *pBomberManHUD : m_BomberManHUD)
	{
		pBomberManHUD->GetComponent<SpriteComponent>()->SetIsVisible(!isVisible);
	}
	m_pTimerHUD->GetComponent<SpriteComponent>()->SetIsVisible(!isVisible);
	m_CurrentPauseButtonIndex = 0;
	GiveVisualFeedBack();
}

void Level1::DrawWinnerInfo(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	const GameSettings &gameSettings = OverlordGame::GetGameSettings();
	float windowWidth{ (float)gameSettings.Window.Width };
	float xOffset{ windowWidth / 2.0f - 200.0f };
	float yOffset{ 20.0f};
	int count{};

	std::wstring gameOverText{ L"Game Over!" };
	std::wstring winnerText{ L"Winners are " };
	for (size_t playerIdx{}; playerIdx < m_WinnerBomberManIdxs.size(); ++playerIdx)
	{
		if (m_WinnerBomberManIdxs[playerIdx] == true)
		{
			++count;
			winnerText.append(L" player " + std::to_wstring(playerIdx + 1));
		}
	}

	TextRenderer::GetInstance()->DrawText(m_pScoreSpriteFont, gameOverText, XMFLOAT2{ xOffset , yOffset }, XMFLOAT4{0.63f, 0.12f, 0.12f, 1.0f});
	if (count != 0)
		TextRenderer::GetInstance()->DrawText(m_pScoreSpriteFont, winnerText, XMFLOAT2{ xOffset , yOffset + 22.0f }, XMFLOAT4{ 0.63f, 0.12f, 0.12f, 1.0f });
}