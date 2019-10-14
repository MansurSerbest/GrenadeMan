float4x4 gTransform : WorldViewProjection;
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

DepthStencilState NoDepth
{
	DepthEnable = FALSE;
};

RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	uint TextureId: TEXCOORD0;
	float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
	float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
	float4 Color: COLOR;	
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
	if (rotation != 0)
	{
		//Step 3.
		//Do rotation calculations
		//Transform to origin
		//Rotate
		//Retransform to initial position

	    pos.x = pos.x - offset.x - pivotOffset.x;
		pos.y = pos.y - offset.y - pivotOffset.y;

		float newPosx = pos.x * rotCosSin.x - pos.y*rotCosSin.y;
		float newPosy = pos.y * rotCosSin.x + pos.x*rotCosSin.y;
		pos.x = newPosx;
		pos.y = newPosy;

		pos.x = pos.x + offset.x + pivotOffset.x;
		pos.y = pos.y + offset.y + pivotOffset.y;
	}
	else
	{
		//Step 2.
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
		pos.x -= pivotOffset.x;
		pos.y -= pivotOffset.y;
	}

	//Geometry Vertex Output
	GS_DATA geomData = (GS_DATA) 0;
	geomData.Position = mul(float4(pos, 1.0f), gTransform);
	geomData.Color = col;
	geomData.TexCoord = texCoord;
	triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
	float3 position = float3(vertex[0].TransformData.x,vertex[0].TransformData.y,vertex[0].TransformData.z); //Extract the position data from the VS_DATA vertex struct
	float2 offset = float2(vertex[0].TransformData.x, vertex[0].TransformData.y); //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
	float rotation = vertex[0].TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
	float2 pivot = float2(vertex[0].TransformData2.x,vertex[0].TransformData2.y); //Extract the pivot data from the VS_DATA vertex struct
	float2 scale = float2(vertex[0].TransformData2.z,vertex[0].TransformData2.w); //Extract the scale data from the VS_DATA vertex struct
	float2 texCoord = float2(0, 0); //Initial Texture Coordinate
	
	//...
	
	//Positions
	float3 position1;
	position1.x = offset.x;
	position1.y = offset.y;
	position1.z = position.z;
	float2 texCoord1 = float2(0,0);
	
	float3 position2;
	position2.x = offset.x + scale.x * gTextureSize.x;
	position2.y = offset.y;
	position2.z = position.z;
	float2 texCoord2 = float2(1, 0);
	
	float3 position3;
	position3.x = offset.x + scale.x *  gTextureSize.x;
	position3.y = offset.y + scale.y *  gTextureSize.y;
	position3.z = position.z;
	float2 texCoord3 = float2(0, 1);
	
	float3 position4;
	position4.x = offset.x;
	position4.y = offset.y + scale.y *  gTextureSize.y;
	position4.z = position.z;
	float2 texCoord4 = float2(1, 1);

	//Offset
	float2 pivotOffset = float2(gTextureSize.x * pivot.x * scale.x, gTextureSize.y * pivot.y * scale.y);

	//Rotation
	float2 rotCosSin;
	if (rotation == 0)
	{
		rotCosSin = float2(0, 0);
	}
	else
	{
		rotCosSin.x = cos(rotation);
		rotCosSin.y = sin(rotation);
	}

	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB

	//VERTEX 1 [LT]
	CreateVertex(triStream, position1, float4(1,1,1,1), texCoord1, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 2 [RT]
	CreateVertex(triStream, position2, float4(1,1,1,1), texCoord2, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 3 [LB]
	CreateVertex(triStream, position4, float4(1,1,1,1), texCoord3, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 4 [RB]
	CreateVertex(triStream, position3, float4(1,1,1,1), texCoord4, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET {

	return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;	
}

// Default Technique
technique11 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(NoDepth,0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
