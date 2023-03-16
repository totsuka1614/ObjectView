/******************************************************************************
* 
* @file      CollisionList.cpp
* @brief     �����蔻�胊�X�g
* @author    Totsuka Kensuke
* @date      2023/03/02
* @note      �����蔻������I�u�W�F�N�g�����[����N���X
* @attention 
* 
******************************************************************************/
#include "CollisionList.h"
#include <math.h>

ColList g_ColList;	//�C���X�^���X

ColList* ColList::m_pCol = &g_ColList;			// ����

/******************************************************************************
* 
* @brief      CollisionAABB
* @param[in]  first
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �ʏ�̓����蔻��(���X�g�̑S�Ăɑ΂���)
* @attention  
******************************************************************************/
bool ColList::CollisionAABB(ObjectBase* first)
{
	// ���S���W�����߂�
	XMFLOAT3 vA, vB;
	//�����蔻����Ƃ�I�u�W�F�N�g�̃}�g���b�N�X�擾
	XMFLOAT4X4 one; XMStoreFloat4x4(&one, first->GetWorldMatrix());

	// ���W�v�Z
	vA = XMFLOAT3(one._41, one._42, one._43);
	XMFLOAT3 vAr;
	vAr.x = sqrt(one._11 * one._11 + one._21 * one._21 + one._31 * one._31);
	vAr.y = sqrt(one._12 * one._12 + one._22 * one._22 + one._32 * one._32);
	vAr.z = sqrt(one._13 * one._13 + one._23 * one._23 + one._33 * one._33);
	
	//�Փ˔���
	for (auto second : m_pObj)		//���X�g���J��Ԃ�
	{
		//���X�g�Ɠ����I�u�W�F�N�g�������ꍇ
		if (second == first)
			continue;
		//�R���W�������Ȃ������ꍇ
		if (second->GetColFlag() || first->GetColFlag())
			continue;

		//�����蔻����Ƃ�I�u�W�F�N�g�̃}�g���b�N�X�擾
		XMFLOAT4X4 two; XMStoreFloat4x4(&two, second->GetWorldMatrix());
		vB = XMFLOAT3(two._41,two._42,two._43);
		
		//���W�v�Z
		XMFLOAT3 vBr;
		vBr.x = sqrt(two._11 * two._11 + two._21 * two._21 + two._31 * two._31);
		vBr.y = sqrt(two._12 * two._12 + two._22 * two._22 + two._32 * two._32);
		vBr.z = sqrt(two._13 * two._13 + two._23 * two._23 + two._33 * two._33);

		//�������Ă��邩�H
		if (vA.x - vAr.x <= vB.x + vBr.x &&
			vB.x - vBr.x <= vA.x + vAr.x &&
			vA.y - vAr.y <= vB.y + vBr.y &&
			vB.y - vBr.y <= vA.y + vAr.y &&
			vA.z - vAr.z <= vB.z + vBr.z &&
			vB.z - vBr.z <= vA.z + vAr.z)
			return true;
	}
	return false;
}

/******************************************************************************
* 
* @brief      CollisionAABB
* @param[in]  first
* @param[in]  tag
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       �^�O���������Ĉ�v������̂Ɠ����蔻����s��
* @attention  
******************************************************************************/
bool ColList::CollisionAABB(ObjectBase* first, TAG tag)
{
	// ���S���W�����߂�
	XMFLOAT3 vA, vB;
	//�����蔻����Ƃ�I�u�W�F�N�g�̃}�g���b�N�X�擾
	XMFLOAT4X4 one; XMStoreFloat4x4(&one, first->GetWorldMatrix());

	// ���W�v�Z
	vA = XMFLOAT3(one._41, one._42, one._43);
	XMFLOAT3 vAr;
	vAr.x = sqrt(one._11 * one._11 + one._21 * one._21 + one._31 * one._31);
	vAr.y = sqrt(one._12 * one._12 + one._22 * one._22 + one._32 * one._32);
	vAr.z = sqrt(one._13 * one._13 + one._23 * one._23 + one._33 * one._33);

	//�Փ˔���
	for (auto second : m_pObj)		//���X�g���J��Ԃ�
	{
		//���X�g�Ɠ����I�u�W�F�N�g�������ꍇ
		if (second == first)
			continue;
		//�R���W�������Ȃ������ꍇ
		if (second->GetColFlag() || first->GetColFlag())
			continue;
		//�R���W�����^�O�������ƈ�v���Ȃ��ꍇ
		if (second->GetTag() != tag)
			continue;

		//�����蔻����Ƃ�I�u�W�F�N�g�̃}�g���b�N�X�擾
		XMFLOAT4X4 two; XMStoreFloat4x4(&two, second->GetWorldMatrix());
		vB = XMFLOAT3(two._41, two._42, two._43);

		//���W�v�Z
		XMFLOAT3 vBr;
		vBr.x = sqrt(two._11 * two._11 + two._21 * two._21 + two._31 * two._31);
		vBr.y = sqrt(two._12 * two._12 + two._22 * two._22 + two._32 * two._32);
		vBr.z = sqrt(two._13 * two._13 + two._23 * two._23 + two._33 * two._33);

		//�������Ă��邩�H
		if (vA.x - vAr.x <= vB.x + vBr.x &&
			vB.x - vBr.x <= vA.x + vAr.x &&
			vA.y - vAr.y <= vB.y + vBr.y &&
			vB.y - vBr.y <= vA.y + vAr.y &&
			vA.z - vAr.z <= vB.z + vBr.z &&
			vB.z - vBr.z <= vA.z + vAr.z)
			return true;
	}
	return false;
}

