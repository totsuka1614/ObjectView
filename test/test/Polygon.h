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
#include <string.h>
#include "ObjectBase.h"

class CPolygon : public ObjectBase
{

public:
	// �V�F�[�_�ɓn���l
	struct SHADER_GLOBAL {
		XMMATRIX	mWorld;		// ���[���h�ϊ��s��(�]�u�s��)
		XMMATRIX	mView;		// �r���[�ϊ��s��(�]�u�s��)
		XMMATRIX	mProj;		// �ˉe�ϊ��s��(�]�u�s��)
		XMMATRIX	mTex;		// �e�N�X�`���ϊ��s��(�]�u�s��)
		float bColor;
		float rand;
		float vignette;
		float VertGlitchPase;
		float HorzGlitchPase;
		float GlitchStepValue;
		float time;
	};

	CPolygon(){}

	virtual void Init();
	virtual void Fin();
	virtual void Draw();
	void SetTexture(ID3D11ShaderResourceView* pTexture);
	void SetTexture(const char* path);
	void SetPos(float fX, float fY);
	void SetPos(XMFLOAT2 xy) { m_vPos.x = xy.x; m_vPos.y = xy.y; }
	void SetPos(XMFLOAT3 xyz) { m_vPos = xyz; }
	void SetSize(float fScaleX, float fScaleY);
	void SetAngle(float fAngle);
	void SetUV(float fU, float fV);
	void SetFrameSize(float fWidth, float fHeight);
	void SetColor(float fRed, float fGreen, float fBlue);
	void SetColor(XMFLOAT3 vColor);
	void SetAlpha(float fAlpha);
	void SetColor(float fRed, float fGreen, float fBlue, float fAlpha);
	void SetColor(XMFLOAT4 vColor);
	void UseColor(bool use = true) { if (use)m_fColor = 1.0f; else m_fColor = 0.0f; }
protected:
	XMFLOAT4 m_vColor;						// �|���S���̒��_�J���[

	ID3D11ShaderResourceView* m_pTexture;	// �e�N�X�`���ւ̃|�C���^

	VERTEX_2D m_vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
	bool m_bInvalidate;						// ���_�f�[�^�X�V�t���O

	XMFLOAT2 m_vPosTexFrame;					// UV���W
	XMFLOAT2 m_vSizeTexFrame;				// �e�N�X�`�����o�T�C�Y

	ConstantBuffer* m_pConstantBuffer;			// �萔�o�b�t�@
	ID3D11Buffer* m_pVertexBuffer;			// ���_�o�b�t�@
	ID3D11InputLayout* m_pInputLayout;		// ���_�t�H�[�}�b�g

	XMFLOAT4X4 m_mProj;						// �ˉe�ϊ��s��
	XMFLOAT4X4 m_mView;						// �r���[�ϊ��s��
	XMFLOAT4X4 m_mWorld;						// ���[���h�ϊ��s��
	XMFLOAT4X4 m_mTex;						// �e�N�X�`���ϊ��s��
	float m_fColor;

	HRESULT MakeVertex(ID3D11Device* pDevice);
	void SetVertex(void);
private:

};
