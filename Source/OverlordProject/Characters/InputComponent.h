#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"

class InputComponent : public BaseComponent
{
public:
	InputComponent(int playerIdx);
	~InputComponent();

	void SetEnableInput(bool isEnabled);
private:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	void InitializeControllerConfiguration0(const GameContext& gameContext);
	void InitializeControllerConfiguration1(const GameContext& gameContext);
	void InitializeControllerConfiguration2(const GameContext& gameContext);
	void InitializeControllerConfiguration3(const GameContext& gameContext);
private:
	enum class InputID
	{
		left = 0,
		right = 1,
		forward = 2,
		backward = 3,
		dropBomb = 4,
		punchBomb = 5,
	};
	int m_PlayerIndex;
	int m_NrOfButtonMappings;
	bool m_IsInputEnabled;
};
