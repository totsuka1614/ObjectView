// 2D用ピクセルシェーダ

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
	float4 Color = input.Diffuse;
	if (g_mTexture._44 > 0.0f) {
		if (g_color != 1.0f)
			Color *= g_texture.Sample(g_sampler, input.TexCoord);
		else
			Color *= g_texture.Sample(g_sampler, input.TexCoord).w;

	}

	//float2 p = floor(rand);
	//float2 f = frac(rand);
	//float2 u = f * f * (3.0 - 2.0 * f);

	//float v00 = frac(sin(dot((float2(p + float2(0, 0))), float2(12.9898, 78.233))) * 43758.5453);
	//float v10 = frac(sin(dot((float2(p + float2(1, 0))), float2(12.9898, 78.233))) * 43758.5453);
	//float v01 = frac(sin(dot((float2(p + float2(0, 1))), float2(12.9898, 78.233))) * 43758.5453);
	//float v11 = frac(sin(dot((float2(p + float2(1, 1))), float2(12.9898, 78.233))) * 43758.5453);

	//float perlin = lerp(lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
	//	           lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
 //       		   u.y) + 0.5f;
	
	/*float vertNoise = frac(sin(dot(float2(floor((input.TexCoord.x) / VertGlitchPase) * VertGlitchPase, time * 0.1), float2(8.7819, 3.255))) * 437.645);
	float horzNoise = frac(sin(dot(float2(floor((input.TexCoord.y) / HorzGlitchPase) * HorzGlitchPase, time * 0.2), float2(8.7819, 3.255))) * 437.645);
	float vertGlitchStrength = vertNoise / GlitchStepValue;
	float horzGlitchStrength = horzNoise / GlitchStepValue;
	vertGlitchStrength = vertGlitchStrength * 2.0 - 1.0;
	horzGlitchStrength = horzGlitchStrength * 2.0 - 1.0;
	float V = step(vertNoise, GlitchStepValue * 2) * vertGlitchStrength;
	float H = step(horzNoise, GlitchStepValue) * horzGlitchStrength;

	float2 samplePoint = input.TexCoord;
	float sinv = sin(samplePoint.y * 2 - time * -0.1);
	float steped = 1 - step(0.99, sinv * sinv);
	float timeFrac = steped * step(0.8, frac(time));
	samplePoint.x += timeFrac * (V + H);
	float4 Tex = g_texture.Sample(g_sampler, samplePoint);
	return Tex;*/

	float2 samplePoint = input.TexCoord;
	float4 Tex = g_texture.Sample(g_sampler, samplePoint);
	float sinv = sin(input.TexCoord.y * 2 + time * -0.1);
	float steped = step(1, sinv * sinv);
	Tex.rgb -= (1 - steped) * abs(sin(input.TexCoord.y * 50.0 + time * 1.0)) * 0.05;
	Tex.rgb -= (1 - steped) * abs(sin(input.TexCoord.y * 100.0 - time * 2.0)) * 0.08;
	//Tex.rgb += steped * 0.1;
	return Tex;
}
