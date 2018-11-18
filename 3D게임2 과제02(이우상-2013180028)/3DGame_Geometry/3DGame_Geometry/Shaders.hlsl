//#define _WITH_CONSTANT_BUFFER_SYNTAX

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
struct CB_PLAYER_INFO
{
	matrix		mtxWorld;
}; 

struct CB_GAMEOBJECT_INFO
{
	matrix		mtxWorld;
};

struct CB_CAMERA_INFO
{
	matrix		mtxView;
	matrix		mtxProjection;
};

ConstantBuffer<CB_PLAYER_INFO> gcbPlayerObjectInfo : register(b0);
ConstantBuffer<CB_CAMERA_INFO> gcbCameraInfo : register(b1);
ConstantBuffer<CB_GAMEOBJECT_INFO> gcbGameObjectInfo : register(b2);
#else
cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView						: packoffset(c0);
	matrix		gmtxProjection				: packoffset(c4);
	//matrix		gmtxViewProjection		: packoffset(c8);
	//float3		gmtxCameraPosition	: packoffset(c12);
	float3		gmtxCameraPosition	: packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxWorld : packoffset(c0);
};

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_DIFFUSED_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSDiffused(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
#endif
	output.color = input.color;

	return(output);
}

float4 PSDiffused(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbPlayerObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
#endif
	output.color = input.color;

	return(output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
	return(input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture2D gtxtTexture : register(t0);
SamplerState gWrapSamplerState : register(s0);
SamplerState gClampSamplerState : register(s1);

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
#endif
	output.uv = input.uv;

	return(output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input, uint primitiveID : SV_PrimitiveID) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);

	return(cColor);
}

float4 PSBillboard(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.SampleLevel(gWrapSamplerState, input.uv, 0);
	//	float4 cColor = gtxtTexture.Sample(gWrapSamplerState, input.uv);

	// 알파 블랜딩 사용으로 discard를 쓸 필요가 없음
	//if (cColor.a <= 0.3f) discard; 
	//clip(cColor.a - 0.3f);

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 PSSkyBox(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gClampSamplerState, input.uv);

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Texture2D gtxtTerrainBaseTexture	: register(t1);
Texture2D gtxtTerrainDetailTexture	: register(t2);
Texture2D gtxtSandTexture				: register(t3);

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;

	float2 uv2 : TEXCOORD2;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;

	float2 uv2 : TEXCOORD2;
};

struct GS_TERRAIN_OUTPUT
{
	float4 position	: SV_POSITION;
	float4 color		: COLOR;
	float2 uv0			: TEXCOORD0;
	float2 uv1			: TEXCOORD1;

	float2 uv2			: TEXCOORD2;
};

VS_TERRAIN_OUTPUT VS_Terrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

#ifdef _WITH_CONSTANT_BUFFER_SYNTAX
	output.position = mul(mul(mul(float4(input.position, 1.0f), gcbGameObjectInfo.mtxWorld), gcbCameraInfo.mtxView), gcbCameraInfo.mtxProjection);
#else
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
#endif
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;
	output.uv2 = input.uv2;

	return(output);
}

