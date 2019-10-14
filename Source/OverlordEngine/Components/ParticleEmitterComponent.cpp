//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ParticleEmitterComponent.h"
#include "../Helpers/EffectHelper.h"
#include "../Content/ContentManager.h"
#include "../Content/TextureDataLoader.h"
#include "../Graphics/Particle.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"


ParticleEmitterComponent::ParticleEmitterComponent(const wstring& assetFile, int particleCount) :
	m_ParticleCount(particleCount),
	m_AssetFile(assetFile),
	m_IsActive{true}
{
	//See Lab10_2
	for (int idx{ 0 }; idx < m_ParticleCount; ++idx)
	{
		m_Particles.push_back(new Particle(m_Settings));
	}
}


ParticleEmitterComponent::~ParticleEmitterComponent(void)
{
	//See Lab10_2
	for (Particle* particle : m_Particles)
	{
		delete particle;
	}
	m_Particles.clear();
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);

}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	//See Lab10_2
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
	if (!m_pParticleTexture)
	{
		Logger::LogError(L"m_pParticleTexture could not be initialized!");
	}
}

void ParticleEmitterComponent::SetTexture(const wstring& assetFile)
{
	m_pParticleTexture = ContentManager::Load<TextureData>(assetFile);
	if (!m_pParticleTexture)
	{
		Logger::LogError(L"m_pParticleTexture could not be initialized!");
	}
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	//See Lab10_2
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ParticleRenderer.fx");
	m_pDefaultTechnique = m_pEffect->GetTechniqueByName("Default");

	if (!m_pWvpVariable)
		m_pWvpVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pWvpVariable)
		Logger::LogError(L"m_pWvpVariable could not be initialized!");

	if (!m_pViewInverseVariable)
		m_pViewInverseVariable = m_pEffect->GetVariableBySemantic("ViewInverse")->AsMatrix();
	if (!m_pViewInverseVariable)
		Logger::LogError(L"m_pViewInverseVariable could not be initialized!");

	if (!m_pTextureVariable)
		m_pTextureVariable = m_pEffect->GetVariableByName("gParticleTexture")->AsShaderResource();
	if (!m_pTextureVariable)
		Logger::LogError(L"m_pTextureVariable could not be initialized!");

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	//See Lab10_2
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(ParticleVertex) * m_ParticleCount;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = gameContext.pDevice->CreateBuffer(&desc, nullptr, &m_pVertexBuffer);
	if (Logger::LogHResult(hr, L"ParticleEmitterComponent::Update")) return;
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	//See Lab10_2
	//m_ParticleCount
	float particleInterval{((m_Settings.MaxEnergy + m_Settings.MinEnergy) / 2.0f) / m_ParticleCount };

	m_LastParticleInit += gameContext.pGameTime->GetElapsed();

	m_ActiveParticles = 0;


	//BUFFER MAPPING CODE [PARTIAL :)]
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ParticleVertex* pBuffer = (ParticleVertex*) mappedResource.pData;
	for (Particle *particle : m_Particles)
	{
		particle->Update(gameContext);
		if (particle->IsActive())
		{
			pBuffer[m_ActiveParticles] = particle->GetVertexInfo();
			++m_ActiveParticles;
		}
		else
		{
			if (m_LastParticleInit >= particleInterval && m_IsActive)
			{
				particle->Init(GetTransform()->GetWorldPosition());
				pBuffer[m_ActiveParticles] = particle->GetVertexInfo();
				++m_ActiveParticles;
				m_LastParticleInit = 0;
			}
		}
	}
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	//See Lab10_2
	XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMMATRIX view = XMLoadFloat4x4(&gameContext.pCamera->GetView());
	XMMATRIX projection = XMLoadFloat4x4(&gameContext.pCamera->GetProjection());
	XMMATRIX wvp = world * view * projection;
	XMFLOAT4X4 finalWVP;
	XMStoreFloat4x4(&finalWVP, wvp);

	m_pWvpVariable->SetMatrix((float*)&finalWVP);
	m_pViewInverseVariable->SetMatrix((float*)(&gameContext.pCamera->GetViewInverse()));
	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (UINT j = 0; j < techDesc.Passes; ++j)
	{
		m_pDefaultTechnique->GetPassByIndex(j)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}
