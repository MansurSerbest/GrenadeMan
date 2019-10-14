#pragma once
#include "BaseComponent.h"

class ControllerComponent : public BaseComponent
{
public:
	ControllerComponent(PxMaterial* material, float radius = 2, float height = 5, wstring name = L"Character", PxCapsuleClimbingMode::Enum climbingMode = PxCapsuleClimbingMode::eEASY);
	virtual ~ControllerComponent();

	void Translate(XMFLOAT3 position);
	void Translate(float x, float y, float z);
	void TranslateWithPivot(XMFLOAT3 position);
	void Move(XMFLOAT3 displacement, float minDist = 0);
	const PxControllerCollisionFlags& GetCollisionFlags() const { return m_CollisionFlag; }
	void SetCollisionGroup(CollisionGroupFlag group);
	void SetCollisionIgnoreGroups(CollisionGroupFlag ignoreGroups);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetPositionWithPivot();
	XMFLOAT3 GetFootPosition();
	void SetPivot(XMFLOAT3 pivot);

	void Resize(float height);
	void SetFootPosition(XMFLOAT3 footPosition);

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_Radius, m_Height;
	XMFLOAT3 m_Pivot;
	wstring m_Name;
	PxController* m_pController;
	PxCapsuleClimbingMode::Enum m_ClimbingMode;
	PxMaterial* m_pMaterial;
	PxControllerCollisionFlags m_CollisionFlag;
	PxFilterData m_CollisionGroups;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ControllerComponent( const ControllerComponent& obj);
	ControllerComponent& operator=( const ControllerComponent& obj);
};

