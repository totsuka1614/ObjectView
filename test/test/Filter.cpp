/******************************************************************************
* 
* @file      Filter.cpp
* @brief     ��ʃt�B���^
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      
* @attention 
* 
******************************************************************************/
#include "Filter.h"
#include "Input.h"
#include <algorithm>

#define MAX_FILTER (5)

/******************************************************************************
* 
* @brief      Update
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �X�V
* @attention  
******************************************************************************/
void CFilter::Update()
{
	//60f���ƂɃ��Z�b�g
	if (m_fTime >= 60.0f)
		m_fTime = 0.0f;

	m_fTime++;

	//CONTROL + T �ŉ�ʃt�B���^�ύX
	if (CInput::GetKeyPress(VK_CONTROL))
	{
		if (CInput::GetKeyTrigger(VK_T))
		{
			m_nNumber++;
			//�}�b�N�X�܂ōs������O��
			if (m_nNumber >= MAX_FILTER)
				m_nNumber = 0;
		}
	}

	//�t�B���^���Ƃɏ���
	switch (m_nNumber)
	{
	case 0:	//�ʏ�
		m_eType = PIXEL2D;
		break;
	case 1: //���m�N��
		m_eType = MONOCHROME_EFFECT;
		break;
	case 2:	//�V���v���m�C�Y
		m_eType = NOIZ_EFFECT;
		break;
	case 3:	//�r�l�b�g(���L�[�Œ���)
		m_eType = VIGNETTE_EFFECT;
		if (CInput::GetKeyPress(VK_LEFT))
			m_fVignette -= 0.05f;
		if (CInput::GetKeyPress(VK_RIGHT))
			m_fVignette += 0.05f;
		m_fVignette = min(m_fVignette, 1.0f);
		m_fVignette = max(m_fVignette, 0.0f);

		break;
	case 4:	//�O���b�`�m�C�Y
		m_eType = GLITCH_EFFECT;
		break;
	default:
		break;
	}
}

/******************************************************************************
* 
* @brief      Draw
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �`��
* @attention  
******************************************************************************/
void CFilter::Draw()
{
	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* pDeviceContext = BACKBUFFER->GetDeviceContext();

	//�V�F�[�_�Z�b�g
	BACKBUFFER->SetUpContext(VERTEX2D, m_eType, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �g�k
	XMMATRIX mWorld = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	// ��]
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_vDegree.x),
		XMConvertToRadians(m_vDegree.y), XMConvertToRadians(m_vDegree.z));
	// �ړ�
	mWorld *= XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	// ���[���h �}�g���b�N�X�ɐݒ�
	XMStoreFloat4x4(&m_mWorld, mWorld);

	if (m_pTexture) {
		// �g�k
		mWorld = XMMatrixScaling(m_vSizeTexFrame.x, m_vSizeTexFrame.y, 1.0f);
		// �ړ�
		mWorld *= XMMatrixTranslation(m_vPosTexFrame.x, m_vPosTexFrame.y, 0.0f);
		// �e�N�X�`�� �}�g���b�N�X�ɐݒ�
		XMStoreFloat4x4(&m_mTex, mWorld);
	}
	else {
		// �e�N�X�`������
		m_mTex._44 = 0.0f;
	}

	// ���_�o�b�t�@�X�V
	SetVertex();

	pDeviceContext->IASetInputLayout(m_pInputLayout);

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//�t�B���^�[�Z�b�g
	BACKBUFFER->SetTexture(m_pTexture);

	//�萔�o�b�t�@�ݒ�
	SHADER_GLOBAL cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&m_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&m_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&m_mTex));
	cb.bColor = m_fColor;
	cb.rand = rand() % 100 + 50;		//�����_��
	cb.vignette = m_fVignette;			//�r�l�b�g
	cb.VertGlitchPase  = rand() % 100 *0.01f;	//�e�X�g��
	cb.HorzGlitchPase  = rand() % 100 *0.01f;	//�e�X�g��
	cb.GlitchStepValue = m_fVignette;			//�e�X�g��
	cb.time = m_fTime;							//���݂̃t���[��
	
	//�萔�o�b�t�@���X�V
	m_pConstantBuffer->Update(&cb);
	m_pConstantBuffer->SetVertexShader();
	m_pConstantBuffer->SetPixelShader();

	// �|���S���̕`��
	pDeviceContext->Draw(NUM_VERTEX, 0);
}