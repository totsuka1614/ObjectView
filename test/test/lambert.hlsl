/******************************************************************************
* 
* @file      lambert.hlsl
* @brief     ランバート
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      拡散光
* @attention 
* 
******************************************************************************/
//頂点シェーダからデータを受け取るので
//頂点シェーダ―の出力と同じデータ形式になる
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
};

struct PS_IN
{
	float4 pos : SV_POSITION0;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
};

Texture2D    Texture : register(t0[0]); // Textureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 main(PS_IN input) : SV_TARGET0
{
	float4 color = g_vKd;

	float3 normal = normalize(input.nor.rgb);

	float3 light = normalize(-g_vLightVector.rgb);

	float shadow = dot(light, normal);

	//マイナスを０に
	if (shadow < 0.0f)
	{
		shadow = 0.0f;
	}

	color.rgb *= shadow;

	float4 Finalcolor = Texture.Sample(Sampler, input.texcoord);

	if (Finalcolor.w == 0.0f)
		Finalcolor = 1.0f;
	Finalcolor.xyz *= color;

	return Finalcolor;
}
