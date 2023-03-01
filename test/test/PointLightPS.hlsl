struct PointLight
{
	float3 pos;
	float range;
	float4 color;
};

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

cbuffer Light : register(b1)
{
	PointLight light[30];
}


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
	float4 color = g_vKd;

	for (int i = 0; i < 30; ++i)
	{
		float3 lightDir = light[i].pos - input.worldPos.xyz;

		float lightLength = length(lightDir);

		float3 L = normalize(lightDir);
		float3 N = normalize(input.nor);
		float d = dot(L, N);
		d = d * 0.5f + 0.5f;

		float attenuation = lightLength / light[i].range;
		attenuation = 1.0f - attenuation;

		attenuation = max(0.0f, attenuation);

		attenuation = pow(attenuation, 2.0f);

		color += light[i].color * d * attenuation;
	}

	return color;
}
