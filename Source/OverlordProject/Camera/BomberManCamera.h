#pragma once
#include "../OverlordEngine/Scenegraph/GameObject.h"

struct GameContext;
class CameraComponent;

class BomberManCamera : public GameObject
{
public:
	BomberManCamera(void);
	virtual ~BomberManCamera(void);

protected:
	void Initialize(const GameContext& gameContext);
	void PostInitialize(const GameContext& gameContext);

private:

	CameraComponent * m_pCamera;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BomberManCamera(const BomberManCamera& yRef);
	BomberManCamera& operator=(const BomberManCamera& yRef);
};
