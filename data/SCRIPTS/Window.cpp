//=======================================================================================
//
//	  ウィンドウ クラス
//
//=======================================================================================
#include "Window.h"

/* =================================== */
/* =====		 定数宣言		 ===== */
/* =================================== */
static const TCHAR g_szProc[] = _T("PropCWindowProc");	// プロパティを識別する文字列
static const TCHAR g_szClass[] = _T("CWindow");

/* =================================== */
/* =====		プログラム		 ===== */
/* =================================== */
// -------------------------------------------------------------------
// コンストラクタ
// -------------------------------------------------------------------
CWindow::CWindow() :
	m_hWnd(NULL),
	m_lpfnOldWndProc(NULL),
	m_bDialog(false),
	m_nMsgLast(0),
	m_pszClassName(NULL),
	m_Atom(NULL)
{
	m_ptCursorLast.x = m_ptCursorLast.y = -1;
	m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	::ZeroMemory(&m_msgCur, sizeof(m_msgCur));
}

// -------------------------------------------------------------------
// デストラクタ
// -------------------------------------------------------------------
CWindow::~CWindow()
{
	HWND hWnd = Detach();
	if (hWnd)
		::DestroyWindow(hWnd);
	if (m_Atom) {
		::UnregisterClass(m_pszClassName, m_hInstance);
		m_Atom = NULL;
	}
	if (m_pszClassName) {
		delete[] m_pszClassName;
		m_pszClassName = NULL;
	}
}

// -------------------------------------------------------------------
// [静的メンバ関数] 振り分け用のウィンドウプロシージャ
// -------------------------------------------------------------------
LRESULT CALLBACK CWindow::WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindow* pWnd = (CWindow*)::GetProp(hWnd, g_szProc);
	if (pWnd)	// インスタンス毎のプロシージャ処理
		return pWnd->WndProc(uMsg, wParam, lParam);

	// ダイアログの場合、FALSEを返す
	if (::GetWindowLong(hWnd, DWL_DLGPROC))
		return FALSE;
	// デフォルトウィンドウプロシージャを呼び出す
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// -------------------------------------------------------------------
// オブジェクト毎のウィンドウプロシージャ
// -------------------------------------------------------------------
LRESULT CWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		return OnCreate((LPCREATESTRUCT)lParam);

	case WM_KEYDOWN:
		OnKeyDown(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_PAINT:
		OnPaint();
		return 1;

	case WM_ERASEBKGND:
		if (OnEraseBkgnd((HDC)wParam))
			return 1;
		break;

	case WM_CLOSE:
		OnClose();
		return 0;

	case WM_DESTROY:
		OnDestroy();
		break;

	case WM_COMMAND:
		if (OnCommand(wParam, lParam))
			return 0;
		break;

	case WM_SETCURSOR:
		if (OnSetCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam)))
			return TRUE;
		break;

	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
		OnMouseMove(wParam, pt);
	}
	return 0;

	case WM_DROPFILES:
		OnDropFiles((HDROP)wParam);
		return 0;

	case WM_MOUSEWHEEL:
	{
		POINT pt;
		pt.x = (short)LOWORD(lParam);
		pt.y = (short)HIWORD(lParam);
		if (OnMouseWheel(GET_KEYSTATE_WPARAM(wParam),
			GET_WHEEL_DELTA_WPARAM(wParam), pt)) return 0;
	}
	break;

	default:
		break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}

// -------------------------------------------------------------------
// デフォルト ウィンドウ プロシージャ
// -------------------------------------------------------------------
LRESULT CWindow::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_lpfnOldWndProc)
		return ::CallWindowProc(m_lpfnOldWndProc, m_hWnd, uMsg, wParam, lParam);
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

// ---------------------------------------------------------------------
// WM_KEYDOWN メッセージ ハンドラ
//	in :	nChar	仮想キー コード
//			nRepCnt	リピート回数
//			nFlags	キー データ
//	out:	none
// ---------------------------------------------------------------------
void CWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	DefWindowProc(WM_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
}

