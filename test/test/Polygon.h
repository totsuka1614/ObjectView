//=============================================================================
//
// 2D�|���S�������N���X��` [Polygon.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "ConstantBuffer.h"
#include "BackBuffer.h"

class CPolygon
{
private:
	static ID3D11ShaderResourceView* m_pTexture;	// �e�N�X�`���ւ̃|�C���^

	static VERTEX_2D m_vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	static XMFLOAT3 m_vPos;							// �|���S���̈ړ���
	static XMFLOAT3 m_vAngle;						// �|���S���̉�]��
	static XMFLOAT3 m_vScale;						// �|���S���̃T�C�Y
	static XMFLOAT4 m_vColor;						// �|���S���̒��_�J���[
	static bool m_bInvalidate;						// ���_�f�[�^�X�V�t���O

	static XMFLOAT2 m_vPosTexFrame;					// UV���W
	static XMFLOAT2 m_vSizeTexFrame;				// �e�N�X�`�����o�T�C�Y

	static ConstantBuffer* m_pConstantBuffer;			// �萔�o�b�t�@
	static ID3D11Buffer* m_pVertexBuffer;			// ���_�o�b�t�@
	static ID3D11InputLayout* m_pInputLayout;		// ���_�t�H�[�}�b�g

	static XMFLOAT4X4 m_mProj;						// �ˉe�ϊ��s��
	static XMFLOAT4X4 m_mView;						// �r���[�ϊ��s��
	static XMFLOAT4X4 m_mWorld;						// ���[���h�ϊ��s��
	static XMFLOAT4X4 m_mTex;						// �e�N�X�`���ϊ��s��

private:
	static HRESULT MakeVertex(ID3D11Device* pDevice);
	static void SetVertex(void);

public:
	static HRESULT Init();
	static void Fin();
	static void Draw();
	static void SetTexture(ID3D11ShaderResourceView* pTexture);
	static void SetPos(float fX, float fY);
	static void SetSize(float fScaleX, float fScaleY);
	static void SetAngle(float fAngle);
	static void SetUV(float fU, float fV);
	static void SetFrameSize(float fWidth, float fHeight);
	static void SetColor(float fRed, float fGreen, float fBlue);
	static void SetColor(XMFLOAT3 vColor);
	static void SetAlpha(float fAlpha);
	static void SetColor(float fRed, float fGreen, float fBlue, float fAlpha);
	static void SetColor(XMFLOAT4 vColor);
};
