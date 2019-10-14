#pragma once
#include "../Scenegraph/GameObject.h"

struct GameContext;
class CameraComponent;

class FixedCamera : public GameObject
{
public:
	FixedCamera(void);
	virtual ~FixedCamera(void);

protected:

	virtual void Initialize(const GameContext& gameContext);

private:

	CameraComponent *m_pCamera;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	FixedCamera(const FixedCamera& yRef);									
	FixedCamera& operator=(const FixedCamera& yRef);
};

