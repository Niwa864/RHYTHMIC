#include "SelectChara.h"
#include "GameWnd.h"
#include "Input.h"


const int nMaxTexture = 7;
const float fCharDis = 200.0f;


CSelectChara::CSelectChara(CScene* pScene) :CCharacterBase(pScene)
{
	_uID = ID_SELECTCHAR;
	m_pDevice = CGraphics::GetDevice();

	m_pTexture[0] = new CTexture(m_pDevice, "../data/TEXTURE/BG_Bar.png");
	m_pTexture[1] = new CTexture(m_pDevice, "../data/TEXTURE/stage.png");
	m_pTexture[2] = new CTexture(m_pDevice, "../data/TEXTURE/CharSelect.png");

	m_CharaStatus[0].pos = { 0.0f, 0.0f, 0.0f };
	m_CharaStatus[1].pos = { 0.0f, 650.0f, 0.0f };
	m_CharaStatus[2].pos = { 30.0f, 500.0f, 0.0f };

	m_pBackEff = new CBackEffect();

	for (int i = 3; i < nMaxTexture; i++) {
		m_pTexture[i] = new CTexture(m_pDevice, "../data/TEXTURE/Ghost.png");
		m_CharaStatus[i].pos = { 50.0f + fCharDis * (i - 3), 550.0f, 0.0f };
		m_CharaStatus[i].scale = {1.0f, 1.0f, 0.0f};
	}

	m_nNowChar = m_nMotion = 0;

	m_pTexture[0]->Init(m_CharaStatus[0].pos, m_CharaStatus[0].scale, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, false);
	m_pTexture[1]->Init(m_CharaStatus[1].pos, m_CharaStatus[1].scale, 0.0f, SCREEN_WIDTH, 118.0f, false);
	m_pTexture[2]->Init(m_CharaStatus[2].pos, m_CharaStatus[2].scale, 0.0f, 150.0f, 150.0f, false, {255,255,255,(int)(255.0f * 0.9)});

	for (int i = 3; i < nMaxTexture; i++) {
		m_pTexture[i]->Init(m_CharaStatus[i].pos, m_CharaStatus[i].scale, 0.0f, PARTY_SIZE_X, PARTY_SIZE_Y, true);
		m_pTexture[i]->SetAnime(PARTY_PATTERN_MAX, PARTY_DIVIDE_X, PARTY_DIVIDE_Y, m_nPatternNo, m_nMotion);
	}

	m_Sound = (CSound*)Find(ID_SOUND);
	m_Sound->Play(BGM_CHARSELECT);
	
}

CSelectChara::~CSelectChara()
{
}

bool CSelectChara::Init()
{
	return false;
}

void CSelectChara::PreUpdate()
{
}

void CSelectChara::Update()
{
	Anim();
	GetKey();
	m_Sound->Update();
	//m_pTexture[0]->Scroll(MOVE_VALUE / STAGEBG_SIZE_X);

	for (int i = 2; i < nMaxTexture; i++)
		m_pTexture[i]->Update(m_CharaStatus[i].pos, D3DXToRadian(m_CharaStatus[i].angle));
}

void CSelectChara::PostUpdate()
{
}

void CSelectChara::PreDraw()
{
}

void CSelectChara::Draw()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (int i = 0; i < 2; i++)
		m_pTexture[i]->Draw();
	//m_pBackEff->Draw();
	for (int i = 2; i < nMaxTexture; i++)
		m_pTexture[i]->Draw();
}

void CSelectChara::DrawAlpha()
{
}

void CSelectChara::PostDraw()
{
}

void CSelectChara::Fin()
{
	for (int i = 0; i < nMaxTexture; i++) {
		m_pTexture[i]->Uninit();
		delete m_pTexture[i];
	}
	m_Sound->Stop(BGM_CHARSELECT);
	m_Sound = nullptr;
}

void CSelectChara::GetKey()
{
	if (CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetJoyTrigger(0, 3 - 1)) {
		m_FrontEff = (CFrontEffect*)Find(ID_FRONTEFF);
		m_FrontEff->SetFade(GAME_MAIN);
		m_Sound->Play(SE_NEXT);
	}
	if ((CInput::GetKeyTrigger(DIK_RIGHT)/* || CInput::GetJoyAxis(0, JOY_X)*/) && m_nNowChar < 3) {
		m_CharaStatus[2].pos.x += 200.0f;
		m_nNowChar++;
	}
	else if ((CInput::GetKeyTrigger(DIK_LEFT)/* || CInput::GetJoyAxis(0, JOY_X)*/) && m_nNowChar > 0) {
		m_CharaStatus[2].pos.x -= 200.0f;
		m_nNowChar--;
	}
}

void CSelectChara::Anim()
{
	m_nAnimeCount++;

		// to:do アニメーションをずらす
	if ((m_nAnimeCount % INTERVAL_CHANGE_PATTERN) == 0)				// 画像切り替えタイミングの判断
	{
		m_nPatternNo = (m_nPatternNo + 1) % PATTERN_MAX;		// パターン切り替え

		for (int i = 2; i < nMaxTexture; i++)
			m_pTexture[i]->SetAnime(PARTY_PATTERN_MAX, PARTY_DIVIDE_X, PARTY_DIVIDE_Y, m_nPatternNo, m_nMotion);
		m_nAnimeCount = 0;
	}
}
