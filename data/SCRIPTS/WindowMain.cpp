//===============================================================================
//
// ウィンドウ画面生成[WindowMain.cpp]
//
//===============================================================================

#include "GameWnd.h"
#include <windows.h>

//-------- ライブラリのリンク（こう記述しておけば別途リンク設定が不要となる）
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

//-- todo
#define CAPTION_NAME	_T("Rhythmic")	// キャプション

//-----------------------------------------------------------------
// エントリーポイント
//-----------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// ウィンドウを作成
	int nResult = EXIT_FAILURE;
	CGameWindow* pWnd = new CGameWindow();
	if (pWnd->CreateFrame(CAPTION_NAME,				// ウィンドウ キャプション
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, 0,	// ウィンドウ スタイル，拡張スタイル
		CW_USEDEFAULT, CW_USEDEFAULT,				// ウィンドウ位置
		SCREEN_WIDTH, SCREEN_HEIGHT)) {				// クライアント領域サイズ
		pWnd->Show(iCmdShow);							// ウィンドウを表示
		pWnd->Update();									// ウィンドウ表示を更新
		nResult = pWnd->Run();							// メッセージ ループ
	}
	delete pWnd;

	::CoUninitialize();
	return nResult;
}
//=================================================================================
// EOF
//=================================================================================