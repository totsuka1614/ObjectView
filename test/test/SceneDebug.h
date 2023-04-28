/******************************************************************************
* 
* @file      SceneDebug.h
* @brief     シーンデバッグクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//インクルード部
#include "main.h"
#include "SceneBase.h"
#include "grid.h"
#include <list>
#include <string>
#include "Mesh.h"
#include "SplitCamera.h"

class CDebug : public SceneBase
{
public:
	//コンストラクタ
	CDebug() {}
	//デストラクタ
	~CDebug() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void DrawObj();
	void Draw2D();
	void DrawDepthShadow();
	void DrawShadow(ID3D11ShaderResourceView*);
	void SplitDraw(ID3D11ShaderResourceView* tex = NULL);
	std::list<std::string>& GetNameList() { return m_NameList; }
private:
	std::list<std::string> m_NameList;
	bool m_bSplit;
	CCameraSplit m_split;
};