/******************************************************************************
* 
* @brief      CollisionOBB
* @param[in]  first
* @return     bool
* @author     Totsuka Kensuke
* @date       2023/03/02
* @note       ���G�Ȍ`�̓����蔻����s��
* @attention  ���ɏd���̂ő����g���̂͊댯
******************************************************************************/
bool ColList::CollisionOBB(ObjectBase* first)
{
	XMFLOAT3 pos1 = first->GetTransform();
	XMFLOAT4X4 mW1;XMStoreFloat4x4(&mW1,first->GetWorldMatrix());
	// ���S���W�擾
	XMFLOAT3 vP1;
	XMStoreFloat3(&vP1,
		XMVector3TransformCoord(XMLoadFloat3(&pos1), XMLoadFloat4x4(&mW1)));
	XMFLOAT3 vBB1;
	vBB1.x = sqrt(mW1._11 * mW1._11 + mW1._21 * mW1._21 + mW1._31 * mW1._31);
	vBB1.y = sqrt(mW1._12 * mW1._12 + mW1._22 * mW1._22 + mW1._32 * mW1._32);
	vBB1.z = sqrt(mW1._13 * mW1._13 + mW1._23 * mW1._23 + mW1._33 * mW1._33);

	for(auto second : m_pObj)
	{
		if (second == first || second->GetName() == "Player")
			continue;

		if (!second->GetEnable() || !first->GetEnable())
			continue;
	
		XMFLOAT3 pos2 = second->GetTransform();
		XMFLOAT4X4 mW2; XMStoreFloat4x4(&mW2, second->GetWorldMatrix());
		// ���S���W�擾
		XMFLOAT3 vP2;
		XMStoreFloat3(&vP2,
			XMVector3TransformCoord(XMLoadFloat3(&pos2), XMLoadFloat4x4(&mW2)));

		// ���S���W�Ԃ̃x�N�g�������߂�
		XMVECTOR vD;
		vD = XMVectorSet(vP2.x - vP1.x, vP2.y - vP1.y, vP2.z - vP1.z, 0.0f);
		// ���f�����W�������߂�
		XMVECTOR vN[6];
		vN[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
		vN[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
		vN[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);
		vN[3] = XMVectorSet(mW2._11, mW2._12, mW2._13, 0.0f);
		vN[4] = XMVectorSet(mW2._21, mW2._22, mW2._23, 0.0f);
		vN[5] = XMVectorSet(mW2._31, mW2._32, mW2._33, 0.0f);
		// OBB�̑傫��(����)���|�����x�N�g�������߂�
		XMFLOAT3 vBB2;
		vBB2.x = sqrt(mW2._11 * mW2._11 + mW2._21 * mW2._21 + mW2._31 * mW2._31);
		vBB2.y = sqrt(mW2._12 * mW2._12 + mW2._22 * mW2._22 + mW2._32 * mW2._32);
		vBB2.z = sqrt(mW2._13 * mW2._13 + mW2._23 * mW2._23 + mW2._33 * mW2._33);
		XMVECTOR vL[6];
		vL[0] = XMVectorSet(mW1._11 * vBB1.x, mW1._12 * vBB1.x, mW1._13 * vBB1.x, 0.0f);
		vL[1] = XMVectorSet(mW1._21 * vBB1.y, mW1._22 * vBB1.y, mW1._23 * vBB1.y, 0.0f);
		vL[2] = XMVectorSet(mW1._31 * vBB1.z, mW1._32 * vBB1.z, mW1._33 * vBB1.z, 0.0f);
		vL[3] = XMVectorSet(mW2._11 * vBB2.x, mW2._12 * vBB2.x, mW2._13 * vBB2.x, 0.0f);
		vL[4] = XMVectorSet(mW2._21 * vBB2.y, mW2._22 * vBB2.y, mW2._23 * vBB2.y, 0.0f);
		vL[5] = XMVectorSet(mW2._31 * vBB2.z, mW2._32 * vBB2.z, mW2._33 * vBB2.z, 0.0f);

		// ��������₪�ʂ̖@���x�N�g��(���f�����W��)
		float fL, f, fD;
		for (int i = 0; i < 6; ++i) {
			XMVECTOR& vS = vN[i];//���������
			// OBB�̉e(�̔���)�̍��v
			fL = 0.0f;
			for (int j = 0; j < 6; ++j) {
				XMStoreFloat(&f, XMVector3Dot(vS, vL[j]));
				fL += fabsf(f);
			}
			// ���S���W�Ԃ̋����Ɣ�r
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD) return false;// �������Ă��Ȃ�
		}
		// ��������₪���ꂼ��̕ӂɐ����ȃx�N�g��
		XMVECTOR vS;
		for (int i = 0; i < 3; ++i) {
			for (int j = 3; j < 6; ++j) {
				// �������������߂�
				vS = XMVector3Normalize(XMVector3Cross(vN[i], vN[j]));
				// OBB�̉e(�̔���)�̍��v
				fL = 0.0f;
				for (int k = 0; k < 6; ++k) {
					XMStoreFloat(&f, XMVector3Dot(vS, vL[k]));
					fL += fabsf(f);
				}
				// ���S���W�Ԃ̋����Ɣ�r
				XMStoreFloat(&fD, XMVector3Dot(vS, vD));
				fD = fabsf(fD);
				if (fL < fD) return false;// �������Ă��Ȃ�
			}
		}
		return true;// �������Ă���

	}

		return false;
}
