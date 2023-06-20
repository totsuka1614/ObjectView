/******************************************************************************
* 
* @file      DissolvePS.hlsl
* @brief     �f�B�\���u
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
//���_�V�F�[�_����f�[�^���󂯎��̂�
//���_�V�F�[�_�\�̏o�͂Ɠ����f�[�^�`���ɂȂ�
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
	float	g_vKe;			// �ł������
};

cbuffer dissolve : register(b1)
{
	float Rate;
}

struct PS_IN
{
	float4 pos : SV_POSITION0;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
};

Texture2D    Texture : register(t0[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
Texture2D    Texture2 : register(t1[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float4 main(PS_IN input) : SV_TARGET0
{
	float4 color = g_vKd;

	float dissolve = Texture2.Sample(Sampler, input.texcoord).r;

	float show = step(Rate, dissolve);
	
	float4 Finalcolor = Texture.Sample(Sampler, input.texcoord);

	if (Finalcolor.w == 0.0f)
		Finalcolor = color;
	Finalcolor.rgba *= show;

	return Finalcolor;
}
