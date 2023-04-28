/******************************************************************************
* 
* @file      DepthWriteVS.hlsl
* @brief     �[�x�}�b�v
* @author    Totsuka Kensuke
* @date      2023/04/28
* @note      �[�x�}�b�v�̒��_����
* @attention 
* 
******************************************************************************/
struct VertexIn
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float2 texcoord : TEXTURE0;
};

struct VertexOut
{
	float4 pos : SV_POSITION0;
	float4 screenPos : TEXCOORD0;
};

cbuffer ConstantBuffer: register(b0)
{
	matrix	mWVP;		// ���[���h�~�r���[�~�ˉe�s��(�]�u�s��)
	matrix	mW;			// ���[���h�s��(�]�u�s��)
	matrix	mTex;		// �e�N�X�`���s��(�]�u�s��)
	float4	fLightVector;
}

VertexOut main(VertexIn input)
{
	VertexOut output;
	output.pos = mul(input.pos, mWVP);

	output.screenPos = output.pos;

	return output;
}