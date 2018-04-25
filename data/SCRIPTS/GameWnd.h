//=======================================================================================
//
//	  ゲーム用ウィンドウ クラス定義
//
//=======================================================================================
#pragma once

#include "Window.h"
#include "Graphics.h"
#include "Scene.h"

#define SCREEN_WIDTH	1024		// スクリーンの幅
#define SCREEN_HEIGHT	768			// スクリーンの高さ
#define FRAME_RATE		(1000/60)	// フレームレート

class CGameWindow : public CWindow
{
private:
	CGraphics*	m_pGraph;			// 描画デバイス クラス
	CScene*		m_pScene;			// シーン

	bool		m_bWindow;			// ウィンドウ / フルスクリーン

	DWORD		m_dwExecLastTime;	// FPS計測用
	DWORD		m_dwFPSLastTime;
	DWORD		m_dwFrameCount;

public:
	bool InitInstance();
	int ExitInstance();
	bool OnIdle(long lCount);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool OnEraseBkgnd(HDC hdc);
	bool OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg);
};

//=======================================================================================
//	End of File
//=======================================================================================