#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"

class PickUpComponent final : public BaseComponent
{
public:
	enum class PickUpState
	{
		speed = 0,
		bombCapacity = 1,
		bombPower = 2
	};

	PickUpComponent(int stateNr, XMFLOAT3 pos);
	~PickUpComponent();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

	PickUpState GetState() const;
private:
	void Hover(const GameContext& gameContext);
private:

	PickUpState m_State;
	XMFLOAT3 m_Position;
	float m_HoverValue;
};
