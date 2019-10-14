#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"

class MenuController : public BaseComponent
{
public:
	MenuController(int playerIdx);
	~MenuController();

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
		up = 0,
		down = 1,
		select = 2
	};
	int m_PlayerIndex;
	int m_NrOfButtonMappings;
};
