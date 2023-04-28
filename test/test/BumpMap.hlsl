/******************************************************************************
* 
* @file      BumpMap.hlsl
* @brief     �o���v�}�b�v
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      �}�b�v�e�N�X�`���𗘗p���ăo���v������
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
	float	Rate;			// �ł������
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
	float4 worldPos : TEXCOORD0;
};


Texture2D    Texture : register(t0[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
Texture2D    Texture2 : register(t1[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float4 main(PS_IN input) : SV_TARGET0
{
	float4 color = g_vKd;

	float3 N = Texture2.Sample(Sampler, input.texcoord);
	N = N * 2.0f - 1.0f;
	N = normalize(N);

	float3 L = normalize(g_vLightVector.rgb);

	float DiffuseLig = dot(L, N);
	
	//���˃x�N�g�������߂�
	float3 refVec = reflect(-g_vLightVector.rgb, N);

	//���_�Ɍ������Ă���x�N�g�������߂�
	float3 ToEyes = g_vEye.rgb - input.worldPos.rgb;

	//���K��
	ToEyes = normalize(ToEyes);

	//���ʔ���Phong�̌v�Z
	float specShadow = dot(refVec, ToEyes);

	//�}�C�i�X���O��
	if (specShadow < 0.0f)
	{
		specShadow = 0.0f;
	}

	//���ʔ��˂̋��x��K���ɂ���
	specShadow = pow(specShadow, 3.0f);

	//���ƍ��킹�ċ��ʔ��ˊ���
	float3 specularLig = color.rgb * specShadow;

	//�ŏI�I�Ȍ�
	float3 Lig = DiffuseLig + specularLig;

	float4 Finalcolor = Texture.Sample(Sampler, input.texcoord);

	if (Finalcolor.w == 0.0f)
		Finalcolor = color;
	Finalcolor.rgb *= Lig;

	color.rgb *= Lig;

	return Finalcolor;
}
