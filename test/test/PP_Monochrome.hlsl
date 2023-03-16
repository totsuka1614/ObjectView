/******************************************************************************
* 
* @file      PP_Monochrome.hlsl
* @brief     モノクロ
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
	float4 Color = input.Diffuse;
	
	//モノクロ
	float y = 0.299*Color.r + 0.587*Color.g + 0.114*Color.b;
	float i = 0.596*Color.r - 0.274*Color.g - 0.322*Color.b;
	float q = 0.211*Color.r - 0.522*Color.g + 0.311*Color.b;

	Color.r = y;
	Color.g = y;
	Color.b = y;

	return Color;
}
