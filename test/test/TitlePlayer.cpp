#include "TitlePlayer.h"

//----- 構造体
struct BALL {
	int		status;				// ステータス
	float	position;			// 変位
	float	velocity;			// 速度 (Velocity)
	float	acceleration;		// 加速度 (Acceleration)
	float	gravity;			// 重力 (Gravity)
	float	tension;			// 弾性力 (Tension)
};

BALL		g_Ball;						// ボール本体

#define SPRING_DEFAULT_LENGTH	150.0f	// ばねの自然長
#define SPRING_COEFFICIENT		1.0f	// ばね係数
#define BALL_MASS				150.0f	// ボールの質量


void TitlePlayer::Init(void)
{
	FBXFile::Load("data/model/airplane.fbx");

	m_vScale = XMFLOAT3(10.0f, 10.0f, 10.0f);
	m_Material.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.8f,1.0f);
	m_PStype = PHONG;
	m_VStype = VERTEX;

}

void TitlePlayer::Update(void)
{
	switch (g_Ball.status) {
	case 0:		// 初期化
		g_Ball.position = SPRING_DEFAULT_LENGTH + 100.0f;		// 変位
		g_Ball.velocity = 0.0f;									// 速度
		g_Ball.acceleration = 0.0f;									// 加速度
		g_Ball.gravity = BALL_MASS * 0.98f;					// 重力
		g_Ball.tension = 0.0f;									// 弾性力
		g_Ball.status = 1;
		break;
	case 1:		// メイン処理
		// 弾性力を求める
		g_Ball.tension = -SPRING_COEFFICIENT * (g_Ball.position - SPRING_DEFAULT_LENGTH);

		// 合力と質量から加速度を求める
		g_Ball.acceleration = (g_Ball.gravity + g_Ball.tension) / BALL_MASS;

		// 速度に加速度を加える
		g_Ball.velocity += g_Ball.acceleration;

		// 速度から座標を移動
		g_Ball.position += g_Ball.velocity;
		break;
	}

	m_vDegree.y += 1.0f;

	ObjectBase::Update();
	Model::Update();
}

void TitlePlayer::Draw(void)
{
	m_vPos.y = g_Ball.position;
	
	BACKBUFFER->SetBlendState();
	BACKBUFFER->SetCullMode();
	FBXFile::Draw();
	BACKBUFFER->SetCullMode();
	BACKBUFFER->SetBlendState(BS_ALPHABLEND);
}