/******************************************************************************
* 
* @file      PP_Glitch.hlsl
* @brief     グリッチノイズ
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      未完成
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
	float vignette;
	float VertGlitchPase;
	float HorzGlitchPase;
	float GlitchStepValue;
	float time;
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
	//ToDo:未完成状態,
	float2 samplePoint = input.TexCoord;
	float4 Tex = g_texture.Sample(g_sampler, samplePoint);
	float sinv = sin(input.TexCoord.y * 2 + time * -0.1);
	float steped = step(1, sinv * sinv);
	Tex.rgb -= (1 - steped) * abs(sin(input.TexCoord.y * 50.0 + time * 1.0)) * 0.05;
	Tex.rgb -= (1 - steped) * abs(sin(input.TexCoord.y * 100.0 - time * 2.0)) * 0.08;
	
	return Tex;
}
