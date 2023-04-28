/******************************************************************************
* 
* @file      EdgeVS.hlsl
* @brief     ふち頂点シェーダ
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
struct VertexIn
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float2 texcoord : TEXTURE0;
};

struct VertexOut
{
	float4 pos : SV_POSITION0;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
	float4 worldPos : TEXCOORD0;
	float4 OutlinePos : TEXCOORD1;
};

cbuffer ConstantBuffer: register(b0)
{
	matrix	mWVP;		// ワールド×ビュー×射影行列(転置行列)
	matrix	mW;			// ワールド行列(転置行列)
	matrix	mTex;		// テクスチャ行列(転置行列)
	float4	fLightVector;
}

VertexOut main(VertexIn input)
{
	VertexOut output;
	input.nor.w = 0.0f;
	output.pos = mul(input.pos, mWVP);
	output.nor = mul(input.nor, mW).xyzw;
	output.worldPos = mul(input.pos, mW);
	output.texcoord = input.texcoord;

	output.pos = input.pos + input.nor * 1.0f;
	output.pos = mul(output.pos, mWVP);
	return output;
}