// ---------------------------------------------------------------------
// WM_PAINT メッセージ ハンドラ
//	in :	none
//	out:	none
// ---------------------------------------------------------------------
void CWindow::OnPaint()
{
	PAINTSTRUCT ps;
	OnDraw(::BeginPaint(m_hWnd, &ps));
	::EndPaint(m_hWnd, &ps);
}

// ---------------------------------------------------------------------
// 背景消去メッセージ ハンドラ
//	in :	hdc		デバイスコンテキスト ハンドル
//	out:	bool	true; 処理済み、false; 未処理
// ---------------------------------------------------------------------
bool CWindow::OnEraseBkgnd(HDC hdc)
{
	return false;
}

// ---------------------------------------------------------------------
// 描画メッセージ ハンドラ
//	in :	hdc		デバイスコンテキスト ハンドル
//	out:	none
// ---------------------------------------------------------------------
void CWindow::OnDraw(HDC hdc)
{
}

// ---------------------------------------------------------------------
// WM_CLOSE メッセージ ハンドラ
//	in :	none
//	out:	none
// ---------------------------------------------------------------------
void CWindow::OnClose()
{
	// ウィンドウ破棄
	::DestroyWindow(m_hWnd);
}

// ---------------------------------------------------------------------
// WM_DESTROY メッセージ ハンドラ
//	in :	none
//	out:	none
// ---------------------------------------------------------------------
void CWindow::OnDestroy()
{
	// ウィンドウ ハンドル解放
	Detach();
	// アプリケーション終了
	::PostQuitMessage(0);
}

// ---------------------------------------------------------------------
// ウィンドウ クラス登録
// ---------------------------------------------------------------------
bool CWindow::RegisterClass()
{
	if (!m_pszClassName) {
		m_pszClassName = new TCHAR[_tcslen(g_szClass) + 8 + 1];
		::wsprintf(m_pszClassName, _T("%s%08X"), g_szClass, ::GetTickCount());
		if (!m_pszClassName)
			return false;
	}

	if (!m_Atom) {
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
		wcex.cbSize = sizeof(wcex);
		wcex.lpszClassName = m_pszClassName;						// クラス名
		wcex.lpfnWndProc = WndMapProc;							// ウィンドウプロシージャ
		wcex.style = CS_VREDRAW | CS_HREDRAW;				// 占有指定
		wcex.hInstance = m_hInstance;							// インスタンス
		wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);		// デフォルト カーソル
		wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);	// デフォルト アイコン
		wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);// 背景色
		wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);	// デフォルト アイコン
		m_Atom = ::RegisterClassEx(&wcex);
		if (!m_Atom)
			return false;
	}

	return true;
}

// ---------------------------------------------------------------------
// ウィンドウ生成
// ---------------------------------------------------------------------
bool CWindow::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	int nX, int nY, int nWidth, int nHeight,
	HWND hwndParent, HMENU nIDorHMenu, LPVOID pParam)
{
	if (!RegisterClass())
		return false;

	HWND hWnd = ::CreateWindowEx(dwExStyle, m_pszClassName, pszWindowName, dwStyle,
		nX, nY, nWidth, nHeight, hwndParent, nIDorHMenu, m_hInstance, pParam);
	if (hWnd) {
		if (!Attach(hWnd)) {
			::DestroyWindow(hWnd);
			m_hWnd = NULL;
		}
	}
	return m_hWnd != NULL;
}

// ---------------------------------------------------------------------
// ウィンドウ生成
// ---------------------------------------------------------------------
bool CWindow::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	const RECT& rc, CWindow* pParentWnd, UINT nID, LPVOID pParam)
{
	return Create(pszWindowName, dwStyle, dwExStyle,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		pParentWnd->m_hWnd, (HMENU)nID, pParam);
}

