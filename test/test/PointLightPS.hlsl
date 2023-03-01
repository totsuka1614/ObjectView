struct PointLight
{
	float3 pos;
	float range;
	float4 color;
};

// �O���[�o��
cbuffer global : register(b0) {
	float4	g_vEye;			// ���_���W
	// ����
	float4	g_vLightVector;	// ��������
	float4	g_vLa;			// ����
	float4	g_vLd;			// �g�U���ˌ�
	float4	g_vLs;			// ���ʔ��ˌ�
	// �}�e���A��
	float4	g_vKa;			// �A���r�G���g�F(+�e�N�X�`���L��)
	float4	g_vKd;			// �f�B�t���[�Y�F
	float4	g_vKs;			// �X�y�L�����F(+�X�y�L�������x)
	float4	g_vKe;			// �G�~�b�V�u�F
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

Texture2D    Texture : register(t0[0]); // TworldPoexture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float4 main(PS_IN input) : SV_TARGET0
{
	//���̋���(0.7,0.7,0.7,1.0f)
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
