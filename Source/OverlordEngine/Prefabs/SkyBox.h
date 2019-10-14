#pragma once
#include "..\Scenegraph\GameObject.h"
#include "..\Components\ModelComponent.h"

class SkyBox : public GameObject
{
public:
	SkyBox(std::wstring assetFile);
	~SkyBox(void);

protected:

	virtual void Initialize(const GameContext& gameContext);

private:
	wstring m_assetFile;
	ModelComponent * m_pModelComponent;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBox(const SkyBox& yRef);
	SkyBox& operator=(const SkyBox& yRef);
};