// グローバル
cbuffer global : register(b0) {
	float4	g_vEye;			// 視点座標
	// 光源
	float3	g_vLightVector;	// 光源方向
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
	float4 OutlinePos : TEXCOORD1;
};

Texture2D    Texture : register(t0[0]); // Textureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 main(PS_IN input) : SV_Target0
{
	float4 color = input.nor;

	if (color.r < 0.0f)
		color.r = 0.0f;
	if (color.g < 0.0f)
		color.g = 0.0f;
	if (color.b < 0.0f)
		color.b = 0.0f;

	return color;
}