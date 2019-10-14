#pragma once
#include "../OverlordEngine/Components/BaseComponent.h"
#include "..\Levels\Level1.h"

class ControllerComponent;

class MovementComponent : public BaseComponent
{
public:
	enum class MovementState
	{
		idle = 0,
		left = 1,
		right = 2,
		forward = 3,
		backward = 4,
		punch = 5, 
		dead = 6
	};

	enum class DirectionState
	{
		left = 0,
		right = 1,
		forward = 2,
		backward = 3
	};
public:
	MovementComponent(std::vector<std::vector<Grid>>& grids);
	~MovementComponent();
	void SetMovementState(MovementState state);
	bool IsCollidingWithBomb(float deltaTime);
	DirectionState GetDirectionState() const;
	void IncreaseSpeed(float toAddSpeed);

	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void MoveIdle();
	void MovePunch();
	void MoveDead();

	void PlayPunchSound();
private:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
private:
	MovementState m_PrevState;
	MovementState m_CurrentState;
	DirectionState m_DirectionState;
	float m_Speed;
	std::vector<std::vector<Grid>>& m_Grids;

	float m_PunchAnimationTimer;
	float m_PunchAnimationTimeLeft;

	float m_DeadAnimationTimer;
	float m_DeadAnimationTimeLeft;
};
