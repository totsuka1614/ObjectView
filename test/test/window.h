/******************************************************************************
* 
* @file      window.h
* @brief     ウィンドウクラス
* @author    Totsuka Kensuke
* @date      2023/04/27
* @note      
* @attention 
* 
******************************************************************************/
#ifndef WINDOW_H
#define WINDOW_H
//インクルード部
#include "main.h"

class CWindow
{
public:
	CWindow() {};
	~CWindow() {};
	void Release(void);

	HRESULT Create(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
	static CWindow* GetWindow(){ return m_pWindow; }
	HWND GetWindowHandle() { return m_hWnd; }

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT ReSizeWindow(HWND hWnd, LPCREATESTRUCT lpcs);
private:

	static CWindow*  m_pWindow;
	HWND			m_hWnd;					// メイン ウィンドウ ハンドル
	HINSTANCE		m_hInst;				// インスタンス ハンドル
	
};

#endif // !WINDOW_H
