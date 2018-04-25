#include "Select.h"
#include "GameWnd.h"
#include "Input.h"

const int nMaxTexture = 1;
const int nMaxDot = 6;
const float fFinalXFirst = 0.15f;
const float fFinalYFirst = 0.284f;
const float fFinalXLast = 0.495f;
const float fFinalYLast = 0.64f;
const float fFinalTime = 1.0f;

CSelect::CSelect(CScene* pScene) :CCharacterBase(pScene)
{
	std::string str1(".png");

	_uID = ID_SELECT;

	m_pDevice = CGraphics::GetDevice();
	m_pTexture[0] = new CTexture(m_pDevice, "../data/TEXTURE/StageSelect.png");
	m_pCross = new CTexture(m_pDevice ,"../data/TEXTURE/Dot/Cross.png");
	m_CharaStatus[0].pos = { 0.0f, -130.0f, 0.0f };
	m_Cross.pos = { 1200.0f / 3, 800.0f / 3, 0.0f };

	for (int i = 1; i < nMaxDot + 1; i++)
	{
		std::string str("../data/TEXTURE/Dot/Dot");

		str.push_back(i + '0');
		std::string str2 = str + str1;
		char *p = new char[str2.size() + 1];
		std::char_traits<char>::copy(p, str2.c_str(), str2.size() + 1);

		m_pDot[i - 1] = new CTexture(m_pDevice, p);
		m_Dot[i - 1].pos = {840.0f / 3, 930.0f / 3, 0.0f};
		m_Dot[i - 1].Dead = false;

		delete[] p;
	}

	for (int i = 0; i < nMaxTexture ; i++)
		m_CharaStatus[i].scale = { 1.0f, 1.0f, 0.0f };

	m_Cross.scale = { 1.0f, 1.0f, 0.0f };
	m_Cross.Dead = false;

	for (int i = 0; i < nMaxDot; i++) {
		m_Dot[i].scale = { 1.0f, 1.0f, 0.0f };
		m_pDot[i]->Init(m_Dot[i].pos, m_Dot[i].scale, 0.0f, 181.0f, 86.0f, false);
	}

	m_pTexture[0]->Init(m_CharaStatus[0].pos, m_CharaStatus[0].scale, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, false);
	m_pCross->Init(m_Cross.pos, m_Cross.scale, 0.0f, 156.0f, 185.0f, false);

	m_bCrossFade = m_bDotFlag = false;

	m_fDotTimer = fFinalTime;
	m_fTime = 0.0f;

	m_nFade = 255;

	m_Sound = (CSound*)Find(ID_SOUND);
	m_Sound->Play(BGM_SELECT);
}

CSelect::~CSelect()
{
}

bool CSelect::Init()
{
	return false;
}

void CSelect::PreUpdate()
{
}

void CSelect::Update()
{
	GetKey();
	LerpOffset();
	DotShow();
	CrossShow();
	m_Sound->Update();
	m_pTexture[0]->SetTexOffSet(Offset.firstX, Offset.firstY, Offset.lastX, Offset.lastY);
	m_pTexture[0]->Update(m_CharaStatus[0].pos, D3DXToRadian(m_CharaStatus[0].angle));
}

void CSelect::PostUpdate()
{
}

void CSelect::PreDraw()
{
}

void CSelect::Draw()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (int i = 0; i < nMaxTexture; i++)
		m_pTexture[0]->Draw();

	for (int i = 0; i < nMaxDot - 1; i++) {
		if (m_Dot[i].Dead == false)continue;
		m_pDot[i]->Draw();
	}

	if(m_Cross.Dead == true)
		m_pCross->Draw();
}

void CSelect::DrawAlpha()
{
}

void CSelect::PostDraw()
{
}

void CSelect::Fin()
{
	for (int i = 0; i < nMaxTexture; i++) {
		m_pTexture[i]->Uninit();
		delete m_pTexture[i];
	}

	m_Sound->Stop(BGM_SELECT);
	m_Sound = nullptr;
}

void CSelect::GetKey()
{
	if (m_Cross.Dead == false)return;

	if (CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetJoyTrigger(0, 3 - 1)) {
		m_FrontEff = (CFrontEffect*)Find(ID_FRONTEFF);
		m_FrontEff->SetFade(GAME_CHARSELECT);
		m_Sound->Play(SE_NEXT);
	}
}

void CSelect::LerpOffset()
{
	if (m_fTime >= 1.0f) { 
		m_bDotFlag = true;
		return; 
	}
	// アイリスイン
	Offset.firstX = (fFinalXFirst) * (m_fTime);
	Offset.firstY = (fFinalYFirst) * (m_fTime);
	Offset.lastX = 1.0f - (1.0f - fFinalXLast) * (m_fTime);
	Offset.lastY = 1.0f - (1.0f - fFinalYLast) * (m_fTime);
	m_fTime += 1.0f / 200;
}

void CSelect::DotShow()
{
	if (m_bDotFlag == false)return;

	m_fDotTimer += 0.1f;

	if (m_fDotTimer >= fFinalTime)
	{
		for (int i = 0; i < nMaxDot - 1; i++) {
			if (m_Dot[i].Dead == true)continue;

			m_Dot[i].Dead = true;
			break;
		}
		m_fDotTimer = 0.0f;
	}
}

void CSelect::CrossShow() {
	if (m_Cross.Dead == true) {
		if (m_bCrossFade == false)
		{
			m_nFade-= 3;

			if (m_nFade <= 100)
				m_bCrossFade = true;
		}
		else if (m_bCrossFade == true) 
		{
			m_nFade+=3;

			if (m_nFade >= 255)
				m_bCrossFade = false;
		}

		m_pCross->SetColor({255,255,255, m_nFade});
	}
	if (m_Dot[nMaxDot - 2].Dead == false)return;
	m_Cross.Dead = true;
}