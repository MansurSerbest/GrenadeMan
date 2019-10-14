#pragma once
#include "Graphics/PostProcessingMaterial.h"

class PostTimer final: public PostProcessingMaterial
{
public:
	PostTimer();
	~PostTimer();
	void SetIntensity(const float intensity);
	void SetRange(const float range);

protected:
	void LoadEffectVariables();
	void UpdateEffectVariables(RenderTarget* rendertarget);


	ID3DX11EffectShaderResourceVariable *m_pTextureMapVariabele;
	ID3DX11EffectScalarVariable *m_pIntensityVariable;
	float m_Intensity;
	ID3DX11EffectScalarVariable *m_pRangeVariable;
	float m_Range;
	float m_CurrRange;
	float m_PulseSign;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostTimer(const PostTimer &obj);
	PostTimer& operator=(const PostTimer& obj);
};