// ---------------------------------------------------------------------
// クライアント サイズを指定してウィンドウ生成
// ---------------------------------------------------------------------
bool CWindow::CreateFrame(LPCTSTR pszWindowName,
	DWORD dwStyle, DWORD dwExStyle,
	int nX, int nY, int nClientWidth, int nClientHeight,
	HWND hwndParent, HMENU nIDorHMenu, LPVOID pParam)
{
	RECT rc = { 0, 0, nClientWidth, nClientHeight };
	if (!::AdjustWindowRectEx(&rc, dwStyle,
		nIDorHMenu && !IS_INTRESOURCE(nIDorHMenu), dwExStyle))
		return false;

	return Create(pszWindowName, dwStyle, dwExStyle,
		nX, nY, rc.right - rc.left, rc.bottom - rc.top,
		hwndParent, nIDorHMenu, pParam);
}

// ---------------------------------------------------------------------
// ウィンドウ ハンドルと CWindow オブジェクトを結び付ける
// ---------------------------------------------------------------------
bool CWindow::Attach(HWND hWnd)
{
	// ウィンドウ ハンドル hWnd を m_hWnd に保存
	if (!hWnd)
		return false;
	m_hWnd = hWnd;
	// ダイアログかウィンドウかを判定する
	m_bDialog = (::GetWindowLong(hWnd, DWL_DLGPROC) != 0);
	int nIndex = m_bDialog ? DWL_DLGPROC : GWL_WNDPROC;

	// ウィンドウハンドルと CWindow オブジェクトを結びつける
	::SetProp(m_hWnd, g_szProc, (HANDLE)this);

	// 既存のウィンドウをサブクラス化する場合は、ウィンドウ(ダイアログ)
	// プロシージャをWndMapProcに置き換える
	if (::GetWindowLong(m_hWnd, nIndex) != (LONG)WndMapProc)
		m_lpfnOldWndProc = (WNDPROC)::SetWindowLong(m_hWnd, nIndex, (LONG)WndMapProc);

	return true;
}

// ---------------------------------------------------------------------
// ウィンドウハンドルを CWindow オブジェクトから切り離す
// ---------------------------------------------------------------------
HWND CWindow::Detach()
{
	if (!m_hWnd)
		return NULL;

	// ウィンドウがサブクラス化されている場合は、
	// ウィンドウ(ダイアログ)プロシージャを元に戻す。
	if (m_lpfnOldWndProc) {
		::SetWindowLong(m_hWnd, (m_bDialog ? DWL_DLGPROC : GWL_WNDPROC), (LONG)m_lpfnOldWndProc);
		m_lpfnOldWndProc = NULL;
	}

	// ウィンドウ ハンドルを CWindow オブジェクトから切り離す
	::RemoveProp(m_hWnd, g_szProc);

	HWND hWnd = m_hWnd;
	m_hWnd = NULL;
	return hWnd;
}

// ---------------------------------------------------------------------
// アプリケーション終了時の処理
// ---------------------------------------------------------------------
bool CWindow::InitInstance()
{
	return true;
}

// ---------------------------------------------------------------------
// デフォルトのメッセージ ループ
// ---------------------------------------------------------------------
int CWindow::Run()
{
	if (!InitInstance())
		return ExitInstance();

	bool bIdle = TRUE;
	long lIdleCount = 0;
	for (;;) {
		while (bIdle && !::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE)) {
			if (!OnIdle(lIdleCount++))
				bIdle = false;
		}

		do {
			if (!PumpMessage())
				return ExitInstance();
			if (IsIdleMessage(&m_msgCur)) {
				bIdle = true;
				lIdleCount = 0;
			}

		} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
	}
	return -1;	// ここには来ないはず。
}

// ---------------------------------------------------------------------
// メッセージをディスパッチするか否かの判定
// ---------------------------------------------------------------------
bool CWindow::PreTranslateMessage(MSG* pMsg)
{
	return false;
}

// ---------------------------------------------------------------------
// メッセージの取得とディスパッチ
// ---------------------------------------------------------------------
bool CWindow::PumpMessage()
{
	if (!::GetMessage(&m_msgCur, NULL, NULL, NULL))
		return false;

	if (!PreTranslateMessage(&m_msgCur)) {
		::TranslateMessage(&m_msgCur);
		::DispatchMessage(&m_msgCur);
	}
	return true;
}

