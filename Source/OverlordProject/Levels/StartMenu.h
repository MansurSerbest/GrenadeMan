#pragma once
#include "../OverlordEngine/Scenegraph/GameScene.h"

class GameObject;

class StartMenu : public GameScene
{
public:
	StartMenu();
	~StartMenu();

	void MoveUp();
	void MoveDown();
	void Select(const GameContext& gameContext);
	void Reset();
private:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void SceneActivated() override;

	void InitializeBackGround();
	void InitializeButtons();
	void InitializeSounds();
	void InitializeConrollers();
	void ExitLevel();
	void GiveVisualFeedBack();

	void TransitionToPlayerMenu();
	void TransitionToMainMenu();
private:
	enum class MenuState
	{
		main = 0,
		nrOfPlayer = 1,
		controlSheet = 2
	};
	MenuState m_State;
	std::vector<GameObject*> m_Controllers;
	GameObject *m_pBackGround;
	GameObject *m_pPlayButton;
	GameObject *m_pExitButton;
	GameObject *m_p1PlayerButton;
	GameObject *m_p2PlayerButton;
	GameObject *m_p3PlayerButton;
	GameObject *m_p4PlayerButton;
	GameObject *m_pMainMenuButton;
	GameObject *m_pControlSheet;

	FMOD::Sound *m_pBackgroundMusic;
	int m_MainCurrentButtonIdx;
	int m_MainNrOfButtons;
	int m_PlayerCurrentButtonIdx;
	int m_PlayerNrOfButtons;
	bool m_IsInitialized;
	bool m_IsPlayLevelInitialized;
};
