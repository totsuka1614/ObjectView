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
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float4 main(PS_IN input) : SV_TARGET0
{
	//���̋���(0.7,0.7,0.7,1.0f)
	float4 color = g_vKd;

	//�@���x�N�g��
	float3 normal = normalize(input.nor.rgb);

	//���C�g�x�N�g��
	float3 light = normalize(-g_vLightVector.rgb);

	//�g�U��Lambert�̌v�Z
	float LamShadow = dot(light, normal);

	//�}�C�i�X���O��
	if (LamShadow < 0.0f)
	{
		LamShadow = 0.0f;
	}

	//���ƍ��킹��Lambert����
	float3 DiffuseLig = color.rgb * LamShadow;

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
	specShadow = pow(specShadow,3.0f);

	//���ƍ��킹�ċ��ʔ��ˊ���
	float3 specularLig = color.rgb * specShadow;

	//�ŏI�I�Ȍ�
	float3 Lig = DiffuseLig + specularLig;

	//����
	Lig.x += 0.1f;
	Lig.y += 0.1f;
	Lig.z += 0.1f;

	//color.rgb *= LamShadow;
	// �e�N�X�`���J���[�̎擾
	float4 Finalcolor = Texture.Sample(Sampler, input.texcoord);

	if (Finalcolor.w == 0.0f)
		Finalcolor = 1.0f;
	Finalcolor.xyz *= Lig;

	color.xyz = Lig;

	return Finalcolor;
}