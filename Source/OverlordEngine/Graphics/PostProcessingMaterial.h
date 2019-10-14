#pragma once
#include "../Helpers/EffectHelper.h"

class RenderTarget;
struct GameContext;

class PostProcessingMaterial
{
public:
	PostProcessingMaterial(wstring effectFile, const wstring& technique = L"");
	virtual ~PostProcessingMaterial();

	void Initialize(const GameContext& gameContext);
	static void Destroy();

	RenderTarget* GetRenderTarget() const;
	virtual void Draw(const GameContext& gameContext, RenderTarget* previousRendertarget);

protected:
	virtual bool LoadEffect(const GameContext& gameContext, const wstring& effectFile);
	virtual void LoadEffectVariables() = 0;
	virtual void UpdateEffectVariables(RenderTarget* rendertarget) = 0;

	bool m_IsInitialized;
	
	wstring m_effectFile;
	wstring m_TechniqueName;
	ID3DX11Effect *m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	
	RenderTarget* m_pRenderTarget;

	//Static, shared between every PostProcessingMaterial
	static void CreateVertexBuffer(const GameContext& gameContext);
	static void CreateInputLayout(const GameContext& gameContext, ID3DX11EffectTechnique* pTechnique);

	static ID3D11Buffer* m_pVertexBuffer;
	static const int NUM_VERTS = 4;
	static unsigned int m_VertexBufferStride;

	static ID3D11InputLayout *m_pInputLayout;
	static vector<ILDescription> m_pInputLayoutDescriptions;
	static UINT m_pInputLayoutSize;
	static UINT m_InputLayoutID;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostProcessingMaterial(const PostProcessingMaterial& yRef);									
	PostProcessingMaterial& operator=(const PostProcessingMaterial& yRef);
};

