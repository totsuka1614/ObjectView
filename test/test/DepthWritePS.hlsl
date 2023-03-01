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
	float4 pos : SV_POSITION0;
	float4 screenPos : TEXCOORD0;
};


Texture2D    Texture : register(t0[0]); // TworldPoexture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float main(PS_IN input) : SV_TARGET0
{
	float depth = input.screenPos.z / input.screenPos.w;

	return depth;
}