float4x4 gTransform : WORLDVIEWPROJECTION;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	int Channel : TEXCOORD2; //Texture Channel
	float3 Position : POSITION; //Left-Top Character Quad Starting Position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Left-Top Character Texture Coordinate on Texture
	float2 CharSize: TEXCOORD1; //Size of the character (in screenspace)
};

struct GS_DATA
{
	float4 Position : SV_POSITION; //HOMOGENEOUS clipping space position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Texcoord of the vertex
	int Channel: TEXCOORD1; //Channel of the vertex
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, int channel)
{

	//Create a new GS_DATA object
	GS_DATA output = (GS_DATA)0;
	//Fill in all the fields
	output.Channel = channel;
	output.Position = mul(float4(pos, 1.0f), gTransform);
	output.Color = col;
	output.TexCoord = texCoord;

	//Append it to the TriangleStream
	triStream.Append(output);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//REMOVE THIS >
	//GS_DATA dummyData = (GS_DATA) 0; //Just some dummy data
	//triStream.Append(dummyData); //The geometry shader needs to emit something, see what happens if it doesn't emit anything.
	//< STOP REMOVING

	//Create a Quad using the character information of the given vertex
	//Note that the Vertex.CharSize is in screenspace, TextureCoordinates aren't ;) [Range 0 > 1]
	float2 charSize = vertex[0].CharSize;
	float3 vertPos = vertex[0].Position;
	float4 vertColor = vertex[0].Color;
	int channel = vertex[0].Channel;
	float2 vertUV = vertex[0].TexCoord;

	//1. Vertex Left-Top
	//CreateVertex(...);
	float3 LeftTopPos = vertPos;
	float2 LeftTopUV = vertUV;

	//2. Vertex Right-Top
	float3 RightTopPos;
	RightTopPos.x = vertPos.x + charSize.x;
	RightTopPos.y = vertPos.y;
	RightTopPos.z = vertPos.z;
	float2 RightTopPosUV = float2(vertUV.x + charSize.x / gTextureSize.x, vertUV.y);

	//3. Vertex Left-Bottom
	float3 LeftBottomPos;
	LeftBottomPos.x = vertPos.x;
	LeftBottomPos.y = vertPos.y + charSize.y;
	LeftBottomPos.z = vertPos.z;
	float2 LeftBottomPosUV = float2(vertUV.x, vertUV.y + charSize.y / gTextureSize.y);

	//4. Vertex Right-Bottom
	float3 RightBottomPos;
	RightBottomPos.x = vertPos.x + charSize.x;
	RightBottomPos.y = vertPos.y + charSize.y;
	RightBottomPos.z = vertPos.z;
	float2 RightBottomPosUV = float2(vertUV.x + charSize.x / gTextureSize.x, vertUV.y + charSize.y / gTextureSize.y);

	CreateVertex(triStream, LeftTopPos, vertColor, LeftTopUV, channel);
	CreateVertex(triStream, RightTopPos, vertColor, RightTopPosUV, channel);
	CreateVertex(triStream, LeftBottomPos, vertColor, LeftBottomPosUV, channel);
	CreateVertex(triStream, RightBottomPos, vertColor, RightBottomPosUV, channel);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET {
	
	//Sample the texture and return the correct channel [Vertex.Channel]
	float4 color = gSpriteTexture.Sample(samPoint, input.TexCoord);
	//You can iterate a float4 just like an array, using the index operator
	//Also, don't forget to colorize ;) [Vertex.Color]
	return color[input.Channel] * input.Color;
	//return input.Color; //TEMP, just to test if the rectangles are visible on the screen
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

// Default Technique
technique10 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
