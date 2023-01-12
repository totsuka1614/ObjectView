#pragma once

#include "main.h"
#include "BackBuffer.h"
#include "ConstantBuffer.h"
#include <vector>
class Grid
{
	struct Vertex
	{
		float pos[3];
		float color[3];
	};

public:
	Grid() : m_InputLayout(nullptr),m_VertexBuffer(nullptr),m_bDraw(false){};
	~Grid() {};

	void Init(void);
	void Update(void);
	void Draw(void);
private:
	std::vector<Vertex> m_vertex;
	ID3D11InputLayout* m_InputLayout;
	ID3D11Buffer* m_VertexBuffer;
	ConstantBuffer *m_pConstantBuffer[2];
	MATERIAL m_Material;

	bool m_bDraw;
};