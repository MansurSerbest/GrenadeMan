#pragma once
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include<vector>

class GameObject;
class CameraComponent;
class SpriteFont;
class BomberManCamera;

struct Grid
{
	bool isWall;
	XMFLOAT3 position;
	GameObject *pBox{nullptr};
	GameObject *pExplosion{ nullptr };
	GameObject *pBomb{ nullptr };
	GameObject *pPickUp{ nullptr };
	bool isKillBox{ false };
};

class Level1 final: public GameScene
{
public:
	Level1();
	~Level1();

	enum class LevelState
	{
		play = 0,
		pause = 1,
		killMode = 2,
		end = 3
	};
private:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void SceneActivated() override;

	void InitializeMaterials(const GameContext& gameContext);
	void InitializeGroundPlane(const GameContext& gameContext);
	void InitializeGrids(const GameContext& gameContext);
	void InitializePickUpAssets(const GameContext& gameContext);
	void InitializeBomberMans(const GameContext& gameContext);
	void InitalizePauseInputs(const GameContext& gameContext);
	void InitializeHUD(const GameContext& gameContext);
	void PlayBackGroundMusic();
	void Reset(const GameContext& gameContext);
	void ExitLevel(const GameContext& gameContext);
	void UpdatTimer(const GameContext& gameContext);
	void DrawBomberManScores(const GameContext& gameContext);
	void DrawTimerText(const GameContext& gameContext);
	void CheckBombermansHealth(const GameContext &gameContext);

	//PauseMenu
	void CheckPauseLevel(const GameContext& gameContext);
	void InitializePauseObjects(const GameContext& gameContext);
	void GiveVisualFeedBack();
	void ProcessPauseMenuInput(const GameContext& gameContext);
	void SetPauseMenuVisuals();

	//KillMode
	void DropKillBoxes(const GameContext& gameContext);

	//EndMenu
	void ProcessEndMenuInput(const GameContext& gameContext);
	void SetEndMenuVisuals(bool isVisible);
	void DrawWinnerInfo(const GameContext& gameContext);
private:
	//Play State
	GameObject * m_pExteriorWall;
	int m_GridWidth;
	int m_GridLength;
	float m_GridSize;

	std::vector<std::vector<Grid>> m_Grids;
	GameObject *m_pGroundPlane;
	GameObject *m_pGroundGrid;
	std::vector<GameObject*> m_BomberMans;
	std::vector<bool> m_WinnerBomberManIdxs;

	std::vector<GameObject*> m_BomberManHUD;
	GameObject *m_pTimerHUD;
	SpriteFont *m_pScoreSpriteFont;
	BomberManCamera *m_pBomberManCamera;
	FMOD::Sound *m_pBackgroundMusic;

	LevelState m_CurrLevelState;
	LevelState m_PrevLevelState;;
	float m_LevelTimer;
	float m_LevelTimeLeft;
	float m_EndGameDelayTimer;
	float m_EndGameDelayTimeLeft;
	bool m_IsDelayStart;

	
	//Pause State
	enum class PauseInputId
	{
		pause = 0,
		up = 1,
		down = 2,
		select = 3
	};

	GameObject *m_pPauseMenuBackground;
	GameObject *m_pRestartButton;
	GameObject *m_pMainMenuButton;
	GameObject *m_pExitButton;
	int m_CurrentPauseButtonIndex;
	int m_InputIdOffset;
	bool m_IsInitialized;

	//Kill State
	float m_KillStartTime;
	float m_BoxSpawnTimer;
	float m_BoxSpawnTimerLeft;
	int m_CurrKillRow;
	int m_CurrKillCol;
	int m_CycleCount;
	enum class KillBoxDirectionState
	{
		right = 0,
		down = 1,
		left = 2,
		up = 3
	};
	KillBoxDirectionState m_KillBoxDirectionState;
	//End State
	GameObject *m_pPlayButton;
};
