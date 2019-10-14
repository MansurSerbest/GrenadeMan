#include "../../OverlordEngine/Base/stdafx.h"
#include "StartMenu.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "../OverlordEngine/Components/SpriteComponent.h"
#include "../OverlordEngine/Base/SoundManager.h"
#include "../OverlordEngine/Base/OverlordGame.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "MenuController.h"
#include "../Levels/Level1.h"
#include "../OverlordEngine/Scenegraph/SceneManager.h"
#include "GameData.h"

StartMenu::StartMenu()
	:GameScene(L"StartMenu"), m_pBackGround{nullptr}, m_pPlayButton{ nullptr }, m_p1PlayerButton{ nullptr },
	m_p2PlayerButton{ nullptr }, m_p3PlayerButton{ nullptr }, m_p4PlayerButton{ nullptr }, m_pExitButton{ nullptr }, m_pMainMenuButton{nullptr}
	, m_MainCurrentButtonIdx{ 0 }, m_MainNrOfButtons{ 2 }, m_IsInitialized{ false }, m_State{}, m_PlayerCurrentButtonIdx{0}, m_IsPlayLevelInitialized{false}
	,m_PlayerNrOfButtons{5}, m_pControlSheet{nullptr}
{

}
StartMenu::~StartMenu()
{
	
}


void StartMenu::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	InitializeBackGround();
	InitializeButtons();
	InitializeSounds();
	InitializeConrollers();
	m_IsInitialized = true;
}
void StartMenu::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void StartMenu::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void StartMenu::SceneActivated()
{
	if (m_IsInitialized)
		Reset();
}

