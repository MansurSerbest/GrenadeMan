#include "../../OverlordEngine/Base/stdafx.h"
#include "PickUpComponent.h"
#include "../OverlordEngine/Components/TransformComponent.h"
#include "../OverlordEngine/Scenegraph/GameObject.h"

PickUpComponent::PickUpComponent(int stateNr, XMFLOAT3 pos)
	:m_State{PickUpComponent::PickUpState(stateNr)}, m_Position{ pos }, m_HoverValue{}
{

}
PickUpComponent::~PickUpComponent()
{

}

void PickUpComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void PickUpComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	Hover(gameContext);
}
void PickUpComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
PickUpComponent::PickUpState PickUpComponent::GetState() const
{
	return m_State;
}
void PickUpComponent::Hover(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_HoverValue += 0.01f;
	float heightScale{ 0.25f };
	float hoverHeight = cos(m_HoverValue) * heightScale;
	m_pGameObject->GetTransform()->Translate(m_Position.x, m_Position.y + hoverHeight, m_Position.z);
}

