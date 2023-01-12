#include "Trail.h"
#include "Input.h"
#include "BackBuffer.h"

#define TRAIL_MAX		100					// パーティクルの数


// 変数
typedef struct {
	XMFLOAT2 size;
	int		status;		// 状態
	float	px, py;		// 位置
	float	vx, vy;		// 速度
	float	ax, ay;		// 加速度
	int life;
} TRAIL;

TRAIL	g_Trail[TRAIL_MAX];			// パーティクル本体

void CTrail::Init()
{
	for (int i = 0; i < TRAIL_MAX; i++) {
		g_Trail[i].status = 0;	// ステータスを初期化
		g_Trail[i].life = 0;	// ステータスを初期化
		g_Trail[i].size = XMFLOAT2(9.0f, 9.0f);	// ステータスを初期化
	}

	CPolygon::Init();

	SetTexture("data/Texture/Particle002.png");

	SetPos(0.0f, 0.0f);
	SetSize(9.0f, 9.0f);
	SetColor(1.0f, 1.0f, 1.0f);

	m_nInterval = 0;

	m_OldMouse = *CInput::GetMousePosition();
}

void CTrail::Update() 
{
	
	m_nInterval++;
	


	POINT* mouse = CInput::GetMousePosition();

	//mouse->x -= SCREEN_CENTER_X;
	//mouse->y += SCREEN_CENTER_Y;
	XMFLOAT2 pos = XMFLOAT2(mouse->x - m_OldMouse.x, mouse->y - m_OldMouse.y);

	for (int i = 0; i < TRAIL_MAX; i++) {
		switch (g_Trail[i].status) {
		case 0:					// 待機状態
			if (m_nInterval >= 1 && (pos.x != 0.0f || pos.y != 0.0f)) {
				g_Trail[i].status = 1;
				m_nInterval = 0;
			}
			break;
		case 1:
			g_Trail[i].px = (float)mouse->x;
			g_Trail[i].py = (float)mouse->y;
			g_Trail[i].vx = 0.0f;
			g_Trail[i].vy = 0.0f;
			g_Trail[i].ax = 0.0f;
			g_Trail[i].ay = 0.0f;
			g_Trail[i].status = 2;
			// THRU
		case 2:

			g_Trail[i].vx += g_Trail[i].ax;
			g_Trail[i].vy += g_Trail[i].ay;

			g_Trail[i].px += g_Trail[i].vx;
			g_Trail[i].py += g_Trail[i].vy;

			g_Trail[i].life++;

			if (g_Trail[i].life >= 10) {
				g_Trail[i].size.x -= 0.5f;
				g_Trail[i].size.y -= 0.5f;
				if (g_Trail[i].size.x <= 0.0f)
				{
					g_Trail[i].status = 0;
					g_Trail[i].life = 0;
					g_Trail[i].size.x = 9.0f;
					g_Trail[i].size.y = 9.0f;
				}
			}
			break;
		}
	}

	m_OldMouse = *CInput::GetMousePosition();
}

void CTrail::Draw()
{
	BACKBUFFER->SetBlendState(BS_ALPHABLEND);
	for (int i = 0; i < TRAIL_MAX; i++) {
		if (g_Trail[i].status == 2) {
			m_vPos.x = g_Trail[i].px;
			m_vPos.y = g_Trail[i].py;
			m_vScale.x = g_Trail[i].size.x;
			m_vScale.y = g_Trail[i].size.y;
			CPolygon::Draw();
		}
	}
}