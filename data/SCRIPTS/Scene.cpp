//=======================================================================================
//
//	  ゲームシーン クラス
//
//=======================================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "define.h"
#include "Scene.h"
#include "Input.h"

bool CScene::m_bInit = true;
GAME_MODE CScene::m_Game = GAME_TITLE;
const int nMaxEnemy = 2;
const int nMaxPlayer = 1;

// シーンにオブジェクトを追加
void CScene::Add(CBaseObj* pObj)
{
	if (m_pObj) {
		m_pObj->SetBack(pObj);
		pObj->SetNext(m_pObj);
	}
	else {
		pObj->SetNext(NULL);
	}
	m_pObj = pObj;
	pObj->SetBack(NULL);
}

// シーンからオブジェクトを削除
void CScene::Del(CBaseObj* pObj)
{
	CBaseObj* pBack = pObj->GetBack();
	CBaseObj* pNext = pObj->GetNext();
	if (pBack) {
		pBack->SetNext(pNext);
	}
	else {
		m_pObj = pNext;
	}
	if (pNext) {
		pNext->SetBack(pBack);
	}
}

// オブジェクト検索
CBaseObj* CScene::Find(UINT uID, CBaseObj* p)
{
	CBaseObj* pObj = (p) ? p->GetNext() : m_pObj;
	if (uID) {
		while (pObj) {
			if (pObj->GetID() == uID) {
				break;
			}
			pObj = pObj->GetNext();
		}
	}
	return pObj;
}

// 全オブジェクト初期化
void CScene::InitObj()
{
	/*for (CBaseObj* p = m_pObj; p; p = p->GetNext()) {
		p->Init();
	}*/
	if(m_FrontEff == nullptr)
		m_FrontEff = new CFrontEffect(this);
	if (m_pSound == nullptr) {
		m_pSound = new CSound(this);
		m_pSound->Init();
	}
	LoopInit();
}

void CScene::LoopInit()
{
	if (m_bInit == true)
	{
		switch (m_Game)
		{
		case GAME_TITLE:
			m_pTitle = new CTitle(this);
			m_bInit = false;
			break;

		case GAME_SELECT:
			m_pSelect = new CSelect(this);
			m_bInit = false;
			break;

		case GAME_CHARSELECT:
			m_pSelectChar = new CSelectChara(this);
			m_bInit = false;
			break;
		case GAME_MAIN:
			for(int i = 0; i < nMaxPlayer; i++)
				m_pParty[i] = new CParty(this, i);
			m_Character = new CCharacter(this);
			m_Rhythm = new CRhythm(this);
			m_pBoss = new CBoss(this);
			for( int i = 0; i < nMaxEnemy; i++)
				m_Enemy[i] = new CEnemy(this, i);
			m_Stage = new CStage(this);
			m_StageBG = new CStageBG(this);
			m_bInit = false;
			break;

		case GAME_CLEAR:
			m_bInit = false;
			break;
		case GAME_OVER:
			m_bInit = false;
			break;

		}
	}
}

void CScene::LoopUnInit()
{
	if (m_bInit == true)
	{
		switch (m_Game)
		{
		case GAME_SELECT:
			m_pTitle->Fin();
			delete m_pTitle;
			break;
		case GAME_CHARSELECT:
			m_pSelect->Fin();
			delete m_pSelect;
			break;

		case GAME_MAIN:
			m_pSelectChar->Fin();
			delete m_pSelectChar;
			break;

		case GAME_TITLE:
			FinObj();
			for (int i = 0; i < nMaxPlayer; i++)
				delete m_pParty[i];
			delete m_Character;
			delete m_Rhythm;
			delete m_pBoss;
			for (int i = 0; i < nMaxEnemy; i++)
				delete m_Enemy[i];
			delete m_Stage;
			delete m_StageBG;
			break;

		}
	}
}

// 全オブジェクト破棄
void CScene::FinObj()
{
	CBaseObj* pNext;
	for (CBaseObj* p = m_pObj; p; p = p->GetNext()) {
		if (p->GetID() < ID_2D)continue;
		pNext = p->GetNext();
		p->Fin();
	}
}

// 全オブジェクト更新
void CScene::UpdateObj()
{
	CBaseObj* pNext;
	for (CBaseObj* p = m_pTop; p; p = pNext) {
		pNext = p->GetNext();
		p->PreUpdate();
	}

	for (CBaseObj* p = m_pObj; p; p = pNext) {
		pNext = p->GetNext();
		p->Update();
	}

	for (CBaseObj* p = m_pTop; p; p = pNext) {
		pNext = p->GetNext();
		p->PostUpdate();
	}

	_pCamera->PostUpdate();	// カメラ更新
}

