//=============================================================================
//
// �E�B���h�E �N���X [Window.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
//�C���N���[�h��
#include "window.h"
#include "imgui_impl_win32.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		_T("AppClass")		// �E�C���h�E�̃N���X��
#define WINDOW_NAME		_T("a")		// �E�C���h�E�̃L���v�V������

Window g_window;

Window* Window::m_pWindow = &g_window;

void Window::Release()
{
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, m_hInst);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg) {
	case WM_CREATE:					//�E�B���h�E���������ꂽ
		return ReSizeWindow(hWnd, (LPCREATESTRUCT)lParam);
	case WM_DESTROY:				//�E�B���h�E�j���w��������
		PostQuitMessage(0);				//�V�X�e���ɃX���b�h�̏I����v��
		break;
	case WM_KEYDOWN:				//�L�[�{�[�h�������ꂽ
		switch (wParam) {
		case VK_ESCAPE:					// ESC�L�[�������ꂽ
			PostMessage(hWnd, WM_CLOSE, 0, 0);	// x�������ꂽ�悤�ɐU����
			return 0;
		}
		break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;			// Alt+Enter����BEEP��}�~
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

HRESULT Window::Create(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = {
	sizeof(WNDCLASSEX),
	CS_CLASSDC,
	WindowProc,
	0,
	0,
	hInstance,
	LoadIcon(hInstance, MAKEINTRESOURCE(NULL)),
	LoadCursor(nullptr, IDC_ARROW),
	(HBRUSH)(COLOR_WINDOW + 1),
	nullptr,
	CLASS_NAME,
	nullptr
	};

	// COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		MessageBox(NULL, _T("COM�̏������Ɏ��s���܂����B"), _T("error"), MB_OK);
		return -1;
	}

	// �C���X�^���X �n���h���ۑ�
	m_hInst = hInstance;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�T�C�Y����E�B���h�E �T�C�Y�Z�o		
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION
		| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX;
	DWORD dwExStyle = 0;
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

	// �E�B���h�E�̍쐬
	m_hWnd = CreateWindowEx(dwExStyle,
		CLASS_NAME,
		WINDOW_NAME,
		dwStyle,
		CW_USEDEFAULT,		// �E�B���h�E�̍����W
		CW_USEDEFAULT,		// �E�B���h�E�̏���W
		rc.right - rc.left,	// �E�B���h�E����
		rc.bottom - rc.top,	// �E�B���h�E�c��
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	// �E�C���h�E�̕\��
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return 0;
}

HRESULT Window::ReSizeWindow(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// �N���C�A���g�̈�T�C�Y��SCREEN_WIDTH�~SCREEN_HEIGHT�ɍĐݒ�.
	RECT rcClnt;
	GetClientRect(hWnd, &rcClnt);
	rcClnt.right -= rcClnt.left;
	rcClnt.bottom -= rcClnt.top;
	if (rcClnt.right != SCREEN_WIDTH || rcClnt.bottom != SCREEN_HEIGHT) {
		RECT rcWnd;
		GetWindowRect(hWnd, &rcWnd);
		SIZE sizeWnd;
		sizeWnd.cx = (rcWnd.right - rcWnd.left) - rcClnt.right + SCREEN_WIDTH;
		sizeWnd.cy = (rcWnd.bottom - rcWnd.top) - rcClnt.bottom + SCREEN_HEIGHT;
		SetWindowPos(hWnd, nullptr, 0, 0, sizeWnd.cx, sizeWnd.cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	// IME������**���ȓ��͂������Ȃ��悤��
	ImmAssociateContext(hWnd, nullptr);

	return 0;	// -1��Ԃ���CreateWindow[Ex]�����s����.
}