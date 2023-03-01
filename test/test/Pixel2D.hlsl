// 2D用ピクセルシェーダ

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

	//ノイズ
	//float2 samplePoint = input.TexCoord;
	//float4 Tex = g_texture.Sample(g_sampler, samplePoint);
	//float noise = frac(sin(dot(input.TexCoord * rand, float2(8.7819, 3.255))) * 437.645) - 0.5;
	//Tex.rgb += float3(noise, noise, noise);
	//return Tex;
	
	
	//モノクロ
	//float y = 0.299*Color.r + 0.587*Color.g + 0.114*Color.b;
	//float i = 0.596*Color.r - 0.274*Color.g - 0.322*Color.b;
	//float q = 0.211*Color.r - 0.522*Color.g + 0.311*Color.b;

	//i = 0;
	//q = 0;

	//Color.r = y;
	//Color.g = y;
	//Color.b = y;

	//Color.r = 1 - Color.r;
	//Color.g = 1 - Color.g;
	//Color.b = 1 - Color.b;

	return Color;
}