// 全オブジェクト描画
void CScene::DrawObj()
{
	_pCamera->PreDraw();		// カメラ反映

								// 不透明部分描画
	CBaseObj* p;
	for (p = m_pObj; p; p = p->GetNext()) {
		p->Draw();
	}

	// 半透明部分描画
	LPDIRECT3DDEVICE9 pD = CGraphics::GetDevice();
	pD->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pD->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pD->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	p = m_pObj;
	for (p = m_pObj; p; p = p->GetNext()) {
		p->DrawAlpha();
	}
	pD->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pD->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


//---------------------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------------------
CScene::CScene() :
	m_pGraph(NULL),
	m_pObj(NULL),
	m_FPS(),
	m_dwTick(0)
{
	m_szDebug[0] = _T('\0');

	_pCamera = new CCamera(this);

}

//---------------------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------------------
CScene::~CScene()
{
	SAFE_DELETE(_pCamera);
}

//---------------------------------------------------------------------------------------
// シーン生成
//---------------------------------------------------------------------------------------
CScene* CScene::Create(CGraphics* pGraph)
{
	CScene* pScene = new CScene();
	if (pScene) {
		if (!pScene->Initialize(pGraph)) {
			SAFE_DELETE(pScene);
		}
	}
	return pScene;
}

//---------------------------------------------------------------------------------------
// デバイス、リソース初期化
//---------------------------------------------------------------------------------------
bool CScene::Initialize(CGraphics* pGraph)
{
	m_pGraph = pGraph;

	//-------- ゲーム用オブジェクトの初期化
	InitObj();

	return true;
}

//---------------------------------------------------------------------------------------
// デバイス、リソース解放
//---------------------------------------------------------------------------------------
void CScene::Finalize()
{
	//-------- ゲーム用オブジェクトの後始末
	FinObj();
}

//---------------------------------------------------------------------------------------
// シーン解放
//---------------------------------------------------------------------------------------
void CScene::Release()
{
	Finalize();
	delete this;
}

//---------------------------------------------------------------------------------------
// ゲーム描画処理（メインループからコールされる）
//---------------------------------------------------------------------------------------
void CScene::Render()
{
	LoopUnInit();
	LoopInit();

	if (m_pGraph->Begin()) {	// 描画開始

		Draw();					// 描画処理

		m_pGraph->End();		// 描画終了
	}
	m_pGraph->SwapBuffer();		// バックバッファ入替
}

//---------------------------------------------------------------------------------------
// ゲームメイン処理（メインループからコールされる）
//---------------------------------------------------------------------------------------
void CScene::Update()
{
	++m_dwTick;

	// 入力更新
	CInput::Update();

	//----- ここにゲーム本体処理
	UpdateObj();
}

//---------------------------------------------------------------------------------------
// 描画処理（CGraphics::Render() からコールされる）
//---------------------------------------------------------------------------------------
void CScene::Draw()
{
	/*m_szDebug[0] = _T('\0');	// デバッグ文字列初期化

								// FPS を画面に描画するための文字列を作成
	TCHAR	str[256];
	_stprintf(str, _T("FPS = %d\n"), m_FPS);
	lstrcat(m_szDebug, str);*/

	//----- ここに描画処理
	//CScene::Draw3D();		// 3D描画
	CScene::Draw2D();		// 2D描画

							//DrawObj();

	// デバッグ文字列描画
	m_pGraph->DrawText(0, 0, m_szDebug);
}

void CScene::Draw2D()
{
	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();


	for (CBaseObj* p = m_pObj; p; p = p->GetNext())
	{
		if (p->GetID() < ID_2D)
			continue;
		p->PreDraw();
	}

	for (CBaseObj* p = m_pObj; p; p = p->GetNext())
	{
		if (p->GetID() < ID_2D)
			continue;
		p->Draw();
	}

	for (CBaseObj* p = m_pObj; p; p = p->GetNext())
	{
		if (p->GetID() < ID_2D)
			continue;
		p->PostDraw();
	}

	for (CBaseObj* p = m_pObj; p; p = p->GetNext())
	{
		if (p->GetID() < ID_2D)
			continue;
		p->DrawAlpha();
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバファ
}

void CScene::Draw3D()
{
	
}
//=======================================================================================
//	End of File
//=======================================================================================