//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "FixedCamera.h"
#include "../Components/TransformComponent.h"

FixedCamera::FixedCamera(void):
	m_pCamera(nullptr)
{
}


FixedCamera::~FixedCamera(void)
{
}

void FixedCamera::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Camera Component
	m_pCamera = new CameraComponent();
	AddComponent(m_pCamera);

	GetTransform()->Translate(0,0,-10);
}