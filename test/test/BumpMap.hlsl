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
	float4 pos : SV_POSITION0;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
};

Texture2D    Texture : register(t0[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float4 main(PS_IN input) : SV_TARGET0
{
	float4 color = g_vKd;

	//color.rgb = Texture.Sample(Sampler, input.texcoord);

	float3 N = Texture.Sample(Sampler, input.texcoord);
	N = N * 2.0f - 1.0f;
	N = normalize(N);

	float3 L = normalize(-g_vLightVector.rgb);

	float DiffuseLig = dot(L, N);
	
	//���˃x�N�g�������߂�
	float3 refVec = reflect(g_vLightVector.rgb, normal);

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

	color.rgb *= shadow;

	return color;
}
