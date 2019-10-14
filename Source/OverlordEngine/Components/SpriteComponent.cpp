//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SpriteComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Scenegraph/GameObject.h"
#include "../Graphics/TextureData.h"
#include "../Content/ContentManager.h"
#include "../Graphics/SpriteRenderer.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"
#include "../Graphics/SpriteRenderer.h"

SpriteComponent::SpriteComponent(const wstring& spriteAsset, XMFLOAT2 pivot, XMFLOAT4 color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color),
	m_pTexture(nullptr),
	m_IsVisible{true}
{

}


SpriteComponent::~SpriteComponent(void)
{
}

void SpriteComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Update(const GameContext& gameContext)
{

	UNREFERENCED_PARAMETER(gameContext);
	
}

void SpriteComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (!m_pTexture)
		return;

	if (m_IsVisible)
	{
		// Here you need to draw the SpriteComponent using the Draw of the sprite renderer
		// The sprite renderer is a singleton
		// you will need to position, the rotation and the scale
		// You can use the QuaternionToEuler function to help you with the z rotation 
		XMFLOAT2 position = XMFLOAT2(GetGameObject()->GetTransform()->GetPosition().x, GetGameObject()->GetTransform()->GetPosition().y);
		XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		XMFLOAT2 scale = XMFLOAT2(GetGameObject()->GetTransform()->GetScale().x, GetGameObject()->GetTransform()->GetScale().y);
		XMFLOAT3 eulerRotation = QuaternionToEuler(GetGameObject()->GetTransform()->GetRotation());
		float rotation = eulerRotation.z;

		float zPos = GetGameObject()->GetTransform()->GetPosition().z;
		SpriteRenderer::GetInstance()->Draw(m_pTexture, position, m_Color, m_Pivot, scale, rotation, zPos);
		//SpriteRenderer::GetInstance()->Draw(m_pTexture, position, m_Color, m_Pivot, scale, rotation);

	}

}

void SpriteComponent::SetIsVisible(bool isVisible)
{
	m_IsVisible = isVisible;
}