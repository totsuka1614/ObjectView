//=============================================================================
//
// カメラ クラス [Camera.h]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#pragma once
#include "main.h"
#include "fbx.h"
#include "Player.h"

class CCamera
{
protected:
	XMFLOAT3 m_vPos;		// 視点
	XMFLOAT3 m_vTarget;	// 注視点
	XMFLOAT3 m_vUp;		// 上方ベクトル

	XMFLOAT3 m_vAngle;		// カメラの角度
	float m_fLengthInterval;		// カメラの視点と注視点の距離

private:

	void LoadFile();
	void SaveFile();

	char m_cName[256];

	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	float m_fMoveX;
	float m_fMoveY;
	float m_fFogStart;
	float m_fFogRange;

	DirectX::XMFLOAT4X4 m_mtxWorld;	// ワールド マトリックス
	DirectX::XMFLOAT4X4 m_mtxView;	// ビュー マトリックス
	DirectX::XMFLOAT4X4 m_mtxProj;	// プロジェクション マトリックス

	static CCamera* m_pCamera;		// 現在のカメラ

	FBXFile* m_pSky;
	ObjectBase* m_pPlayer;
	bool m_bStart;
	XMFLOAT3 m_vOldPos;	// 視点
	XMFLOAT3 m_vOldTarget;	// 注視点
public:
	CCamera();

	virtual void Init();
	virtual void Init(XMFLOAT3,XMFLOAT3);
	virtual void Uninit();
	virtual void Update();
	virtual void Clear();
	virtual void Sky();

	void SetTransform(DirectX::XMFLOAT3& vPos) { m_vPos = vPos; }
	DirectX::XMFLOAT3& GetTransform() { return m_vPos; }
	DirectX::XMFLOAT3& GetTarget() { return m_vTarget; }
	void SetTransform(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(DirectX::XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(DirectX::XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	void SetWorldMatrix(DirectX::XMFLOAT4X4& mtxWorld);
	void SetPlayer(ObjectBase &player) { m_pPlayer = &player; }
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

