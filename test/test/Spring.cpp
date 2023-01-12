#include "Spring.h"
#include "Input.h"
#include "Sound.h"

#define SPRING_COEFFICIENT		1.0f			// バネ係数
#define SPRING_MASS				2.0f			// バネの質量

#define ATT_COEFFICIENT			0.2f			// 減衰係数(Attenuation)
#define SPRING_DEFAULT_LENGTH	2.0f			// バネの自然長


struct SPRING {
	int		status;				// ステータス
	bool	onLock;				// 固定フラグ
	bool	onDrag;				// マウスドラッグ中判定フラグ
	XMFLOAT3 position;			// 位置座標 (Position)
	XMFLOAT3 oldposition;			// 位置座標 (Position)
	XMFLOAT3 velocity;			// 速度 (Velocity)
	XMFLOAT3 acceleration;		// 加速度 (Acceleration)
	XMFLOAT3 resultant;			// 合力 (Resultant)
	XMFLOAT3 gravity;			// 重力 (Gravity)
	XMFLOAT3 tension_up;			// 張力 (Tension) 上方向
	XMFLOAT3 tension_down;		// 張力 (Tension) 下方向
};


static SPRING g_Spring;

void CSpring::Init()
{
	CPolygon::Init();

	SetTexture("data/Texture/Title001.png");

	g_Spring.oldposition = XMFLOAT3(275.0f, 150.0f, 0.0f);
	SetPos(0.0f, 0.0f);
	SetSize(90.0f, 90.0f);
	SetColor(1.0f, 0.0f, 0.0f);
	g_Spring.onDrag = false;
}

void CSpring::Update()
{
	float	length;					// バネの長さ
	XMFLOAT3 vec_up;					// 上側の力
	XMFLOAT3 vec_down;				// 下側の力
	XMFLOAT3 resistance;				// 抵抗力
	float	now_tension;			// 弾性力の大きさ

	POINT* mouse = CInput::GetMousePosition();

	if (CInput::GetMouseButton(MOUSEBUTTON_L) && CollisionToPoint(XMFLOAT2(mouse->x,mouse->y)) || g_Spring.onDrag)
	{
		g_Spring.onDrag = true;
		//g_Spring.position.x = 0.0f;
		//g_Spring.position.y = 0.0f;
		g_Spring.position.x = mouse->x;
		g_Spring.position.y = mouse->y;
	}
	if (CInput::GetMouseRelease(MOUSEBUTTON_L) && g_Spring.onDrag)
	{
		CSound::Play(SE_SHOT);
	}
	if(!CInput::GetMouseButton(MOUSEBUTTON_L))
	{
		g_Spring.onDrag = false;
	}


	// ２：下側に引っ張られる力を求める
	vec_down.x =g_Spring.oldposition.x - g_Spring.position.x;
	vec_down.y = g_Spring.oldposition.y - g_Spring.position.y;
	length = sqrt(vec_down.x * vec_down.x + vec_down.y * vec_down.y);
	if (length == 0)
	{
		g_Spring.tension_down.x = 0.0f;
		g_Spring.tension_down.y = 0.0f;
	}
	else
	{
		now_tension = SPRING_COEFFICIENT * (length - SPRING_DEFAULT_LENGTH);
		g_Spring.tension_down.x = vec_down.x * (now_tension / length);
		g_Spring.tension_down.y = vec_down.y * (now_tension / length);
	}

	// ３：重力を求める
	g_Spring.gravity.x = 0.0f;
	g_Spring.gravity.y = SPRING_MASS * -0.98f;

	// ４：抵抗力を求める
	resistance.x = -g_Spring.velocity.x * ATT_COEFFICIENT;
	resistance.y = -g_Spring.velocity.y * ATT_COEFFICIENT;

	// ５：１から４で求めた力を合成して合力を求める
	g_Spring.resultant.x = g_Spring.gravity.x + g_Spring.tension_down.x + resistance.x;
	g_Spring.resultant.y = g_Spring.gravity.y + g_Spring.tension_down.y + resistance.y;

	if (g_Spring.onDrag == false)
	{
		// ６：合力と質量から加速度を求める
		g_Spring.acceleration.x = g_Spring.resultant.x / SPRING_MASS;
		g_Spring.acceleration.y = g_Spring.resultant.y / SPRING_MASS;

		// ７：加速度から速度を求める
		g_Spring.velocity.x += g_Spring.acceleration.x;
		g_Spring.velocity.y += g_Spring.acceleration.y;

		// ８：速度から位置を求める
		g_Spring.position.x += g_Spring.velocity.x;
		g_Spring.position.y += g_Spring.velocity.y;

	}
}

void CSpring::Draw()
{
	m_vPos = g_Spring.position;
	CPolygon::Draw();
}