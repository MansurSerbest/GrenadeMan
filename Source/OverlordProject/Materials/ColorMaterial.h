#pragma once
#include "Graphics\Material.h"

class ColorMaterial: public Material
{
public:
	ColorMaterial(bool enableTransparency = false);
	~ColorMaterial();

	void SetLightDirection(XMFLOAT3 dir);
	void SetColor(XMFLOAT4 color);

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:

	XMFLOAT3 m_LightDirection = { -0.577f, -0.577f, 0.577f };
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	static ID3DX11EffectVectorVariable* m_pColorVariable;
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ColorMaterial(const ColorMaterial &obj);
	ColorMaterial& operator=(const ColorMaterial& obj);
};

