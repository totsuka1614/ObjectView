/******************************************************************************
* 
* @file      BumpVS.hlsl
* @brief     �o���v���_�V�F�[�_
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      
* @attention 
* 
******************************************************************************/
cbuffer ConstantBuffer: register(b0)
{
	matrix	mWVP;		// ���[���h�~�r���[�~�ˉe�s��(�]�u�s��)
	matrix	mW;			// ���[���h�s��(�]�u�s��)
	matrix	mTex;		// �e�N�X�`���s��(�]�u�s��)
	float4	fLightVector;
}
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
	float3 tangent : TANGENT0; //�ڃx�N�g��
};

struct VertexOut
{
	float4 pos : SV_POSITION0;
	float4 nor : NORMAL;
	float2 texcoord : TEXTURE0;
	float4 worldPos : TEXCOORD0;
	float3 tangentL : TEXCOORD1;
};


VertexOut main(PS_IN input)
{
	VertexOut output;
	input.nor.w = 0.0f;
	output.pos = mul(input.pos, mWVP);
	output.nor = mul(input.nor, mW).xyzw;
	output.worldPos = mul(input.pos, mW);
	output.texcoord = input.texcoord;

	//�ԂՂ܂��Ղ̂��߂Ƀ��C�g��]
	float3 T = normalize(input.tangent);
	float3 N = normalize(output.nor);
	float3 B = normalize(cross(N, T));

	float3x3 bumpRot = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z
		);

	float3 L = normalize(fLightVector.xyz);
	output.tangentL = mul(L, bumpRot);

	return output;
}