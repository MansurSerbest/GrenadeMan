//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MainGame.h"
#include "Base\GeneralStructs.h"
#include "Scenegraph\SceneManager.h"
#include "Physx\PhysxProxy.h"
#include "Diagnostics\DebugRenderer.h"
#include "Scenegraph\GameScene.h"
#include "../OverlordProject/Levels/Level1.h"
#include "../OverlordProject/Levels/StartMenu.h"

MainGame::MainGame(void)
{
}


MainGame::~MainGame(void)
{
}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{
	//SceneManager::GetInstance()->AddGameScene((new Level1()));
	SceneManager::GetInstance()->AddGameScene((new StartMenu()));
}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
			else if (wParam == VK_F4)
			{
				DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
			else if (wParam == VK_F6)
			{
				auto activeScene = SceneManager::GetInstance()->GetActiveScene();
				activeScene->GetPhysxProxy()->NextPhysXFrame();
			}
		}
	}

	return -1;
}
