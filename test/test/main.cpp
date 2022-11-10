//=============================================================================
//
// ���C������ [main.cpp]
// Author : TotsukaKensuke
//
//=============================================================================

//�C���N���[�h��
#include "main.h"
#include "backbuffer.h"
#include "model.h"
#include "window.h"
#include "GUI.h"
#include "FBX.h"
#include "Camera.h"
#include "Input.h"
#include "grid.h"
//���C�u�����̃����N
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")
#pragma comment(lib, "d3d11")

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
int							g_nCountFPS;			// FPS�J�E���^
Model *model;
Grid *grid;

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// ���g�p�錾
	UNREFERENCED_PARAMETER(lpCmdLine);		// ���g�p�錾

	Window* window = Window::GetWindow();

	if(window->Create(hInstance, hPrevInstance,lpCmdLine,nCmdShow))
		return -1;
	
	MSG msg;

	// DirectX�̏�����
	if (FAILED(Init(window->GetWindowHandle(), true))) {
		return -1;
	}

	GUI::Get()->Init();
	grid = new Grid;
	grid->Init();
	CInput::Init();
	Fps fps;

	fps.InitFps();

	// ���b�Z�[�W���[�v
	for (;;) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {

			fps.CalculationFps();

			// �X�V����
			Update();
			// �`�揈��
			Draw();
			fps.CalculationSleep();
		}
	}

	// �^�C�}�ݒ�����ɖ߂�
	timeEndPeriod(1);

	// �I������
	Uninit();

	window->Release();

	// COM�I������
	CoUninitialize();

	return (int)msg.wParam;
}

#pragma region FPS�����֌W
void Fps::InitFps()
{
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);//���݂̎��Ԃ��擾�i1�t���[���ځj
}
/**
 * @fn
 * FPS�v�Z����
 * @brief FPS���v�Z����iCount�Ɏ��[
 */
void Fps::CalculationFps()
{
	//FPS�̌v�Z
	if (iCount == 60)//�J�E���g��60�̎��̏���
	{
		QueryPerformanceCounter(&NowTime);//���݂̎��Ԃ��擾�i60�t���[���ځj
		//FPS = 1�b / 1�t���[���̕`��ɂ����鎞��
		g_dFps = 1000.0 / (static_cast<double>((NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart) / 60.0);
		iCount = 0;//�J�E���g�������l�ɖ߂�
		StartTime = NowTime;//1�t���[���ڂ̎��Ԃ����݂̎��Ԃɂ���
	}
	iCount++;//�J�E���g+1
}
/**
 * @fn
 * FPS�ҋ@����
 * @brief FPS��60�𒴂��Ȃ��悤�A�X���[�v����
 */
void Fps::CalculationSleep()
{
	//Sleep�����鎞�Ԃ̌v�Z
	QueryPerformanceCounter(&NowTime);//���݂̎��Ԃ��擾
	//Sleep�����鎞��ms = 1�t���[���ڂ��猻�݂̃t���[���܂ł̕`��ɂ�����ׂ�����ms - 1�t���[���ڂ��猻�݂̃t���[���܂Ŏ��ۂɂ�����������ms
	//                  = (1000ms / 60)*�t���[���� - (���݂̎���ms - 1�t���[���ڂ̎���ms)
	SleepTime = static_cast<DWORD>((1000.0 / 60.0) * iCount - (NowTime.QuadPart - StartTime.QuadPart) * 1000 / Freq.QuadPart);
	if (SleepTime > 0 && SleepTime < 18)//�傫���ϓ����Ȃ����SleepTime��1�`17�̊Ԃɔ[�܂�
	{
		timeBeginPeriod(1);
		Sleep(SleepTime);
		timeEndPeriod(1);
	}
	else//�傫���ϓ����������ꍇ
	{
		timeBeginPeriod(1);
		Sleep(1);
		timeEndPeriod(1);
	}
}
/**
 * @fn
 * FPS�Q�b�^�[
 * @brief FPS�l��Ԃ�����
 * @return g_dFPS
 */
double Fps::GetFps()
{
	return g_dFps;
}
#pragma endregion

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	
	hr = BackBuffer::GetBuffer()->Init();
	if (FAILED(hr))
		return hr;

	model = new Model;
	model->Init();

	// ���X�^���C�Y�ݒ�
	//D3D11_RASTERIZER_DESC rd;
	//ZeroMemory(&rd, sizeof(rd));
	//rd.FillMode = D3D11_FILL_SOLID;
	//rd.CullMode = D3D11_CULL_NONE;	// �J�����O����(���ʕ`��)
	//g_pDevice->CreateRasterizerState(&rd, &g_pRs[0]);
	//rd.CullMode = D3D11_CULL_FRONT;	// �O�ʃJ�����O(���ʕ`��)
	//g_pDevice->CreateRasterizerState(&rd, &g_pRs[1]);
	//rd.CullMode = D3D11_CULL_BACK;	// �w�ʃJ�����O(�\�ʕ`��)
	//g_pDevice->CreateRasterizerState(&rd, &g_pRs[2]);
	//g_pDeviceContext->RSSetState(g_pRs[2]);

	

	//// �[�x�X�e���V���X�e�[�g����
	//CD3D11_DEFAULT def;
	//CD3D11_DEPTH_STENCIL_DESC dsd(def);
	//dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//g_pDevice->CreateDepthStencilState(&dsd, &g_pDSS[0]);
	//CD3D11_DEPTH_STENCIL_DESC dsd2(def);
	//dsd2.DepthEnable = FALSE;
	//g_pDevice->CreateDepthStencilState(&dsd2, &g_pDSS[1]);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	GUI::Get()->Release();
	delete model;
	delete grid;
	CInput::Fin();
	BackBuffer::GetBuffer()->Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	CInput::Update();
	GUI::Get()->Update();
	CCamera::Get()->Update();
	model->Update();

	GUI::Get()->Display();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	BackBuffer::GetBuffer()->StartRendering();

	GUI::Get()->Draw();
	grid->Draw();
	model->Draw();
	BackBuffer::GetBuffer()->FinishRendering();
}