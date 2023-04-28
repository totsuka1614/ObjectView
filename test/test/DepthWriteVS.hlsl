/******************************************************************************
* 
* @file      DepthWriteVS.hlsl
* @brief     深度マップ
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      深度マップの頂点生成
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
	float4 screenPos : TEXCOORD0;
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
	output.pos = mul(input.pos, mWVP);

	output.screenPos = output.pos;

	return output;
}