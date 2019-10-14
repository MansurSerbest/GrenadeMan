#include "../OverlordEngine/Base/stdafx.h"
#include "../OverlordEngine/Scenegraph/GameScene.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"
#include "BomberManCamera.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "../OverlordEngine/Components/CameraComponent.h"


BomberManCamera::BomberManCamera(void) :
	m_pCamera(nullptr)
{
}


BomberManCamera::~BomberManCamera(void)
{
}

void BomberManCamera::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Camera Component
	m_pCamera = new CameraComponent();
	AddComponent(m_pCamera);

	GetTransform()->Translate(0, 0, -10);
}

void BomberManCamera::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetScene()->SetActiveCamera(m_pCamera);
	GetTransform()->Rotate(XMFLOAT3{ 70.0f, 0.0f, 0.0f });
	GetTransform()->Translate(0.0f, 14.0f, -5.25f);
}