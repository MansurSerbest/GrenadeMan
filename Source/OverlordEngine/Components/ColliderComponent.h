#pragma once
#include "BaseComponent.h"

class ColliderComponent : public BaseComponent
{
public:
	ColliderComponent(std::shared_ptr<PxGeometry>& geometry, const PxMaterial& material, const PxTransform& localPose = PxTransform::createIdentity());
	~ColliderComponent(void);

	PxShape* GetShape() {return m_pShape; };
	bool IsTrigger(){return m_isTrigger;};
	void EnableTrigger(bool isTrigger);

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	void SetShape(PxShape* shape);

	std::shared_ptr<PxGeometry> m_Geometry;
	const PxMaterial& m_Material;
	const PxTransform m_LocalPose;

	PxShape* m_pShape;
	bool m_isTrigger;

	friend class RigidBodyComponent;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ColliderComponent( const ColliderComponent& obj);
	ColliderComponent& operator=( const ColliderComponent& obj);
};

