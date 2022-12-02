#include "CollisionList.h"
#include <math.h>
ColList g_ColList;

ColList* ColList::m_pCol = &g_ColList;			// 現在

bool ColList::CollisionAABB(ObjectBase* first)
{
	// 中心座標を求める
	XMFLOAT3 vA, vB;
	/*XMStoreFloat3(&vA,
		XMVector3TransformCoord(
			XMLoadFloat3(&first->GetTransform()),
			first->GetWorldMatrix()));*/
	XMFLOAT4X4 a; XMStoreFloat4x4(&a, first->GetWorldMatrix());

	vA = XMFLOAT3(a._41, a._42, a._43);

	// 衝突判定
	XMFLOAT3 vAr;
	vAr.x = sqrt(a._11 * a._11 + a._21 * a._21 + a._31 * a._31);
	vAr.y = sqrt(a._12 * a._12 + a._22 * a._22 + a._32 * a._32);
	vAr.z = sqrt(a._13 * a._13 + a._23 * a._23 + a._33 * a._33);

	for (auto second : m_pObj)
	{
		if (second == first)
			continue;

		if (second->GetColFlag() || first->GetColFlag())
			continue;

		/*XMStoreFloat3(&vB,
			XMVector3TransformCoord(
				XMLoadFloat3(&second->GetTransform()),
				second->GetWorldMatrix()));*/
		XMFLOAT4X4 b; XMStoreFloat4x4(&b, second->GetWorldMatrix());
		vB = XMFLOAT3(b._41,b._42,b._43);

		XMFLOAT3 vBr;
		vBr.x = sqrt(b._11 * b._11 + b._21 * b._21 + b._31 * b._31);
		vBr.y = sqrt(b._12 * b._12 + b._22 * b._22 + b._32 * b._32);
		vBr.z = sqrt(b._13 * b._13 + b._23 * b._23 + b._33 * b._33);


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

bool ColList::CollisionOBB(ObjectBase* first)
{
	XMFLOAT3 pos1 = first->GetTransform();
	XMFLOAT4X4 mW1;XMStoreFloat4x4(&mW1,first->GetWorldMatrix());
	// 中心座標取得
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
		// 中心座標取得
		XMFLOAT3 vP2;
		XMStoreFloat3(&vP2,
			XMVector3TransformCoord(XMLoadFloat3(&pos2), XMLoadFloat4x4(&mW2)));

		// 中心座標間のベクトルを求める
		XMVECTOR vD;
		vD = XMVectorSet(vP2.x - vP1.x, vP2.y - vP1.y, vP2.z - vP1.z, 0.0f);
		// モデル座標軸を求める
		XMVECTOR vN[6];
		vN[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
		vN[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
		vN[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);
		vN[3] = XMVectorSet(mW2._11, mW2._12, mW2._13, 0.0f);
		vN[4] = XMVectorSet(mW2._21, mW2._22, mW2._23, 0.0f);
		vN[5] = XMVectorSet(mW2._31, mW2._32, mW2._33, 0.0f);
		// OBBの大きさ(半分)を掛けたベクトルを求める
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

		// 分離軸候補が面の法線ベクトル(モデル座標軸)
		float fL, f, fD;
		for (int i = 0; i < 6; ++i) {
			XMVECTOR& vS = vN[i];//分離軸候補
			// OBBの影(の半分)の合計
			fL = 0.0f;
			for (int j = 0; j < 6; ++j) {
				XMStoreFloat(&f, XMVector3Dot(vS, vL[j]));
				fL += fabsf(f);
			}
			// 中心座標間の距離と比較
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD) return false;// 当たっていない
		}
		// 分離軸候補がそれぞれの辺に垂直なベクトル
		XMVECTOR vS;
		for (int i = 0; i < 3; ++i) {
			for (int j = 3; j < 6; ++j) {
				// 分離軸候補を求める
				vS = XMVector3Normalize(XMVector3Cross(vN[i], vN[j]));
				// OBBの影(の半分)の合計
				fL = 0.0f;
				for (int k = 0; k < 6; ++k) {
					XMStoreFloat(&f, XMVector3Dot(vS, vL[k]));
					fL += fabsf(f);
				}
				// 中心座標間の距離と比較
				XMStoreFloat(&fD, XMVector3Dot(vS, vD));
				fD = fabsf(fD);
				if (fL < fD) return false;// 当たっていない
			}
		}
		return true;// 当たっている

		/*if (pos1.x - size1.x / 2 < pos2.x && pos1.x + size1.x / 2 > pos2.x)
		{
			if (pos1.y - size1.y / 2 < pos2.y && pos1.y + size1.y / 2 > pos2.y)
			{
				if (pos1.z - size1.z / 2 < pos2.z && pos1.z + size1.z / 2 > pos2.z)
				{
					return true;
				}
			}
		}*/
	}

		return false;
}
