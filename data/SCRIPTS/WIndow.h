//=======================================================================================
//
//	  ウィンドウ クラス定義
//
//=======================================================================================
#pragma once
#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0601
#endif
#include <windows.h>
#include <tchar.h>

/* =================================== */
/* =====		 クラス定義		 ===== */
/* =================================== */
class CWindow
{
public:
	// コンストラクタ／デストラクタ
	CWindow();
	virtual ~CWindow();

	// 属性
private:
	HWND		m_hWnd;				// ウィンドウ ハンドル
	HINSTANCE	m_hInstance;		// インスタンス ハンドル
	ATOM		m_Atom;				// ウィンドウ クラス登録時の ATOM
	LPTSTR		m_pszClassName;		// クラス名
	WNDPROC		m_lpfnOldWndProc;	// 
	bool		m_bDialog;			// 
	MSG			m_msgCur;			// 現在のメッセージ
	POINT		m_ptCursorLast;		// 
	UINT		m_nMsgLast;			// 
									// メッセージを振り分けるウィンドウ(ダイアログ)プロシージャ
	static LRESULT CALLBACK WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	// オーバーライド可能なウィンドウ(ダイアログ)プロシージャ
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ウィンドウのクライアント領域を描画
	virtual void OnDraw(HDC hdc);

	virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool OnCommand(WPARAM wParam, LPARAM lParam);

	virtual int OnCreate(LPCREATESTRUCT pcs);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnPaint();
	virtual bool OnEraseBkgnd(HDC hdc);
	virtual void OnClose();
	virtual void OnDestroy();
	virtual bool OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg);
	virtual void OnMouseMove(UINT nFlags, POINT pt);
	virtual void OnDropFiles(HDROP hDropInfo);
	virtual bool OnMouseWheel(UINT nFlags, short zDelta, POINT pt);

	bool RegisterClass();

public:
	bool Create(LPCTSTR pszWindowName,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0,
		int nX = CW_USEDEFAULT, int nY = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL, HMENU nIDorHMenu = NULL, LPVOID pParam = NULL);
	bool Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
		const RECT& rc, CWindow* pParentWnd, UINT nID, LPVOID pParam = NULL);
	bool CreateFrame(LPCTSTR pszWindowName,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0,
		int nX = CW_USEDEFAULT, int nY = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL, HMENU nIDorHMenu = NULL, LPVOID pParam = NULL);
	void Close() { OnClose(); }

	// ウィンドウハンドルとCWindowオブジェクトを結び付ける
	bool Attach(HWND hWnd);
	// ウィンドウハンドルをCWindowオブジェクトから切り離す
	HWND Detach();

	// 初期化
	virtual bool InitInstance();

	// デフォルトのメッセージ ループ
	virtual int Run();
	virtual bool PreTranslateMessage(MSG* pMsg);
	virtual bool PumpMessage();
	virtual bool OnIdle(long lCount);
	virtual bool IsIdleMessage(MSG* pMsg);

	// 終了処理
	virtual int ExitInstance();

	// ウィンドウ ハンドルを返す
	HWND GetHwnd() const { return (this) ? m_hWnd : NULL; }
	operator HWND() const { return GetHwnd(); }

	// ウィンドウの表示
	bool Show(int nCmdShow) { return ::ShowWindow(m_hWnd, nCmdShow) != FALSE; }
	void Update() { ::UpdateWindow(m_hWnd); }

	// メッセージ ボックスの表示
	int MessageBox(LPCTSTR pszText, LPCTSTR pszCaption = NULL, UINT nType = MB_OK);

	// ウィンドウ サイズ
	bool GetClientRect(LPRECT pRect) { return ::GetClientRect(m_hWnd, pRect) != FALSE; }
};

//=======================================================================================
//	End of File
//=======================================================================================