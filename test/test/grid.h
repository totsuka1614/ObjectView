/******************************************************************************
* 
* @file      grid.h
* @brief     �O���b�h�N���X
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#pragma once
//�C���N���[�h��
#include "main.h"
#include "BackBuffer.h"
#include "ConstantBuffer.h"
#include <vector>
class CGrid
{
	//���_���
	struct Vertex
	{
		float pos[3];
		float color[3];
	};

public:
	CGrid() : m_InputLayout(nullptr),m_VertexBuffer(nullptr),m_bDraw(false){};
	~CGrid() {};

	void Init(void);
	void Update(void);
	void Draw(void);
private:
	std::vector<Vertex> m_vertex;
	ID3D11InputLayout* m_InputLayout;
	ID3D11Buffer* m_VertexBuffer;
	CConstantBuffer *m_pConstantBuffer[2];
	MATERIAL m_Material;

	bool m_bDraw;
};