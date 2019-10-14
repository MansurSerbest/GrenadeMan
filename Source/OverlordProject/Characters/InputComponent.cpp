#include "../../OverlordEngine/Base/stdafx.h"
#include "InputComponent.h"
#include "../OverlordEngine/Base/InputManager.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "MovementComponent.h"
#include "BombManagerComponent.h"
#include "BombComponent.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "InteractionComponent.h"

InputComponent::InputComponent(int playerIdx)
	:m_PlayerIndex{playerIdx}, m_NrOfButtonMappings{6}, m_IsInputEnabled{true}
{

}
InputComponent::~InputComponent()
{

}

void InputComponent::SetEnableInput(bool isEnabled)
{
	m_IsInputEnabled = isEnabled;
}

void InputComponent::Initialize(const GameContext& gameContext)
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
void InputComponent::Update(const GameContext& gameContext)
{
	if (m_IsInputEnabled)
	{
		InputManager *input = gameContext.pInput;
		MovementComponent *movComp = m_pGameObject->GetComponent<MovementComponent>();
		bool isTriggered{ false };
		int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };

		if (movComp)
		{
			if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::left))
			{
				movComp->MoveLeft();
				isTriggered = true;
			}
			if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::right))
			{
				movComp->MoveRight();
				isTriggered = true;
			}
			if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::forward))
			{
				movComp->MoveForward();
				isTriggered = true;
			}
			if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::backward))
			{
				movComp->MoveBackward();
				isTriggered = true;
			}
			if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::punchBomb))
			{
				m_pGameObject->GetComponent<InteractionComponent>()->PunchBomb(gameContext);
				movComp->MovePunch();
			}
			if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::punchBomb))
			{
				m_pGameObject->GetComponent<InteractionComponent>()->PunchBomb(gameContext);
				movComp->MovePunch();
			}
			if (!isTriggered)
			{
				movComp->MoveIdle();
			}
		}

		if (input->IsActionTriggered(inputActionIdOffset + (int)InputID::dropBomb))
		{
			if (m_pGameObject)
				m_pGameObject->GetComponent<BombManagerComponent>()->DropBomb(gameContext);
		}
	}
}
void InputComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void InputComponent::InitializeControllerConfiguration0(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::left, InputTriggerState::Down, VK_NUMPAD4, -1, XINPUT_GAMEPAD_DPAD_LEFT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::right, InputTriggerState::Down, VK_NUMPAD6, -1, XINPUT_GAMEPAD_DPAD_RIGHT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::forward, InputTriggerState::Down, VK_NUMPAD8, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::backward, InputTriggerState::Down, VK_NUMPAD5, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::dropBomb, InputTriggerState::Pressed, VK_NUMPAD7, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::punchBomb, InputTriggerState::Pressed, VK_NUMPAD9, -1, XINPUT_GAMEPAD_X, GamepadIndex(m_PlayerIndex) });
}
void InputComponent::InitializeControllerConfiguration1(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::left, InputTriggerState::Down, 0x41, -1, XINPUT_GAMEPAD_DPAD_LEFT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::right, InputTriggerState::Down, 0x44, -1, XINPUT_GAMEPAD_DPAD_RIGHT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::forward, InputTriggerState::Down, 0x57, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::backward, InputTriggerState::Down, 0x53, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::dropBomb, InputTriggerState::Pressed, 0x51, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::punchBomb, InputTriggerState::Pressed, 0x45, -1, XINPUT_GAMEPAD_X, GamepadIndex(m_PlayerIndex) });
}
void InputComponent::InitializeControllerConfiguration2(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::left, InputTriggerState::Down, 0x47, -1, XINPUT_GAMEPAD_DPAD_LEFT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::right, InputTriggerState::Down, 0x4A, -1, XINPUT_GAMEPAD_DPAD_RIGHT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::forward, InputTriggerState::Down, 0x59, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::backward, InputTriggerState::Down, 0x48, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::dropBomb, InputTriggerState::Pressed, 0x54, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::punchBomb, InputTriggerState::Pressed, 0x55, -1, XINPUT_GAMEPAD_X, GamepadIndex(m_PlayerIndex) });
}
void InputComponent::InitializeControllerConfiguration3(const GameContext& gameContext)
{
	int inputActionIdOffset{ m_NrOfButtonMappings * m_PlayerIndex };
	InputManager *input = gameContext.pInput;
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::left, InputTriggerState::Down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::right, InputTriggerState::Down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::forward, InputTriggerState::Down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::backward, InputTriggerState::Down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::dropBomb, InputTriggerState::Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, GamepadIndex(m_PlayerIndex) });
	input->AddInputAction(InputAction{ inputActionIdOffset + (int)InputID::punchBomb, InputTriggerState::Pressed, VK_CONTROL, -1, XINPUT_GAMEPAD_X, GamepadIndex(m_PlayerIndex) });
}