#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11XEffectShaderResourceVariable;

class PostBlur : public PostProcessingMaterial
{
public:
	PostBlur();
	~PostBlur();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;


private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostBlur(const PostBlur &obj);
	PostBlur& operator=(const PostBlur& obj);
};
