/******************************************************************************
* 
* @file      Pixel2D.hlsl
* @brief     2D用ピクセルシェーダ
* @author    Totsuka Kensuke
* @date      2023/04/28
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
	float4 Color = input.Diffuse;
	if (g_mTexture._44 > 0.0f) {
		if (g_color != 1.0f)
			Color *= g_texture.Sample(g_sampler, input.TexCoord);
		else
			Color *= g_texture.Sample(g_sampler, input.TexCoord).w;
		
	}

	return Color;
}
