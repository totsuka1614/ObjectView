/******************************************************************************
* 
* @file      Fog.hlsl
* @brief     フォグ
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
// グローバル
cbuffer global : register(b0) {
	float4	g_vEye;			// 視点座標
	// 光源
	float4	g_vLightVector;	// 光源方向
	float4	g_vLa;			// 環境光
	float4	g_vLd;			// 拡散反射光
	float4	g_vLs;			// 鏡面反射光
	// マテリアル
	float4	g_vKa;			// アンビエント色(+テクスチャ有無)
	float4	g_vKd;			// ディフューズ色
	float4	g_vKs;			// スペキュラ色(+スペキュラ強度)
	float4	g_vKe;			// エミッシブ色

	float g_fFogStart;
	float g_fFogRange;
};


struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
	float4 worldPos : TEXCOORD0;
};

Texture2D    Texture : register(t0[0]); // TworldPoextureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 main(PS_IN input) : SV_TARGET0
{
	//光の強さ(0.7,0.7,0.7,1.0f)
	float4 color = float4(1.0f,1.0f,1.0f,1.0f);

	float3 L = normalize(-g_vLightVector.xyz);
	float3 N = normalize(input.nor.xyz);
	float3 view = normalize(g_vEye.xyz - input.worldPos.xyz);

	//フォグの計算
	float dist = length(g_vEye.xyz - input.worldPos.xyz);
	float fog = (dist - g_fFogStart) / g_fFogRange;
	fog = saturate(fog);
	float3 fogColor = float3(0.8f, 0.9f, 1.0f);

	float lambert = dot(L, N);

	color.rgb = lerp(lambert, fogColor, fog);

	return color;
}