// 2D�p�s�N�Z���V�F�[�_

// �O���[�o��
cbuffer global : register(b0) {
	matrix g_mWorld;
	matrix g_mView;
	matrix g_mProjection;
	matrix g_mTexture;
	float g_color;
	float rand;
	float vignette;
};

// �p�����[�^
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float2	TexCoord	: TEXCOORD0;
	float4	Diffuse		: COLOR0;
};

Texture2D    g_texture : register(t0);	// �e�N�X�`��
SamplerState g_sampler : register(s0);	// �T���v��

float4 main(VS_OUTPUT input) : SV_Target0
{
	float4 Color = input.Diffuse;
	if (g_mTexture._44 > 0.0f) {
		if (g_color != 1.0f)
			Color *= g_texture.Sample(g_sampler, input.TexCoord);
		else
			Color *= g_texture.Sample(g_sampler, input.TexCoord).w;

	}

	
	float2 samplePoint = input.TexCoord;
	float4 Tex = g_texture.Sample(g_sampler, samplePoint);
	float Vignette = length(float2(0.5, 0.5) - input.TexCoord);
	Vignette = clamp(Vignette - vignette, 0, 1);
	Tex.rgb += vignette;
	return Tex;

	//return Color;
}