// ---------------------------------------------------------------------
// 入力待ち時の処理
// ---------------------------------------------------------------------
bool CWindow::OnIdle(long lCount)
{
	return lCount < 0;
}

// ---------------------------------------------------------------------
// OnIdle 呼び出し可否の判定
// ---------------------------------------------------------------------
bool CWindow::IsIdleMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE) {
		if (m_ptCursorLast.x == pMsg->pt.x && m_ptCursorLast.y == pMsg->pt.y &&
			pMsg->message == m_nMsgLast)
			return false;
		m_ptCursorLast = pMsg->pt;
		m_nMsgLast = pMsg->message;
		return true;
	}
	// WM_PAINT と WM_SYSTIMER(キャレット点滅)
	return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}

// ---------------------------------------------------------------------
// アプリケーション終了時の処理
// ---------------------------------------------------------------------
int CWindow::ExitInstance()
{
	return m_msgCur.wParam;
}

// ---------------------------------------------------------------------
// WM_CREATE メッセージ ハンドラ
//	in :	pcs		作成データを格納する構造体
//	out:	none
// ---------------------------------------------------------------------
int CWindow::OnCreate(LPCREATESTRUCT pcs)
{
	return 0;
}

// ---------------------------------------------------------------------
// WM_COMMAND メッセージ ハンドラ
//	in :	wParam	通知コード、項目ID
//			lParam	コントロール ウィンドウ ハンドル
//	out:	true; 処理済み、false; 未処理
// ---------------------------------------------------------------------
bool CWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//	WORD wNotifyCode = HIWORD(wParam);	// 通知コード
	WORD wID = LOWORD(wParam);			// 項目ID､ コントロールID､ またはアクセラレータID
										//	HWND hwndCtl = (HWND)lParam;		// コントロールのハンドル
	if (m_bDialog) {
		switch (wID) {
		case IDOK:
		case IDCANCEL:
			::EndDialog(m_hWnd, wID);
			return true;
		default:
			break;
		}
	}
	return false;
}

// ---------------------------------------------------------------------
// WM_SETCURSOR メッセージ ハンドラ
//	in :	hwnd		カーソルのあるウィンドウのハンドル
//			nHitTest	ヒット テスト コード
//			uMouseMsg	マウス メッセージ識別子
//	out:	true; 処理済み、false; 未処理
// ---------------------------------------------------------------------
bool CWindow::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	return false;
}

// ---------------------------------------------------------------------
// WM_MOUSEMOVE メッセージ ハンドラ
//	in :	nFlags	キーのフラグ
//			pt		カーソルの位置
//	out:	none
// ---------------------------------------------------------------------
void CWindow::OnMouseMove(UINT nFlags, POINT pt)
{
	DefWindowProc(WM_MOUSEMOVE, nFlags, MAKELPARAM(pt.x, pt.y));
}

// ---------------------------------------------------------------------
// WM_DROPFILES メッセージ ハンドラ
//	in :	hDropInfo	ドロップされたファイルを格納する構造体のハンドル
//	out:	none
// ---------------------------------------------------------------------
void CWindow::OnDropFiles(HDROP hDropInfo)
{
	DefWindowProc(WM_DROPFILES, (WPARAM)hDropInfo, 0);
}

// ---------------------------------------------------------------------
// WM_MOUSEWHEEL メッセージ ハンドラ
//	in :	nFlags	キーのフラグ
//			zDelta	ホイール回転数
//			pt		カーソルの位置
//	out:	true; 処理済み、false; 未処理
// ---------------------------------------------------------------------
bool CWindow::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	return false;
}

// ---------------------------------------------------------------------
// メッセージ ボックスの表示
//	in :	pszText		メッセージ ボックスのテキストのアドレス
//			pszCaption	メッセージ ボックスのタイトルのアドレス
//			nType		メッセージ ボックスのスタイル
//	out:	押されたボタン
// ---------------------------------------------------------------------
int CWindow::MessageBox(LPCTSTR pszText, LPCTSTR pszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, pszText, pszCaption, nType);
}

//=======================================================================================
//	End of File
//=======================================================================================