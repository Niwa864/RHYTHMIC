//
// DirectInput クラス実装
//
#include "Input.h"
#include "define.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

// 静的メンバ変数
LPDIRECTINPUT8			CInput::m_pDI = NULL;				// DirectInput8 オブジェクト
LPDIRECTINPUTDEVICE8	CInput::m_pDIDeviceKey = NULL;		// DirectInputDevice8 オブジェクト
BYTE					CInput::m_diKeyState[MAX_KEY] = { 0 };// キーボード情報取得バッファ
BYTE					CInput::m_diPrevKeyState[MAX_KEY] = { 0 };// 直前のキーボード情報
LPDIRECTINPUTDEVICE8	CInput::m_pDIDeviceMouse = NULL;		// DirectInputDevice8 オブジェクト
DIMOUSESTATE			CInput::m_diMouseState = { 0 };			// マウス情報取得バッファ
DIMOUSESTATE			CInput::m_diPrevMouseState = { 0 };		// 直前のマウス情報
LPDIRECTINPUTDEVICE8	CInput::m_pDIDeviceJoy[MAX_JOY] = { 0 };	// DirectInputDevice8 オブジェクト
DIJOYSTATE				CInput::m_diJoyState[MAX_JOY] = { 0 };	// ジョイスティック情報取得バッファ
DIJOYSTATE				CInput::m_diPrevJoyState[MAX_JOY] = { 0 };// 直前のジョイスティック情報
int						CInput::m_nJoy = 0;						// ジョイスティック接続数
TCHAR					g_szTitle[] = _T("CInput");

// ジョイスティック問い合わせ用コールバック関数
BOOL CALLBACK CInput::EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	DIDEVCAPS diDevCaps;			// デバイス情報

									// ジョイスティック用デバイスオブジェクトを作成
	HRESULT hr = m_pDI->CreateDevice(lpddi->guidInstance, &m_pDIDeviceJoy[m_nJoy], NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;		// 列挙を続ける
									// ジョイスティックの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = m_pDIDeviceJoy[m_nJoy]->GetCapabilities(&diDevCaps);
	if (FAILED(hr)) {
		SAFE_RELEASE(m_pDIDeviceJoy[m_nJoy]);
		return DIENUM_CONTINUE;		// 列挙を続ける
	}
	if (++m_nJoy < MAX_JOY)
		return DIENUM_CONTINUE;		// 列挙を続ける
	return DIENUM_STOP;		// 必要数揃ったので列挙を終了する
}

// コンストラクタ
CInput::CInput(void)
{
}

// デストラクタ
CInput::~CInput(void)
{
}

