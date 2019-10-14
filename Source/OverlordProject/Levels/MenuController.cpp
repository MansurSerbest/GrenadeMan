#include "../../OverlordEngine/Base/stdafx.h"
#include "MenuController.h"
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "StartMenu.h"

MenuController::MenuController(int playerIdx)
	: m_PlayerIndex{playerIdx}, m_NrOfButtonMappings{3}
{

}
MenuController::~MenuController()
{

}
void MenuController::Initialize(const GameContext& gameContext)
{
	switch (m_PlayerIndex)
	{
	case 0:
		InitializeControllerConfiguration0(gameContext);
		break;
	case 1:
		InitializeControllerConfiguration1(gameContext);
		break;
	case 2:
		InitializeControllerConfiguration2(gameContext);
		break;
	case 3:
		InitializeControllerConfiguration3(gameContext);
		break;
	}
}
void MenuController::Update(const GameContext& gameContext)
{
	InputManager *input = gameContext.pInput;
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	StartMenu *pScene = dynamic_cast<StartMenu*>(GetGameObject()->GetScene());

	if (pScene)
	{
		if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::up))
		{
			pScene->MoveUp();
		}
		if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::down))
		{
			pScene->MoveDown();
		}
		if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::select))
		{
			pScene->Select(gameContext);
		}
	}
}
void MenuController::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void MenuController::InitializeControllerConfiguration0(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::up, InputTriggerState::Pressed, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::down, InputTriggerState::Pressed, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::select, InputTriggerState::Pressed, VK_RETURN, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
}
void MenuController::InitializeControllerConfiguration1(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::up, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::down, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::select, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
}
void MenuController::InitializeControllerConfiguration2(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::up, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::down, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::select, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
}
void MenuController::InitializeControllerConfiguration3(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::up, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::down, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::select, InputTriggerState::Pressed, 0, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
}

