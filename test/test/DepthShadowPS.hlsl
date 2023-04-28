/******************************************************************************
* 
* @file      DepthShadowPS.hlsl
* @brief     深度シャドウ
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      深度マップから影を実装
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
};
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
	float4 worldPos : TEXCOORD0;
	float4 sunPos : TEXCOORD1;
};

Texture2D    Texture : register(t0); // TworldPoextureをスロット0の0番目のテクスチャレジスタに設定
Texture2D    Texture2 : register(t1); // TworldPoextureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 main(PS_IN input) : SV_TARGET0
{
	//光の強さ(0.7,0.7,0.7,1.0f)
	float4 color = g_vKd;

	//法線ベクトル
	float3 normal = normalize(input.nor.rgb);

	//ライトベクトル
	float3 light = normalize(-g_vLightVector.rgb);

	//拡散光Lambertの計算
	float LamShadow = dot(light, normal);

	//マイナスを０に
	if (LamShadow < 0.0f)
	{
		LamShadow = 0.0f;
	}

	//光と合わせてLambert完成
	float3 DiffuseLig = color.rgb * LamShadow;

	//反射ベクトルを求める
	float3 refVec = reflect(g_vLightVector.rgb, normal);

	//視点に向かってくるベクトルを求める
	float3 ToEyes = g_vEye.rgb - input.worldPos.rgb;

	//正規化
	ToEyes = normalize(ToEyes);

	//鏡面反射Phongの計算
	float specShadow = dot(refVec, ToEyes);

	//マイナスを０に
	if (specShadow < 0.0f)
	{
		specShadow = 0.0f;
	}

	//鏡面反射の強度を適当につける
	specShadow = pow(specShadow,3.0f);

	//光と合わせて鏡面反射完成
	float3 specularLig = color.rgb * specShadow;

	//最終的な光
	float3 Lig = DiffuseLig + specularLig;

	//環境光
	Lig.x += 0.1f;
	Lig.y += 0.1f;
	Lig.z += 0.1f;

	//color.rgb *= LamShadow;
	// テクスチャカラーの取得
	float4 Finalcolor = Texture.Sample(Sampler, input.texcoord);

	if (Finalcolor.w == 0.0f)
		Finalcolor = g_vKd;
	Finalcolor.xyz *= Lig;

	float2 sunUV = input.sunPos.xy / input.sunPos.w;

	sunUV = sunUV * 0.5f + 0.5f;

	sunUV.y = 1.0f - sunUV.y;

	//光の強さ(0.7,0.7,0.7,1.0f)
	float sunDepth = Texture2.Sample(Sampler,sunUV).r;
	
	float pixelDepth = input.sunPos.z / input.sunPos.w;

	if (sunDepth + 0.005f < pixelDepth )
	{
		Finalcolor.rgb = 0.0f;
	}


	return Finalcolor;
}