void StartMenu::InitializeBackGround()
{
	m_pBackGround = new GameObject();
	m_pBackGround->GetTransform()->Translate(0.0f, 0.0f, 0.8f);
	SpriteComponent *pSpriteComponent = new SpriteComponent(L"Resources/Textures/HUD/MenuBackGround.png");
	m_pBackGround->AddComponent(pSpriteComponent);
	PendToAddChild(m_pBackGround);

}
void StartMenu::InitializeButtons()
{
	const GameSettings &gameSettings = OverlordGame::GetGameSettings();
	float windowWidth{(float)gameSettings.Window.Width};
	float windowHeight{ (float)gameSettings.Window.Height };
	float xOffset{ windowWidth / 2.0f };
	float yOffset{ windowHeight / 3.0f};
	float buttonsSpaceBetween{ 200.0f };
	float buttonSpaceBetweenPlayer{ 100.0f };

	m_pPlayButton = new GameObject();
	m_pPlayButton->GetTransform()->Translate(xOffset, yOffset, 0.7f);
	m_pPlayButton->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/PlayButton.png", XMFLOAT2{ 0.5f, 0.25f }));
	PendToAddChild(m_pPlayButton);

	m_pExitButton = new GameObject();
	m_pExitButton->GetTransform()->Translate(xOffset, yOffset + buttonsSpaceBetween, 0.6f);
	m_pExitButton->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/ExitButton.png", XMFLOAT2{ 0.5f, 0.25f }));
	PendToAddChild(m_pExitButton);

	float PlayerButtonScale{ 0.75f };

	m_p1PlayerButton = new GameObject();
	m_p1PlayerButton->GetTransform()->Translate(xOffset, yOffset - buttonSpaceBetweenPlayer, 0.7f);
	m_p1PlayerButton->GetTransform()->Scale(PlayerButtonScale, PlayerButtonScale, 1.0f);
	m_p1PlayerButton->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/1PlayerButton.png", XMFLOAT2{ 0.5f, 0.25f }));
	m_p1PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	PendToAddChild(m_p1PlayerButton);

	m_p2PlayerButton = new GameObject();
	m_p2PlayerButton->GetTransform()->Translate(xOffset, yOffset, 0.6f);
	m_p2PlayerButton->GetTransform()->Scale(PlayerButtonScale, PlayerButtonScale, 1.0f);
	m_p2PlayerButton->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/2PlayerButton.png", XMFLOAT2{ 0.5f, 0.25f }));
	m_p2PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	PendToAddChild(m_p2PlayerButton);

	m_p3PlayerButton = new GameObject();
	m_p3PlayerButton->GetTransform()->Translate(xOffset, yOffset + buttonSpaceBetweenPlayer, 0.5f);
	m_p3PlayerButton->GetTransform()->Scale(PlayerButtonScale, PlayerButtonScale, 1.0f);
	m_p3PlayerButton->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/3PlayerButton.png", XMFLOAT2{ 0.5f, 0.25f }));
	m_p3PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	PendToAddChild(m_p3PlayerButton);

	m_p4PlayerButton = new GameObject();
	m_p4PlayerButton->GetTransform()->Translate(xOffset, yOffset + 2 * buttonSpaceBetweenPlayer, 0.4f);
	m_p4PlayerButton->GetTransform()->Scale(PlayerButtonScale, PlayerButtonScale, 1.0f);
	m_p4PlayerButton->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/4PlayerButton.png", XMFLOAT2{ 0.5f, 0.25f }));
	m_p4PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	PendToAddChild(m_p4PlayerButton);

	m_pMainMenuButton = new GameObject();
	m_pMainMenuButton->GetTransform()->Translate(xOffset, yOffset + 3 * buttonSpaceBetweenPlayer, 0.3f);
	m_pMainMenuButton->GetTransform()->Scale(PlayerButtonScale, PlayerButtonScale, 1.0f);
	m_pMainMenuButton->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/MainMenuButton.png", XMFLOAT2{ 0.5f, 0.25f }));
	m_pMainMenuButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	PendToAddChild(m_pMainMenuButton);

	m_pControlSheet = new GameObject();
	m_pControlSheet->AddComponent(new SpriteComponent(L"Resources/Textures/HUD/ControlSheet.png"));
	m_pControlSheet->GetComponent<SpriteComponent>()->SetIsVisible(false);
	PendToAddChild(m_pControlSheet);

	GiveVisualFeedBack();
}
void StartMenu::InitializeSounds()
{
	SoundManager *pSoundManager = SoundManager::GetInstance();
	pSoundManager->Play2DSound(4, 4, -1, false, "Resources/Sound/MainMenuMusic.mp3");
	pSoundManager->SetChannelVolume(4, 0.2f);
}
void StartMenu::InitializeConrollers()
{
	for (unsigned int playerIdx{ 0 }; playerIdx < 4; ++playerIdx)
	{
		GameObject *pController = new GameObject();
		pController->AddComponent(new MenuController(playerIdx));
		m_Controllers.push_back(pController);
		PendToAddChild(pController);
	}
}
void StartMenu::MoveUp()
{
	switch (m_State)
	{
	case StartMenu::MenuState::main:
		m_MainCurrentButtonIdx--;
		if (m_MainCurrentButtonIdx < 0)
		{
			m_MainCurrentButtonIdx = m_MainNrOfButtons - 1;
		}
		GiveVisualFeedBack();
		break;
	case StartMenu::MenuState::nrOfPlayer:
		m_PlayerCurrentButtonIdx--;
		if (m_PlayerCurrentButtonIdx < 0)
		{
			m_PlayerCurrentButtonIdx = m_PlayerNrOfButtons - 1;
		}
		GiveVisualFeedBack();
		break;
	}

}
void StartMenu::MoveDown()
{
	switch (m_State)
	{
	case StartMenu::MenuState::main:
		++m_MainCurrentButtonIdx %= m_MainNrOfButtons;
		GiveVisualFeedBack();
		break;
	case StartMenu::MenuState::nrOfPlayer:
		++m_PlayerCurrentButtonIdx %= m_PlayerNrOfButtons;
		GiveVisualFeedBack();
		break;
	}
}
void StartMenu::Select(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	switch (m_State)
	{
	case StartMenu::MenuState::main:
		if (m_MainCurrentButtonIdx == 0)
		{
			TransitionToPlayerMenu();
		}
		if (m_MainCurrentButtonIdx == 1)
		{
			PostQuitMessage(0);
		}
		break;
	case StartMenu::MenuState::nrOfPlayer:
		{
			GameData &gameData = GameData::GetInstance();
			if (m_PlayerCurrentButtonIdx != 4)
			{
				switch (m_PlayerCurrentButtonIdx)
				{
				case 0:
					gameData.SetNrOfPlayers(1);
					break;
				case 1:
					gameData.SetNrOfPlayers(2);
					break;
				case 2:
					gameData.SetNrOfPlayers(3);
					break;
				case 3:
					gameData.SetNrOfPlayers(4);
					break;
				}

				m_State = StartMenu::MenuState::controlSheet;
				m_pControlSheet->GetComponent<SpriteComponent>()->SetIsVisible(true);
				//SceneManager *pSceneManager = SceneManager::GetInstance();
				//if (m_IsPlayLevelInitialized)
				//{
				//	pSceneManager->SetActiveGameScene(L"Level1");
				//	ExitLevel();
				//}
				//else
				//{
				//	m_IsPlayLevelInitialized = true;
				//	GameScene *pPlayScene = new Level1();
				//	pSceneManager->AddGameScene(pPlayScene);
				//	pSceneManager->SetActiveGameScene(L"Level1");
				//	ExitLevel();
				//}
			}
			else
			{
				TransitionToMainMenu();
			}
			break;
		}
	case StartMenu::MenuState::controlSheet:
		{
		SceneManager *pSceneManager = SceneManager::GetInstance();
		if (m_IsPlayLevelInitialized)
		{
			pSceneManager->SetActiveGameScene(L"Level1");
			ExitLevel();
		}
		else
		{
			m_IsPlayLevelInitialized = true;
			GameScene *pPlayScene = new Level1();
			pSceneManager->AddGameScene(pPlayScene);
			pSceneManager->SetActiveGameScene(L"Level1");
			ExitLevel();
		}
		}
	}
}
void StartMenu::GiveVisualFeedBack()
{

	switch (m_State)
	{
		case StartMenu::MenuState::main:
		{
			switch (m_MainCurrentButtonIdx)
			{
				case 0:
				{
					m_pPlayButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/PlayButtonFeedBack.png");
					m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButton.png");
					break;
				}
				case 1:
				{
					m_pPlayButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/PlayButton.png");
					m_pExitButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/ExitButtonFeedBack.png");
					break;
				}
			}
		}
		break;
		case StartMenu::MenuState::nrOfPlayer:
		{
			m_p1PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/1PlayerButton.png");
			m_p2PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/2PlayerButton.png");
			m_p3PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/3PlayerButton.png");
			m_p4PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/4PlayerButton.png");
			m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButton.png");
			switch (m_PlayerCurrentButtonIdx)
			{
			case 0:
				m_p1PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/1PlayerButtonFeedBack.png");
				break;
			case 1:
				m_p2PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/2PlayerButtonFeedBack.png");
				break;
			case 2:
				m_p3PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/3PlayerButtonFeedBack.png");
				break;
			case 3:
				m_p4PlayerButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/4PlayerButtonFeedBack.png");
				break;
			case 4:
				m_pMainMenuButton->GetComponent<SpriteComponent>()->SetTexture(L"Resources/Textures/HUD/MainMenuButtonFeedBack.png");
				break;
			}
		}
		break;
	}
}

void StartMenu::TransitionToPlayerMenu()
{
	m_State = MenuState::nrOfPlayer;
	m_PlayerCurrentButtonIdx = 0;
	m_pPlayButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_pExitButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_p1PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_p2PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_p3PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_p4PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_pMainMenuButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	GiveVisualFeedBack();
}

void StartMenu::TransitionToMainMenu()
{
	m_State = MenuState::main;
	m_MainCurrentButtonIdx = 0;
	m_pPlayButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_pExitButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_p1PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_p2PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_p3PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_p4PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_pMainMenuButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	GiveVisualFeedBack();
}

void StartMenu::Reset()
{
	m_State = MenuState::main;
	m_MainCurrentButtonIdx = 0;
	m_pPlayButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_pExitButton->GetComponent<SpriteComponent>()->SetIsVisible(true);
	m_p1PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_p2PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_p3PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_p4PlayerButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_pMainMenuButton->GetComponent<SpriteComponent>()->SetIsVisible(false);
	m_pControlSheet->GetComponent<SpriteComponent>()->SetIsVisible(false);
	GiveVisualFeedBack();
	InitializeSounds();
}

void StartMenu::ExitLevel()
{
	SoundManager::GetInstance()->Stop2DSound(4, 4);
}