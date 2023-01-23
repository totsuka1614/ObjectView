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

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
	float4 worldPos : TEXCOORD0;
};

Texture2D    Texture : register(t0[0]); // TworldPoexture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
Texture2D    Texture2 : register(t1[0]); // TworldPoexture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float4 main(PS_IN input) : SV_TARGET0
{

	float2 sunUV = input.sunPos.xy / input.sunPos.w;

	sunUV = sunUV * 0.5f + 0.5f;

	sunUV.y = 1.0f - sunUV.y;

	//���̋���(0.7,0.7,0.7,1.0f)
	float4 sunColor = Texture2.Sample(Sampler,sunUV);
	
	float pixcelDepth = input.sunPos.z / input.sunPos.w;

	float4 color = float4(1, 1, 1, 1);

	if ()
	{
		color.rgb = 0.0f;
	}

	return color;
}