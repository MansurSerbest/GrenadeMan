//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;
float gIntensity = 0.0f;
float gRange = 0.1f;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepthWrite
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
	output.Position = float4(input.Position, 1.0f);
	// Set the TexCoord
	output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	float xTex = input.TexCoord.x;
	float yTex = input.TexCoord.y;
	float4 finalColor = (float4)0;


	if (xTex < gRange)
	{
		if (yTex < 1.0f - gRange)
		{
			if (xTex <= yTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (xTex / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (yTex / gRange)) * gIntensity);
			}
		}
		else
		{
			if (xTex <= 1.0f - yTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (xTex / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f - yTex) / gRange)) * gIntensity);
			}
		}
	}
	else if (xTex > 1.0f - gRange)
	{
		if (yTex < 0.9f)
		{
			if (1.0f - xTex <= yTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f - xTex) / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (yTex / gRange)) * gIntensity);
			}
		}
		else
		{
			if (1.0f - xTex <= 1.0f - yTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f- xTex) / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f - yTex) / gRange)) * gIntensity);
			}
		}
	}

	if (yTex < gRange)
	{
		if (xTex < 1.0f - gRange)
		{
			if (yTex <= xTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (yTex / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (xTex / gRange)) * gIntensity);
			}
		}
		else
		{
			if (yTex <= 1.0f - xTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (yTex / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f - xTex) / gRange)) * gIntensity);
			}
		}
	}
	else if (yTex > 0.9f)
	{
		if (xTex < 0.9f)
		{
			if (1.0f - yTex <= xTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f - yTex) / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - (xTex / gRange)) * gIntensity);
			}
		}
		else
		{
			if (1.0f - yTex <= 1.0f - xTex)
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f - yTex) / gRange)) * gIntensity);
			}
			else
			{
				finalColor = float4(1.0, 0.0f, 0.0f, (1.0f - ((1.0f - xTex) / gRange)) * gIntensity);
			}
		}
	}

	return finalColor;
}


//TECHNIQUE
//---------
technique11 Timer
{
    pass P0
    {
		// Set states...
		SetRasterizerState(BackCulling);
		SetDepthStencilState(EnableDepthWrite, 0);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}