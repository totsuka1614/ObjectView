/******************************************************************************
* 
* @file      Spring.cpp
* @brief     ばねクラス
* @author    Totsuka Kensuke
* @date      2023/04/20
* @note      
* @attention 
* 
******************************************************************************/
#include "Spring.h"
#include "Input.h"
#include "Sound.h"

#define SPRING_COEFFICIENT		1.0f			// バネ係数
#define SPRING_MASS				2.0f			// バネの質量
#define ATT_COEFFICIENT			0.2f			// 減衰係数(Attenuation)
#define SPRING_DEFAULT_LENGTH	2.0f			// バネの自然長

void CSpring::Init()
{
	CPolygon::Init();

	SetTexture("data/Texture/Title001.png");

	m_Spring.oldposition = XMFLOAT3(275.0f, 150.0f, 0.0f);
	SetPos(0.0f, 0.0f);
	SetSize(90.0f, 90.0f);
	SetColor(1.0f, 0.0f, 0.0f);
	m_Spring.onDrag = false;
}

void CSpring::Update()
{
	float	length;					// バネの長さ
	XMFLOAT3 vec_up;					// 上側の力
	XMFLOAT3 vec_down;				// 下側の力
	XMFLOAT3 resistance;				// 抵抗力
	float	now_tension;			// 弾性力の大きさ

	POINT* mouse = CInput::GetMousePosition();

	if (CInput::GetMouseButton(MOUSEBUTTON_L) && CollisionToPoint(XMFLOAT2(mouse->x,mouse->y)) || m_Spring.onDrag)
	{
		m_Spring.onDrag = true;
		//m_Spring.position.x = 0.0f;
		//m_Spring.position.y = 0.0f;
		m_Spring.position.x = mouse->x;
		m_Spring.position.y = mouse->y;
	}
	if (CInput::GetMouseRelease(MOUSEBUTTON_L) && m_Spring.onDrag)
	{
		CSound::Play(SE_SHOT);
	}
	if(!CInput::GetMouseButton(MOUSEBUTTON_L))
	{
		m_Spring.onDrag = false;
	}


	// ２：下側に引っ張られる力を求める
	vec_down.x =m_Spring.oldposition.x - m_Spring.position.x;
	vec_down.y = m_Spring.oldposition.y - m_Spring.position.y;
	length = sqrt(vec_down.x * vec_down.x + vec_down.y * vec_down.y);
	if (length == 0)
	{
		m_Spring.tension_down.x = 0.0f;
		m_Spring.tension_down.y = 0.0f;
	}
	else
	{
		now_tension = SPRING_COEFFICIENT * (length - SPRING_DEFAULT_LENGTH);
		m_Spring.tension_down.x = vec_down.x * (now_tension / length);
		m_Spring.tension_down.y = vec_down.y * (now_tension / length);
	}

	// ３：重力を求める
	m_Spring.gravity.x = 0.0f;
	m_Spring.gravity.y = SPRING_MASS * -0.98f;

	// ４：抵抗力を求める
	resistance.x = -m_Spring.velocity.x * ATT_COEFFICIENT;
	resistance.y = -m_Spring.velocity.y * ATT_COEFFICIENT;

	// ５：１から４で求めた力を合成して合力を求める
	m_Spring.resultant.x = m_Spring.gravity.x + m_Spring.tension_down.x + resistance.x;
	m_Spring.resultant.y = m_Spring.gravity.y + m_Spring.tension_down.y + resistance.y;

	if (m_Spring.onDrag == false)
	{
		// ６：合力と質量から加速度を求める
		m_Spring.acceleration.x = m_Spring.resultant.x / SPRING_MASS;
		m_Spring.acceleration.y = m_Spring.resultant.y / SPRING_MASS;

		// ７：加速度から速度を求める
		m_Spring.velocity.x += m_Spring.acceleration.x;
		m_Spring.velocity.y += m_Spring.acceleration.y;

		// ８：速度から位置を求める
		m_Spring.position.x += m_Spring.velocity.x;
		m_Spring.position.y += m_Spring.velocity.y;

	}
}

void CSpring::Draw()
{
	m_vPos = m_Spring.position;
	CPolygon::Draw();
}