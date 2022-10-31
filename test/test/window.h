//=============================================================================
//
// �E�B���h�E �N���X��` [Window.h]
// Author : Totsuka Kensuke
//
//=============================================================================
#ifndef WINDOW_H
#define WINDOW_H

#include "main.h"

class Window
{
public:
	Window() {};
	~Window() {};
	void Release(void);

	HRESULT Create(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
	static Window* GetWindow(){ return m_pWindow; }
	HWND GetWindowHandle() { return m_hWnd; }

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT ReSizeWindow(HWND hWnd, LPCREATESTRUCT lpcs);
private:

	static Window*  m_pWindow;
	HWND			m_hWnd;					// ���C�� �E�B���h�E �n���h��
	HINSTANCE		m_hInst;				// �C���X�^���X �n���h��
	
};

#endif // !WINDOW_H
