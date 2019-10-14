//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Particle.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"


Particle::Particle(const ParticleEmitterSettings& emitterSettings) :
	m_EmitterSettings(emitterSettings)
{
}


Particle::~Particle(void)
{
}

void Particle::Update(const GameContext& gameContext)
{
	//See Lab10_2

	//If not Active return immediately
	if (!m_IsActive)
	{
		return;
	}

	//if is active:
	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if (m_CurrentEnergy < 0)
	{
		m_IsActive = false;
		return;
	}

	//Velocity
	float elapsedTime{ gameContext.pGameTime->GetElapsed() };
	XMFLOAT3 velocity{ m_EmitterSettings.Velocity };
	m_VertexInfo.Position.x += velocity.x * elapsedTime;
	m_VertexInfo.Position.y += velocity.y * elapsedTime;
	m_VertexInfo.Position.z += velocity.z * elapsedTime;

	//Color
	m_VertexInfo.Color = m_EmitterSettings.Color;
	float particleLifePercent{ m_CurrentEnergy / m_TotalEnergy };
	m_VertexInfo.Color.w = particleLifePercent / 2;

	//Vertex Size
	if (m_SizeGrow < 1)
		m_VertexInfo.Size = m_InitSize - m_InitSize * (m_SizeGrow * (1 - particleLifePercent));
	else if (m_SizeGrow > 1)
		m_VertexInfo.Size = m_InitSize + m_InitSize * (m_SizeGrow * (1 - particleLifePercent));

	
}

void Particle::Init(XMFLOAT3 initPosition)
{
	UNREFERENCED_PARAMETER(initPosition);

	//See Lab10_2
	m_IsActive = true;

	//Energy Initialization
	m_TotalEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_CurrentEnergy = m_TotalEnergy;
	//Position Initialization
	XMVECTOR randomDirection{ 1.0f, 0.0f, 0.0f };
	XMMATRIX randomRotationMatrix = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	randomDirection = XMVector3TransformNormal(randomDirection, randomRotationMatrix);
	float randDistance = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
	//XMVECTOR initPos = XMLoadFloat3(&initPosition);
	//XMStoreFloat3(&m_VertexInfo.Position, initPos + (randomDirection * randDistance));
	XMStoreFloat3(&m_VertexInfo.Position, (randomDirection * randDistance));
	//Size Initialization
	m_VertexInfo.Size = m_InitSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);
	//Rotation
	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);

}
