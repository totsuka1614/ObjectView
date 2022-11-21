//= ============================================================================
//
// ���C������ [main.h]
// Author : TotsukaKensuke
//
//=============================================================================
#pragma once

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#define NOMINMAX
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <string>
#include <DirectXMath.h>

//
using namespace DirectX;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)	// �E�C���h�E�̒��S�x���W

#define	NUM_VERTEX		(4)					// ���_��
#define	NUM_POLYGON		(2)					// �|���S����

#define SinDeg(degree)	sinf(XMConvertToRadians(degree))
#define CosDeg(degree)	cosf(XMConvertToRadians(degree))

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

enum ObjectType
{
	BOX,
	SPHERE,
	FBX,
	MAX_OBJECTTYPE,
};

typedef struct
{
	XMFLOAT3 pos;
	XMFLOAT3 scale;
	XMFLOAT3 deglee;
}SAVE_TRANSFORM;

typedef struct
{
	XMFLOAT3 pos;
	XMFLOAT3 scale;
	XMFLOAT3 deglee;

	bool bEnable;
	ObjectType eType;
}SAVE_DATA;

typedef struct
{
	XMFLOAT3* pos;
	XMFLOAT3* scale;
	XMFLOAT3* deglee;
}TARGET_TRANSFORM;

// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ���_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT3 nor;		// �@���x�N�g��
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

typedef struct{
public:
	XMFLOAT4	Diffuse;		// Diffuse color RGBA
	XMFLOAT4	Ambient;		// Ambient color RGB
	XMFLOAT4	Specular;		// Specular 'shininess'
	XMFLOAT4	Emissive;		// Emissive color RGB
	float		Power;		// Sharpness if specular highlight
}MATERIAL;

//�萔�o�b�t�@�t�H�[�}�b�g
typedef struct{
	XMMATRIX	mWVP;		// ���[���h�~�r���[�~�ˉe�s��(�]�u�s��)
	XMMATRIX	mW;			// ���[���h�s��(�]�u�s��)
	XMMATRIX	mTex;		// �e�N�X�`���s��(�]�u�s��)
	XMFLOAT4	fLightVector;
}CONSTANT_BUFFER;

typedef	struct{
	XMVECTOR	vEye;		// ���_���W
	// ����
	XMVECTOR	vLightDir;	// ��������
	XMVECTOR	vLa;		// �����F(�A���r�G���g)
	XMVECTOR	vLd;		// �����F(�f�B�t���[�Y)
	XMVECTOR	vLs;		// �����F(�X�y�L����)
	// �}�e���A��
	XMVECTOR	vAmbient;	// �A���r�G���g�F(+�e�N�X�`���L��)
	XMVECTOR	vDiffuse;	// �f�B�t���[�Y�F
	XMVECTOR	vSpecular;	// �X�y�L�����F(+�X�y�L�������x)
	XMVECTOR	vEmissive;	// �G�~�b�V�u�F
}CONSTANT_BUFFER2;

enum PSShaderType
{
	UNLIT,
	LAMBERT,
	PHONG,
	PIXEL,
	EDGEPS,
	LIM,
	FOG,
	PIXEL2D,
	DISSOLVE,
	BUMPMAP,

	MAX_PSSHADER,
};

enum VSShaderType
{
	VERTEX,
	EDGEVS,
	VERTEX2D,

	MAX_VSSHADER,
};

enum EBlendState {
	BS_NONE = 0,							// ��������������
	BS_ALPHABLEND,							// ����������
	BS_ADDITIVE,							// ���Z����
	BS_SUBTRACTION,							// ���Z����

	MAX_BLENDSTATE
};

enum ECullMode {
	CULLMODE_NONE = 0,						// �J�����O���Ȃ�
	CULLMODE_CW,							// �O�ʃJ�����O
	CULLMODE_CCW,							// �w�ʃJ�����O

	MAX_CULLMODE
};

class Fps
{
public:
	void InitFps();
	void CalculationFps();
	void CalculationSleep();
	double GetFps();
private:
	double g_dFps;
	LARGE_INTEGER Freq = { 0 };
	LARGE_INTEGER StartTime = { 0 };
	LARGE_INTEGER NowTime = { 0 };
	int iCount = 0;
	DWORD SleepTime = 0;
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************