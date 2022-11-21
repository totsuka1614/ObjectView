//= ============================================================================
//
// メイン処理 [main.h]
// Author : TotsukaKensuke
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
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
// マクロ定義
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

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(720)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)					// 頂点数
#define	NUM_POLYGON		(2)					// ポリゴン数

#define SinDeg(degree)	sinf(XMConvertToRadians(degree))
#define CosDeg(degree)	cosf(XMConvertToRadians(degree))

//*****************************************************************************
// 構造体定義
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

// 頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
typedef struct {
	XMFLOAT3 vtx;		// 頂点座標
	XMFLOAT4 diffuse;	// 拡散反射光
	XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_2D;

// 頂点フォーマット( 頂点座標[3D] / 法線ベクトル / 反射光 / テクスチャ座標 )
typedef struct {
	XMFLOAT3 vtx;		// 頂点座標
	XMFLOAT3 nor;		// 法線ベクトル
	XMFLOAT2 tex;		// テクスチャ座標
} VERTEX_3D;

typedef struct{
public:
	XMFLOAT4	Diffuse;		// Diffuse color RGBA
	XMFLOAT4	Ambient;		// Ambient color RGB
	XMFLOAT4	Specular;		// Specular 'shininess'
	XMFLOAT4	Emissive;		// Emissive color RGB
	float		Power;		// Sharpness if specular highlight
}MATERIAL;

//定数バッファフォーマット
typedef struct{
	XMMATRIX	mWVP;		// ワールド×ビュー×射影行列(転置行列)
	XMMATRIX	mW;			// ワールド行列(転置行列)
	XMMATRIX	mTex;		// テクスチャ行列(転置行列)
	XMFLOAT4	fLightVector;
}CONSTANT_BUFFER;

typedef	struct{
	XMVECTOR	vEye;		// 視点座標
	// 光源
	XMVECTOR	vLightDir;	// 光源方向
	XMVECTOR	vLa;		// 光源色(アンビエント)
	XMVECTOR	vLd;		// 光源色(ディフューズ)
	XMVECTOR	vLs;		// 光源色(スペキュラ)
	// マテリアル
	XMVECTOR	vAmbient;	// アンビエント色(+テクスチャ有無)
	XMVECTOR	vDiffuse;	// ディフューズ色
	XMVECTOR	vSpecular;	// スペキュラ色(+スペキュラ強度)
	XMVECTOR	vEmissive;	// エミッシブ色
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
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成

	MAX_BLENDSTATE
};

enum ECullMode {
	CULLMODE_NONE = 0,						// カリングしない
	CULLMODE_CW,							// 前面カリング
	CULLMODE_CCW,							// 背面カリング

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
// プロトタイプ宣言
//*****************************************************************************