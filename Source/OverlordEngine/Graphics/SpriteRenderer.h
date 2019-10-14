#pragma once
#include "../Helpers/Singleton.h"

class TextureData;
class OverlordGame;
class GameScene;

class SpriteRenderer: public Singleton<SpriteRenderer>
{
public:

	void Draw(TextureData* pTexture, XMFLOAT2 position, XMFLOAT4 color = static_cast<XMFLOAT4>(Colors::White), XMFLOAT2 pivot = XMFLOAT2(0,0), XMFLOAT2 scale = XMFLOAT2(1,1), float rotation = 0.f, float depth = 0.f);
	void DrawImmediate(const GameContext& gameContext, ID3D11ShaderResourceView* pSrv, XMFLOAT2 position, XMFLOAT4 color = static_cast<XMFLOAT4>(Colors::White), XMFLOAT2 pivot = XMFLOAT2(0, 0), XMFLOAT2 scale = XMFLOAT2(1, 1), float rotation = 0.f);

private:

	struct SpriteVertex
	{
		UINT TextureId;
		XMFLOAT4 TransformData;
		XMFLOAT4 TransformData2;
		XMFLOAT4 Color;

		bool Equals(const SpriteVertex& source) const
		{
			if (source.TextureId != TextureId)return false;
			if (!XMFloat4Equals(source.TransformData,TransformData))return false;
			if (!XMFloat4Equals(source.TransformData2, TransformData2))return false;
			if (!XMFloat4Equals(source.Color, Color))return false;

			return true;
		}
	};

	friend class Singleton<SpriteRenderer>;
	friend class OverlordGame;
	friend class GameScene;

	SpriteRenderer();
	~SpriteRenderer();

	void InitRenderer(ID3D11Device* pDevice);
	void Draw(const GameContext& gameContext);
	void UpdateBuffer(const GameContext& gameContext);

	vector<SpriteVertex> m_Sprites;
	vector<TextureData*> m_Textures;
	UINT m_BufferSize, m_InputLayoutSize;
	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer, *m_pImmediateVertexBuffer;
	SpriteVertex m_ImmediateVertex;

	ID3DX11EffectMatrixVariable* m_pTransfromMatrixV;
	XMFLOAT4X4 m_Transform;
	ID3DX11EffectVectorVariable* m_pTextureSizeV;
	ID3DX11EffectShaderResourceVariable* m_pTextureSRV;

	static bool SpriteSortByTexture(const SpriteVertex& v0, const SpriteVertex& v1);
	static bool SpriteSortByDepth(const SpriteVertex& v0, const SpriteVertex& v1);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteRenderer(const SpriteRenderer& t);
	SpriteRenderer& operator=(const SpriteRenderer& t);
};

