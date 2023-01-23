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
};

Texture2D    Texture : register(t0[0]); // TworldPoextureをスロット0の0番目のテクスチャレジスタに設定
Texture2D    Texture2 : register(t1[0]); // TworldPoextureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 main(PS_IN input) : SV_TARGET0
{

	float2 sunUV = input.sunPos.xy / input.sunPos.w;

	sunUV = sunUV * 0.5f + 0.5f;

	sunUV.y = 1.0f - sunUV.y;

	//光の強さ(0.7,0.7,0.7,1.0f)
	float4 sunColor = Texture2.Sample(Sampler,sunUV);
	
	float pixcelDepth = input.sunPos.z / input.sunPos.w;

	float4 color = float4(1, 1, 1, 1);

	if ()
	{
		color.rgb = 0.0f;
	}

	return color;
}