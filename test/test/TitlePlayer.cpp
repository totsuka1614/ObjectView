#include "TitlePlayer.h"

//----- �\����
struct BALL {
	int		status;				// �X�e�[�^�X
	float	position;			// �ψ�
	float	velocity;			// ���x (Velocity)
	float	acceleration;		// �����x (Acceleration)
	float	gravity;			// �d�� (Gravity)
	float	tension;			// �e���� (Tension)
};

BALL		g_Ball;						// �{�[���{��

#define SPRING_DEFAULT_LENGTH	150.0f	// �΂˂̎��R��
#define SPRING_COEFFICIENT		1.0f	// �΂ˌW��
#define BALL_MASS				150.0f	// �{�[���̎���


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
	case 0:		// ������
		g_Ball.position = SPRING_DEFAULT_LENGTH + 100.0f;		// �ψ�
		g_Ball.velocity = 0.0f;									// ���x
		g_Ball.acceleration = 0.0f;									// �����x
		g_Ball.gravity = BALL_MASS * 0.98f;					// �d��
		g_Ball.tension = 0.0f;									// �e����
		g_Ball.status = 1;
		break;
	case 1:		// ���C������
		// �e���͂����߂�
		g_Ball.tension = -SPRING_COEFFICIENT * (g_Ball.position - SPRING_DEFAULT_LENGTH);

		// ���͂Ǝ��ʂ�������x�����߂�
		g_Ball.acceleration = (g_Ball.gravity + g_Ball.tension) / BALL_MASS;

		// ���x�ɉ����x��������
		g_Ball.velocity += g_Ball.acceleration;

		// ���x������W���ړ�
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