// 初期化
void CInput::Init(HWND hWnd)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

	// DirectInputオブジェクトの作成
	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInput オブジェクトの作成に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	//******** キーボード用
	// DirectInputDeviceオブジェクトの作成
	hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDeviceKey, NULL);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInputDevice オブジェクトの作成に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	// データフォーマットの設定
	hr = m_pDIDeviceKey->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInput でデータフォーマットの設定に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	// 協調レベルの設定
	hr = m_pDIDeviceKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInputで協調レベルの設定に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	// デバイスへの入力制御開始
	hr = m_pDIDeviceKey->Acquire();
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInput でデバイスへの入力制御開始に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	//******** マウス用
	// DirectInputDeviceオブジェクトの作成
	hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pDIDeviceMouse, NULL);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInputDevice オブジェクトの作成に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	// データフォーマットの設定
	hr = m_pDIDeviceMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInput でデータフォーマットの設定に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	// 協調レベルの設定
	hr = m_pDIDeviceMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInputで協調レベルの設定に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	// デバイスの設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;	// 相対値モードに設定
	m_pDIDeviceMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);

	// デバイスへの入力制御開始
	hr = m_pDIDeviceMouse->Acquire();
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInput でデバイスへの入力制御開始に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	//******** ジョイスティック用
	// DirectInputDeviceオブジェクトの作成
	m_nJoy = 0;
	hr = m_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr)) {
		MessageBox(hWnd, _T("DirectInputDevice オブジェクトの作成に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
		return;
	}

	for (int i = 0; i < m_nJoy; i++) {
		// データフォーマットの設定
		hr = m_pDIDeviceJoy[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(hr)) {
			MessageBox(hWnd, _T("DirectInput でデータフォーマットの設定に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
			return;
		}

		// 協調レベルの設定
		hr = m_pDIDeviceJoy[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr)) {
			MessageBox(hWnd, _T("DirectInputで協調レベルの設定に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
			return;
		}

		// デバイスの設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_X;
		diprg.lMax = JOY_MAX_X;
		m_pDIDeviceJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｘ方向の範囲を指定
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = JOY_MIN_Y;
		diprg.lMax = JOY_MAX_Y;
		m_pDIDeviceJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定

																	// デバイスへの入力制御開始
		hr = m_pDIDeviceJoy[i]->Acquire();
		if (FAILED(hr)) {
			MessageBox(hWnd, _T("DirectInput でデバイスへの入力制御開始に失敗しました！"), g_szTitle, MB_OK | MB_ICONSTOP);
			return;
		}
	}
}

// 解放
void CInput::Fin(void)
{
	for (int i = 0; i < m_nJoy; i++) {
		if (m_pDIDeviceJoy[i])
			m_pDIDeviceJoy[i]->Unacquire();	// 入力を終了
		SAFE_RELEASE(m_pDIDeviceJoy[i]);	// DirectInputDevice8 オブジェクトの解放
	}
	if (m_pDIDeviceKey)
		m_pDIDeviceKey->Unacquire();		// 入力を終了
	SAFE_RELEASE(m_pDIDeviceKey);			// DirectInputDevice8 オブジェクトの解放
	if (m_pDIDeviceMouse)
		m_pDIDeviceMouse->Unacquire();		// 入力を終了
	SAFE_RELEASE(m_pDIDeviceMouse);			// DirectInputDevice8 オブジェクトの解放
	SAFE_RELEASE(m_pDI);					// DirectInput8 オブジェクトの解放
}

// 更新
void CInput::Update(void)
{
	HRESULT hr;

	if (m_pDIDeviceKey) {
		CopyMemory(m_diPrevKeyState, m_diKeyState, MAX_KEY);
		m_pDIDeviceKey->Acquire();			// 要求を出す
											// 現在のキーボードの状態を取得
		hr = m_pDIDeviceKey->GetDeviceState(MAX_KEY, &m_diKeyState);
		if (hr == DIERR_INPUTLOST)
			m_pDIDeviceKey->Acquire();		// 再度、要求を出す
	}
	if (m_pDIDeviceMouse) {
		m_diPrevMouseState = m_diMouseState;
		m_pDIDeviceMouse->Acquire();		// 要求を出す
											// 現在のマウスの状態を取得
		hr = m_pDIDeviceMouse->GetDeviceState(sizeof(m_diMouseState), &m_diMouseState);
		if (hr == DIERR_INPUTLOST)
			m_pDIDeviceMouse->Acquire();	// 再度、要求を出す
	}
	for (int i = 0; i < m_nJoy; i++) {
		m_diPrevJoyState[i] = m_diJoyState[i];
		m_pDIDeviceJoy[i]->Acquire();		// 要求を出す
		m_pDIDeviceJoy[i]->Poll();			// デバイス上のポーリング対象オブジェクトからデータを取得
											// 現在のジョイスティックの状態を取得
		hr = m_pDIDeviceJoy[i]->GetDeviceState(sizeof(m_diJoyState[0]), &m_diJoyState[i]);
		if (hr == DIERR_INPUTLOST)
			m_pDIDeviceJoy[i]->Acquire();	// 再度、要求を出す
	}
}

// キーの押下状態を取得
bool CInput::GetKeyPress(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diKeyState[nKey] & 0x80) != 0;
}

// キーのトリガ状態を取得
bool CInput::GetKeyTrigger(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diPrevKeyState[nKey] & 0x80) == 0 &&
		(m_diKeyState[nKey] & 0x80) != 0;
}

// キーのリリ－ス状態を取得
bool CInput::GetKeyRelease(int nKey)
{
	if (nKey < 0 || nKey >= MAX_KEY) return false;
	return (m_diPrevKeyState[nKey] & 0x80) != 0 &&
		(m_diKeyState[nKey] & 0x80) == 0;
}

// ジョイスティック数取得
int CInput::GetNumJoystick(void)
{
	return m_nJoy;
}

// ジョイスティックのボタンの押下状態取得
bool CInput::GetJoyPress(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diJoyState[nJoy].rgbButtons[nButton] & 0x80) != 0;
}

// ジョイスティックのボタンのトリガ状態取得
bool CInput::GetJoyTrigger(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diPrevJoyState[nJoy].rgbButtons[nButton] & 0x80) == 0 &&
		(m_diJoyState[nJoy].rgbButtons[nButton] & 0x80) != 0;
}

// ジョイスティックのボタンのリリース状態取得
bool CInput::GetJoyRelease(int nJoy, int nButton)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return false;
	if (nButton < 0 || nButton >= 32) return false;
	return (m_diPrevJoyState[nJoy].rgbButtons[nButton] & 0x80) != 0 &&
		(m_diJoyState[nJoy].rgbButtons[nButton] & 0x80) == 0;
}

// ジョイスティックの軸状態取得
float CInput::GetJoyAxis(int nJoy, int nAxis)
{
	if (nJoy < 0 || nJoy >= m_nJoy) return 0.0f;
	switch (nAxis) {
	case JOY_X:
		if (m_diJoyState[nJoy].lX < 0) {
			return m_diJoyState[nJoy].lX / (float)-JOY_MIN_X;
		}
		return m_diJoyState[nJoy].lX / (float)JOY_MAX_X;
	case JOY_Y:
		if (m_diJoyState[nJoy].lY < 0) {
			return m_diJoyState[nJoy].lY / (float)-JOY_MIN_Y;
		}
		return m_diJoyState[nJoy].lY / (float)JOY_MAX_Y;
	default:
		break;
	}
	return 0.0f;
}

// マウスのボタンの押下状態取得
bool CInput::GetMousePress(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diMouseState.rgbButtons[nButton] & 0x80) != 0;
}

// マウスのボタンのトリガ状態取得
bool CInput::GetMouseTrigger(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diPrevMouseState.rgbButtons[nButton] & 0x80) == 0 &&
		(m_diMouseState.rgbButtons[nButton] & 0x80) != 0;
}

// マウスのボタンのリリース状態取得
bool CInput::GetMouseRelease(int nButton)
{
	if (nButton < 0 || nButton >= 4) return false;
	return (m_diPrevMouseState.rgbButtons[nButton] & 0x80) != 0 &&
		(m_diMouseState.rgbButtons[nButton] & 0x80) == 0;
}

// マウスの変化量取得
long CInput::GetMouseAxis(int nAxis)
{
	switch (nAxis) {
	case MOUSE_X:
		return m_diMouseState.lX;
	case MOUSE_Y:
		return m_diMouseState.lY;
	case MOUSE_Z:
		return m_diMouseState.lZ;
	}
	return 0L;
}