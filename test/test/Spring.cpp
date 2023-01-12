#include "Spring.h"
#include "Input.h"
#include "Sound.h"

#define SPRING_COEFFICIENT		1.0f			// �o�l�W��
#define SPRING_MASS				2.0f			// �o�l�̎���

#define ATT_COEFFICIENT			0.2f			// �����W��(Attenuation)
#define SPRING_DEFAULT_LENGTH	2.0f			// �o�l�̎��R��


struct SPRING {
	int		status;				// �X�e�[�^�X
	bool	onLock;				// �Œ�t���O
	bool	onDrag;				// �}�E�X�h���b�O������t���O
	XMFLOAT3 position;			// �ʒu���W (Position)
	XMFLOAT3 oldposition;			// �ʒu���W (Position)
	XMFLOAT3 velocity;			// ���x (Velocity)
	XMFLOAT3 acceleration;		// �����x (Acceleration)
	XMFLOAT3 resultant;			// ���� (Resultant)
	XMFLOAT3 gravity;			// �d�� (Gravity)
	XMFLOAT3 tension_up;			// ���� (Tension) �����
	XMFLOAT3 tension_down;		// ���� (Tension) ������
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
	float	length;					// �o�l�̒���
	XMFLOAT3 vec_up;					// �㑤�̗�
	XMFLOAT3 vec_down;				// �����̗�
	XMFLOAT3 resistance;				// ��R��
	float	now_tension;			// �e���͂̑傫��

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


	// �Q�F�����Ɉ���������͂����߂�
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

	// �R�F�d�͂����߂�
	g_Spring.gravity.x = 0.0f;
	g_Spring.gravity.y = SPRING_MASS * -0.98f;

	// �S�F��R�͂����߂�
	resistance.x = -g_Spring.velocity.x * ATT_COEFFICIENT;
	resistance.y = -g_Spring.velocity.y * ATT_COEFFICIENT;

	// �T�F�P����S�ŋ��߂��͂��������č��͂����߂�
	g_Spring.resultant.x = g_Spring.gravity.x + g_Spring.tension_down.x + resistance.x;
	g_Spring.resultant.y = g_Spring.gravity.y + g_Spring.tension_down.y + resistance.y;

	if (g_Spring.onDrag == false)
	{
		// �U�F���͂Ǝ��ʂ�������x�����߂�
		g_Spring.acceleration.x = g_Spring.resultant.x / SPRING_MASS;
		g_Spring.acceleration.y = g_Spring.resultant.y / SPRING_MASS;

		// �V�F�����x���瑬�x�����߂�
		g_Spring.velocity.x += g_Spring.acceleration.x;
		g_Spring.velocity.y += g_Spring.acceleration.y;

		// �W�F���x����ʒu�����߂�
		g_Spring.position.x += g_Spring.velocity.x;
		g_Spring.position.y += g_Spring.velocity.y;

	}
}

void CSpring::Draw()
{
	m_vPos = g_Spring.position;
	CPolygon::Draw();
}