//=======================================================================================
//
//	  ゲーム用ウィンドウ クラス定義
//
//=======================================================================================
#include "GameWnd.h"
#include "define.h"
#include "Input.h"

#pragma comment(lib, "imm32")

//---------------------------------------------------------------------------------------
//	ウィンドウ初期化
//---------------------------------------------------------------------------------------
bool CGameWindow::InitInstance()
{
	m_pGraph = NULL;
	m_pScene = NULL;

	::ImmAssociateContext(GetHwnd(), NULL);

	if (!CWindow::InitInstance()) return false;

	m_bWindow = true;
	//int nID = MessageBox(_T("ウィンドウモードで実行しますか？"), _T("画面モード"), MB_YESNOCANCEL);
	//if (nID == IDCANCEL) return false;
	//if (nID == IDYES) m_bWindow = true;

	::timeBeginPeriod(1);	// タイマの分解能を最小にセット

							// グラフィックデバイス初期化
	m_pGraph = CGraphics::Create(GetHwnd(), SCREEN_WIDTH, SCREEN_HEIGHT, m_bWindow);
	if (!m_pGraph) return false;

	// DirectInput初期化
	CInput::Init(GetHwnd());

	// シーン初期化
	m_pScene = CScene::Create(m_pGraph);
	if (!m_pScene) return false;
	m_pScene->InitObj();

	m_dwFPSLastTime = ::timeGetTime();
	m_dwExecLastTime = m_dwFPSLastTime - FRAME_RATE;// 必ず最初に m_pScene->Update() を実行
	m_dwFrameCount = 0;

	return true;
}

//---------------------------------------------------------------------------------------
//	ウィンドウ解放
//---------------------------------------------------------------------------------------
int CGameWindow::ExitInstance()
{
	// シーン解放
	SAFE_RELEASE(m_pScene);

	// DirectInput解放
	CInput::Fin();

	// グラフィック デバイス解放
	SAFE_RELEASE(m_pGraph);

	::timeEndPeriod(1);				// タイマの分解能を元に戻す

	return CWindow::ExitInstance();	// 実行ファイルの戻り値
}

//---------------------------------------------------------------------------------------
//	アイドル時処理
//---------------------------------------------------------------------------------------
bool CGameWindow::OnIdle(long lCount)
{
	// この辺でフレーム数カウント
	DWORD dwCurrentTime = ::timeGetTime();			// 現在のタイマ値を取得
	if (dwCurrentTime - m_dwFPSLastTime >= 500) {	// 0.5 秒ごとに計測
													// フレーム数を計算
		if (m_pScene) {
			m_pScene->SetFPS(m_dwFrameCount * 1000 / (dwCurrentTime - m_dwFPSLastTime));
		}
		m_dwFPSLastTime = dwCurrentTime;	// タイマ値を更新
		m_dwFrameCount = 0;					// フレームカウンタをリセット
	}
	// この辺で時間管理
	while (dwCurrentTime - m_dwExecLastTime >= FRAME_RATE)
	{	// 一定時間が経過したら…
		m_dwExecLastTime += FRAME_RATE;							// タイマ値を更新
		if (m_pScene)
		{
			m_pScene->Update();									// ゲーム メイン処理
		}


	}

	m_dwFrameCount++;					// フレームカウント＋１

										// パソコンの処理ごとにレンダリングをする
	if (m_pScene) {
		m_pScene->Render();				// レンダリング処理
	}
	return true;
}

//---------------------------------------------------------------------------------------
//	WM_KEYDOWN ハンドラ
//---------------------------------------------------------------------------------------
void CGameWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// [Esc] が押されていたら
	if (nChar == VK_END) {
		// ウィンドウを閉じる
		OnClose();
	}
}

//---------------------------------------------------------------------------------------
//	WM_ERASEBKGND ハンドラ
//---------------------------------------------------------------------------------------
bool CGameWindow::OnEraseBkgnd(HDC hdc)
{
	// 背景消去しないで消去済みとして返す
	return true;
}

//---------------------------------------------------------------------------------------
//	WM_SETCURSOR ハンドラ
//---------------------------------------------------------------------------------------
bool CGameWindow::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	// 全画面モードか、マウスカーソル座標がクライアント領域内なら
	if (!m_bWindow || nHitTest == HTCLIENT) {
		// マウスカーソル消去
		::SetCursor(NULL);
		return true;
	}
	return false;
}

//=======================================================================================
//	End of File
//=======================================================================================