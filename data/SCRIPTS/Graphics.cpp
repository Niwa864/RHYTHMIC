//=======================================================================================
//
//	  DirectX Graphics9 クラス
//
//=======================================================================================
#include <windows.h>
#include <tchar.h>
#include "define.h"
#include "Graphics.h"

LPDIRECT3DDEVICE9	CGraphics::m_pDevice = NULL;		// Direct3DDevice9 オブジェクト

//---------------------------------------------------------------------------------------
// グラフィック環境の初期化
//---------------------------------------------------------------------------------------
bool CGraphics::Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	// デバイス情報テーブル
	struct TCreateDevice {
		D3DDEVTYPE	type;			// デバイスの種類
		DWORD		behavior;		// デバイスの動作
	};
	const int c_nMaxDevice = 3;
	const TCreateDevice device[c_nMaxDevice] = {
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// ハードウェアシェーダを使う
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
		{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
	};
	int		nDev;			// デバイス種別カウンタ

							// Direct3Dオブジェクトを生成
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		MessageBox(hWnd, _T("Direct3Dオブジェクトの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}
	// Direct3D 初期化パラメータの設定
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	m_D3DPP.BackBufferWidth = (UINT)nWidth;				// ゲーム画面サイズ
	m_D3DPP.BackBufferHeight = (UINT)nHeight;
	m_D3DPP.Windowed = bWindow;					// ウィンドウモードか?
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.EnableAutoDepthStencil = true;						// Ｚバッファ有効
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;				// Ｚバッファフォーマット
	if (bWindow) {
		// ウィンドウモード
		m_D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;				// バックバッファ
		m_D3DPP.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	}
	else {
		// フルスクリーンモード
		m_D3DPP.BackBufferFormat = D3DFMT_X8R8G8B8;				// バックバッファ
		m_D3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
		m_D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	}

	// デバイスオブジェクトを作成
	for (nDev = 0; nDev < c_nMaxDevice; nDev++) {
		if (SUCCEEDED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, device[nDev].type, hWnd,	// デバイスを作成
			device[nDev].behavior, &m_D3DPP, &m_pDevice))) break;
	}
	if (nDev >= c_nMaxDevice) {
		MessageBox(hWnd, _T("デバイスの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// 情報表示用フォントを設定
	D3DXCreateFont(m_pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);

	//-------- ここから３Ｄ関連
	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&m_MatView,
		&D3DXVECTOR3(0.0f, 100.0f, -300.0f),		// 視点座標
		&D3DXVECTOR3(0.0f, 50.0f, 0.0f),		// 注視点座標
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));		// アップベクトル
	m_pDevice->SetTransform(D3DTS_VIEW, &m_MatView);

	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH(&m_MatProj, D3DXToRadian(FOVY),
		(float)m_nWidth / m_nHeight, NEAR_CLIP, FAR_CLIP);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);

	// マテリアルの設定
	D3DMATERIAL9	mtrl;				// マテリアルオブジェクト
	ZeroMemory(&mtrl, sizeof(mtrl));	// いったんゼロでクリア
	mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0f;
	mtrl.Ambient = mtrl.Diffuse;
	m_pDevice->SetMaterial(&mtrl);

	// ライトの設定
	D3DXVECTOR3	vecDir;					// 方向を指定するベクトル
	D3DLIGHT9	light;					// ライトオブジェクト
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;	// 平行光源
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = light.Diffuse.a = 1.0f;
	light.Ambient.r = light.Ambient.g = light.Ambient.b = light.Ambient.a = 0.2f;
	light.Specular = light.Diffuse;
	vecDir = D3DXVECTOR3(1.0f, -1.0f, 1.0f);					// 光の向きを決める
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	// 正規化したベクトルを返す
	m_pDevice->SetLight(0, &light);
	m_pDevice->LightEnable(0, TRUE);	// ライト0を有効
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);	// 鏡面反射を有効

	m_pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);

	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);



	return true;
}

//---------------------------------------------------------------------------------------
// グラフィック環境のクリーンアップ
//---------------------------------------------------------------------------------------
void CGraphics::Finalize()
{
	SAFE_RELEASE(m_pFont);		// D3DXFont オブジェクトを解放
	SAFE_RELEASE(m_pDevice);	// Direct3DDevice オブジェクトを解放
	SAFE_RELEASE(m_pD3D);		// Direct3D オブジェクトを解放
}

//---------------------------------------------------------------------------------------
// 描画クラス生成
//---------------------------------------------------------------------------------------
CGraphics* CGraphics::Create(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	CGraphics* pGraph = new CGraphics();
	if (pGraph) {
		if (!pGraph->Initialize(hWnd, nWidth, nHeight, bWindow)) {
			SAFE_DELETE(pGraph);
		}
	}
	return pGraph;
}

//---------------------------------------------------------------------------------------
// 描画クラス解放
//---------------------------------------------------------------------------------------
void CGraphics::Release()
{
	Finalize();
	delete this;
}

//---------------------------------------------------------------------------------------
// レンダリング開始
//---------------------------------------------------------------------------------------
bool CGraphics::Begin()
{
	// ビューポートをクリア
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(128, 128, 255), 1.0f, 0);

	//m_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_XRGB(128, 128, 255), 1.0f, 0);

	return SUCCEEDED(m_pDevice->BeginScene());	// シーン開始
}

//---------------------------------------------------------------------------------------
// レンダリング終了
//---------------------------------------------------------------------------------------
void CGraphics::End()
{
	m_pDevice->EndScene();						// シーン終了
	//m_pDevice->Present(NULL, NULL, NULL, NULL);
}

//---------------------------------------------------------------------------------------
// バックバッファ入替
//---------------------------------------------------------------------------------------
void CGraphics::SwapBuffer()
{
	// バックバッファに描画した内容を表示
	if (FAILED(m_pDevice->Present(NULL, NULL, NULL, NULL))) {
		m_pDevice->Reset(&m_D3DPP);
	}
}

//---------------------------------------------------------------------------------------
// デバッグ文字列描画
//---------------------------------------------------------------------------------------
void CGraphics::DrawText(int nX, int nY, LPCTSTR psz)
{
	RECT rcStr = { nX, nY, m_nWidth, m_nHeight };
	m_pFont->DrawText(NULL, psz, -1, &rcStr, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//=======================================================================================
//	End of File
//=======================================================================================