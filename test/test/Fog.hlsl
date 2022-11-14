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
SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 main(PS_IN input) : SV_TARGET0
{
	//光の強さ(0.7,0.7,0.7,1.0f)
	float4 color = float4(1.0f,1.0f,1.0f,1.0f);

	float3 light = normalize(-g_vLightVector.xyz);
	float3 normal = normalize(input.nor.xyz);
	float3 view = normalize(g_vEye.xyz - input.worldPos.xyz);

	float edge = min(1.0f - dot(normal, view), 1.0f);

	float3 EdgeColor = float3(1.0f, 0.0f, 0.0f);

	color.rgb *= edge * EdgeColor;

	return color;
}