//=============================================================================
//
// �J���� �N���X [Camera.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"

enum SplitCamera
{
	NOW_VIEW,
	SIDE_VIEW,
	FRONT_VIEW,
	UP_VIEW,

	MAX_VIEW
};

class CCamera
{
protected:
	DirectX::XMFLOAT3 m_vPos;		// ���_
	DirectX::XMFLOAT3 m_vTarget;	// �����_
	DirectX::XMFLOAT3 m_vUp;		// ����x�N�g��

	DirectX::XMFLOAT3 m_vAngle;		// �J�����̊p�x
	float m_fLengthInterval;		// �J�����̎��_�ƒ����_�̋���

private:

	void LoadFile();
	void SaveFile();

	char m_cName[256];

	float m_fAspectRatio;			// �c����
	float m_fFovY;					// ����p(Degree)
	float m_fNearZ;					// �O���N���b�v����
	float m_fFarZ;					// ����N���b�v����

	float m_fMoveX;
	float m_fMoveY;
	float m_fFogStart;
	float m_fFogRange;

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ���[���h �}�g���b�N�X
	DirectX::XMFLOAT4X4 m_mtxView;	// �r���[ �}�g���b�N�X
	DirectX::XMFLOAT4X4 m_mtxProj;	// �v���W�F�N�V���� �}�g���b�N�X

	static CCamera* m_pCamera;		// ���݂̃J����

public:
	CCamera();

	virtual void Init();
	virtual void Init(XMFLOAT3,XMFLOAT3);
	virtual void Uninit();
	virtual void Update();
	virtual void Clear();

	void SetTransform(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetTransform() { return m_vPos; }
	DirectX::XMFLOAT3& GetTarget() { return m_vTarget; }
	void SetTransform(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	DirectX::XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	DirectX::XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY) { m_fFovY = fFovY; }
	void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
	void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }
	void UpdateMatrix();
	DirectX::XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	DirectX::XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	DirectX::XMFLOAT3& GetAngle() { return m_vAngle; }
	float& GetStart() { return m_fFogStart; }
	float& GetRange() { return m_fFogRange; }

	static CCamera* Get() { return m_pCamera; }
	static void Set(CCamera* pCamera = nullptr);
};