float4 PS_Terrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gWrapSamplerState, input.uv0);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gWrapSamplerState, input.uv1);
	
	float4 cSandTexColor = gtxtSandTexture.Sample(gWrapSamplerState, input.uv2);

	float4 cColor;
	
	if (input.uv2.r > 0.0f)
		cColor = cSandTexColor;
	else
		cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

	return(cColor);
}
// 기하 셰이더(터레인)
[maxvertexcount(8)]
void GS_Terrain(triangle VS_TERRAIN_OUTPUT input[3], inout TriangleStream<GS_TERRAIN_OUTPUT> outStream)
{
	GS_TERRAIN_OUTPUT v[6];

	v[0].position = input[0].position;
	v[0].color = input[0].color;
	v[0].uv0 = input[0].uv0;
	v[0].uv1 = input[0].uv1;
	v[0].uv2 = input[0].uv2;

	v[1].position = input[1].position;
	v[1].color = input[1].color;
	v[1].uv0 = input[1].uv0;
	v[1].uv1 = input[1].uv1;
	v[1].uv2 = input[1].uv2;

	v[2].position = input[2].position;
	v[2].color = input[2].color;
	v[2].uv0 = input[2].uv0;
	v[2].uv1 = input[2].uv1;
	v[2].uv2 = input[2].uv2;

	v[3].position = (input[0].position + input[1].position) * 0.5f;
	v[3].color = (input[0].color + input[1].color + float4(1.0f, 0.0f, 0.0f, 1.0f)) / 3.0f;
	v[3].uv0 = (input[0].uv0 + input[1].uv0) * 0.5f;
	v[3].uv1 = (input[0].uv1 + input[1].uv1) * 0.5f;
	v[3].uv2 = (input[0].uv2 + input[1].uv2) * 0.5f;

	v[4].position = (input[1].position + input[2].position) * 0.5f;
	v[4].color = (input[1].color + input[2].color + float4(0.0f, 1.0f, 0.0f, 1.0f)) / 3.0f;
	v[4].uv0 = (input[1].uv0 + input[2].uv0) * 0.5f;
	v[4].uv1 = (input[1].uv1 + input[2].uv1) * 0.5f;
	v[4].uv2 = (input[1].uv2 + input[2].uv2) * 0.5;

	v[5].position = (input[0].position + input[2].position)*0.5f;
	v[5].color = (input[0].color + input[2].color + float4(0.0f, 0.0f, 1.0f, 1.0f)) / 3.0f;
	v[5].uv0 = (input[0].uv0 + input[2].uv0) *0.5f;
	v[5].uv1 = (input[0].uv1 + input[2].uv1) * 0.5f;
	v[5].uv2 = (input[0].uv2 + input[2].uv2) * 0.5f;

	outStream.Append(v[0]);
	outStream.Append(v[3]);
	outStream.Append(v[5]);
	outStream.Append(v[4]);
	outStream.Append(v[2]);

	outStream.RestartStrip();

	outStream.Append(v[3]);
	outStream.Append(v[1]);
	outStream.Append(v[4]);
}
float4 PS_GSTerrain(GS_TERRAIN_OUTPUT input) : SV_Target
{
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gWrapSamplerState, input.uv0);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gWrapSamplerState, input.uv1);

	float4 cSandTexColor = gtxtSandTexture.Sample(gWrapSamplerState, input.uv2);

	float4 cColor;

	if (input.uv2.r > 0.0f)
		cColor = cSandTexColor;
	else
		cColor = input.color * saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

	return(cColor);
}

// 기하 셰이더(빌보드 오브젝트)
Texture2DArray gtxtTextureArray : register(t4);

struct VS_IN
{
	float3 posW : POSITION;
	float2 sizeW : SIZE;
};
struct VS_OUT
{
	float3 centerW	: POSITION;
	float2 sizeW		: SIZE;
};
struct GS_OUT
{
	float4 posH			: SV_POSITION;
	float3 posW			: POSITION;
	float3 normalW	: NORMAL;
	float2 uv				: TEXCOORD;
	uint primID			: SV_PrimitiveID;
};

VS_OUT VS_Geometry(VS_IN input)
{
	VS_OUT output;
	output.centerW = input.posW;
	output.sizeW = input.sizeW;
	
	return output;
}

[maxvertexcount(4)]
void GS(point VS_OUT input[1], uint primID : SV_PrimitiveID, inout TriangleStream<GS_OUT> outStream)
{
	float3 vUp = float3(0.0f, 1.0f, 0.0f);
	float3 vLook = gmtxCameraPosition.xyz - input[0].centerW;
	vLook = normalize(vLook);
	float3 vRight = cross(vUp, vLook);

	float fHalfW = input[0].sizeW.x * 0.5f;
	float fHalfH = input[0].sizeW.y * 0.5f;

	// 사각형 정점들을 월드변환행렬로 변환하고, 그것들을 하나의 삼각형으로 출력
	float4 pVertices[4];
	pVertices[0] = float4(input[0].centerW + fHalfW * vRight - fHalfH * vUp, 1.0f);
	pVertices[1] = float4(input[0].centerW + fHalfW * vRight + fHalfH * vUp, 1.0f);
	pVertices[2] = float4(input[0].centerW - fHalfW * vRight - fHalfH * vUp, 1.0f);
	pVertices[3] = float4(input[0].centerW - fHalfW * vRight + fHalfH * vUp, 1.0f);

	float2 pUVs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

	GS_OUT output;
	for (int i = 0; i < 4; ++i)
	{
		output.posW = pVertices[i].xyz;
		output.posH = mul(mul(pVertices[i], gmtxView), gmtxProjection);
		output.normalW = vLook;
		output.uv = pUVs[i];
		output.primID = primID;

		outStream.Append(output);
	}
}

float4 PS_Geometry(GS_OUT input) : SV_Target
{
	//float4 cillumination = Lighting(input.posW, input.normalW);
	// 텍스처 배열에 텍스처가 3개있음
	float3 uvw = float3(input.uv, (input.primID % 3));
	float4 cTexture = gtxtTextureArray.Sample(gWrapSamplerState, uvw);
	//float4 cTexture = gtxtTexture.Sample(gWrapSamplerState, input.uv);
	//float4 cColor = cillumination * cTexture;
	float4 cColor = cTexture;
	//cColor.a = cTexture.a;

	return (cColor);
}
