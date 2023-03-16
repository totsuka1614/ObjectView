/******************************************************************************
* 
* @file      PP_Noiz.hlsl
* @brief     シンプルノイズ
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/

// グローバル
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
	float g_color;
	float rand;
};

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

Texture2D    g_texture : register(t0);	// テクスチャ
SamplerState g_sampler : register(s0);	// サンプラ

float4 main(VS_OUTPUT input) : SV_Target0
{
	//シンプルノイズ
	float2 samplePoint = input.TexCoord;
	float4 Tex = g_texture.Sample(g_sampler, samplePoint);
	float noise = frac(sin(dot(input.TexCoord * rand, float2(8.7819, 3.255))) * 437.645) - 0.5;
	Tex.rgb += float3(noise, noise, noise);
	return Tex;

}
