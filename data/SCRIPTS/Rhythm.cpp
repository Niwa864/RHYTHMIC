#include "Rhythm.h"
#include "Input.h"
#include "Boss.h"

typedef enum {
	RHYTHM_1 = 1,
	RHYTHM_2 = 2,
}RHYTHM_TYPE;

const std::vector<int> RhythmType1 = { RHYTHM_1, RHYTHM_1, RHYTHM_1, RHYTHM_2};		// 前進
const std::vector<int> RhythmType2 = { RHYTHM_2, RHYTHM_2, RHYTHM_1, RHYTHM_2};		// 攻撃

const float fFeverY = 100.0f;

CRhythm::CRhythm(CScene* pScene) :CCharacterBase(pScene)
{
	_uID = ID_RHYTHM;

	m_pDevice = CGraphics::GetDevice();
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/Note.png");
	m_pPostEff = new CTexture(m_pDevice, "../data/TEXTURE/FrontEffect.png");
	m_pNumber = new CTexture(m_pDevice, "../data/TEXTURE/combo/Number.png");
	m_pCombo = new CTexture(m_pDevice, "../data/TEXTURE/combo/combo.png");

	for (int i = 0; i < 3; i++)
	{
		std::string str1("../data/TEXTURE/combo/fever");
		std::string str2(".png");
		str1.push_back((i + 1) + '0');
		str1 += str2;
		char *p = new char[str1.size() + 1];
		std::char_traits<char>::copy(p, str1.c_str(), str1.size() + 1);
		m_pFever[i] = new CTexture(m_pDevice, p);
	}

	for (int i = 0; i < 4; i++) {
		if (CInput::GetNumJoystick() > 0)
			m_pRhythm[i] = new CTexture(m_pDevice, "../data/TEXTURE/RhyTest1.png");
		else
			m_pRhythm[i] = new CTexture(m_pDevice, "../data/TEXTURE/RhyTest.png");
		m_RhyStatus[i].pos = { (600.0f / 2) + 100 * i, 100.0f, 0.0f };
		m_RhyStatus[i].scale = { 1.0f, 1.0f, 0.0f };
		m_RhyStatus[i].Dead = false;
		m_pRhythm[i]->Init(m_RhyStatus[i].pos, m_RhyStatus[i].scale, 0.0f, 100.0f, 100.0f, false, { 255,255,255,255 }, 0.0f, 0.5f);
	}
	m_pChar = (CCharacter*)Find(ID_CHARACTER);

	m_PostEff.pos = D3DXVECTOR3(0.0f, -130.0f, 0.0f);
	m_CharaStatus.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Number.pos = { 50.0f, 200.0f, 0.0f};
	m_Combo.pos = { 150.0f, 170.0f, 0.0f};

	for(int i = 0; i < 3; i++)
		m_Fever[i].pos = { 100.0f, fFeverY, 0.0f };
	m_Combo.scale = m_Number.scale = m_PostEff.scale =  m_CharaStatus.scale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_pTexture->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, 100.0f, 75.0f, false);
	m_pPostEff->Init(m_PostEff.pos, m_PostEff.scale, 0.0f, 1024.0f, 768.0f, false);
	m_pNumber->Init(m_Number.pos, m_Number.scale, 0.0f, 500.0f / 5, 100.0f, false);
	m_pCombo->Init(m_Combo.pos, m_Combo.scale, 0.0f, 300.0f * 0.7, 100.0f * 0.7, false);

	for (int i = 0; i < 3; i++)
		m_pFever[i]->Init(m_Fever[i].pos, m_Fever[i].scale, 0.0f, 300.0f, 100.0f, false);

	m_bGetKey = m_bCollect = false;
	m_vRhythmArray.clear();
	m_nNowCombo = m_nRhythmTimer = m_nNowRhythm = m_nTimer = 0;
	m_nComboFade = m_fFade = 255;

	m_pSound = (CSound*)Find(ID_SOUND);
	m_pSound->SetVolume(SE_MARACAS2, 1.0f);
	m_pFrontEff = (CFrontEffect*)Find(ID_FRONTEFF);
}


CRhythm::~CRhythm()
{
}

bool CRhythm::Init()
{
	return false;
}

void CRhythm::PreUpdate()
{
}

void CRhythm::Update()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	RhythmTimer();
	Combo();
	Fever();

	for (int i = 0; i < 4; i++) {
		m_pRhythm[i]->Update(m_RhyStatus[i].pos, 0.0f);
	}

	m_pPostEff->Update(m_PostEff.pos, 0.0f);

	if (m_bGetKey == false) 
	{
		GetKey(); 
		return; 
	}

	int i = 0;

	// リズム検査
	for (std::vector<int>::iterator ite = m_vRhythmArray.begin(); ite < m_vRhythmArray.end(); ite++, i++) {
		if (ite == m_vRhythmArray.end())
			break;
		if (*ite != RhythmType1[i])
		{// タイプ１アウト
			if (*ite != RhythmType2[i]) { // タイプ２アウト
				m_vRhythmArray.clear();

				// ミスった処理
				m_nNowRhythm = 0;
				for (int i = 0; i < 4; i++)
					m_RhyStatus[i].Dead = false;

				m_pSound->SetVolume(SE_MARACAS2, 1.0f);

				m_nNowCombo = 0;
				break;
			}
		}
	}

	// タイプ１成功
	if (m_vRhythmArray == RhythmType1) {
		CCharacterBase::m_bMoveFlag = true;
		m_vRhythmArray.clear();
		 
		m_nNowCombo++; 
		m_nComboFade = 255;
		//m_pFrontEff->SetCombo(true);
	}
	// タイプ２成功
	else if (m_vRhythmArray == RhythmType2)
	{
		for (std::vector<bool>::iterator ite = CCharacterBase::m_vFlagVector.begin();
			ite < CCharacterBase::m_vFlagVector.end(); ite++)
		{// 存在するパーティの行動フラグをon
			*ite = true;
		}
		m_vRhythmArray.clear();
		//m_pFrontEff->SetCombo(true);

		m_nNowCombo++;
		m_nComboFade = 255;
	}

	// 音符の位置
	m_CharaStatus.pos = m_pChar->GetPos() + D3DXVECTOR3(50.0f, -80.0f, 0.0f);
	m_pTexture->Update(m_CharaStatus.pos, m_CharaStatus.angle);

	// 表示タイムの処理
	m_nTimer++;
	if (m_nTimer >= 28) {
		m_bGetKey = false;
		if (CCharacterBase::m_bMoveFlag == true) {
			// 動きを止める
			CCharacterBase::m_bMoveFlag = false;
		}
		if (m_nNowRhythm >= 4) {
			m_nNowRhythm = 0;
			for (int i = 0; i < 4; i++)
				m_RhyStatus[i].Dead = false;

			m_pSound->SetVolume(SE_MARACAS2, 1.0f);
			m_pFrontEff->SetCombo(false);
		}
	}
}

void CRhythm::PostUpdate()
{
}

void CRhythm::PreDraw()
{
}

void CRhythm::Draw()
{

	if (((CBoss*)Find(ID_BOSS))->GetDead() == true)
	{
		return;
	}

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (int i = 0; i < 4; i++) {
		if (m_RhyStatus[i].Dead == false)continue;
		m_pRhythm[i]->Draw();
	}


	m_pPostEff->Draw();


	if (m_bGetKey == true) {
		m_pTexture->Draw();
	}


	if (m_nNowCombo > 0 && m_nNowCombo < 6) {
		m_pNumber->Draw();
		m_pCombo->Draw();
	}
	else if(m_nNowCombo >= 6)
	{
		for (int i = 0; i < 3; i++)
			m_pFever[i]->Draw();
	}

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

void CRhythm::DrawAlpha()
{
}

void CRhythm::PostDraw()
{
}

void CRhythm::Fin()
{
	m_pTexture->Uninit();
	m_pChar = nullptr;
	delete m_pTexture;
}
 
void CRhythm::GetKey()
{
	if (CInput::GetKeyTrigger(DIK_A) || CInput::GetJoyTrigger(0, 3 - 1)) 
	{
		m_bGetKey = true;
		m_vRhythmArray.push_back(RHYTHM_1);
		m_nTimer = 0;
		m_pSound->SetVolume(SE_MARACAS2, 0.0f);
		m_pSound->Play(SE_MARACAS1);
		m_RhyStatus[m_nNowRhythm].Dead = true;
		m_pRhythm[m_nNowRhythm]->SetTexOffSet(0.0f, 0.0f, 0.5f, 1.0f);
		m_pRhythm[m_nNowRhythm]->SetColor({255,0,0,255});
		m_nNowRhythm++;

	}

	if(CInput::GetKeyTrigger(DIK_D) || CInput::GetJoyTrigger(0, 1 - 1))
	{
		m_bGetKey = true;
		m_vRhythmArray.push_back(RHYTHM_2);
		m_nTimer = 0;
		m_pSound->SetVolume(SE_MARACAS2, 0.0f);
		m_pSound->Play(SE_MARACAS1);
		m_RhyStatus[m_nNowRhythm].Dead = true;
		m_pRhythm[m_nNowRhythm]->SetTexOffSet(0.5f, 0.0f, 1.0f, 1.0f);
		m_pRhythm[m_nNowRhythm]->SetColor({ 255,128,255,255 });
		m_nNowRhythm++;
	}
}

void CRhythm::RhythmTimer()
{
	static float mScale = 1.5f;
	static int nCount = 0;
	if (m_nRhythmTimer == 0)
	{
		mScale = 1.5f;
		m_pSound->Play(SE_MARACAS2);
		m_fFade = 255;
		m_Fever[nCount].Dead = true;
	}
	else if (m_nRhythmTimer >= 28) {

		m_nRhythmTimer = 0;
		//m_Fever[nCount].Dead = false;
		nCount = (nCount + 1 == 3 ? 0 : nCount + 1);
		return;
	}
	mScale -= 0.01f;
	m_fFade -= 10;
	m_Number.scale = { mScale, mScale, 0.0f };
	m_pPostEff->SetColor({ 255,255,255, m_fFade });
	m_nRhythmTimer++;
}

void CRhythm::Combo()
{
	static int nTemp = 0;

	if (m_nNowCombo <= 0)return;
	//if (nTemp == m_nNowCombo)return;

	if (m_nNowCombo < 6) {
		nTemp = m_nNowCombo;

		m_pNumber->SetTexOffSet(0.2f * (m_nNowCombo - 1), 0.0f, (0.2f * (m_nNowCombo - 1)) + 0.2f);
		m_pNumber->SetColor({ 255,255,255, m_nComboFade });
		m_pNumber->Update(m_Number.pos, 0.0f, m_Number.scale);
		m_pCombo->SetColor({ 255, 255, 255, m_nComboFade });
		m_pCombo->Update(m_Combo.pos, 0.0f);
		m_nComboFade -= 1;

		if (m_nComboFade <= 30)
			m_nNowCombo = 0;
	}
}

void CRhythm::Fever()
{
	if (m_nNowCombo < 6)return;

	//static bool bJump[3] = { true, false, false};

	for (int i = 0; i < 3; i++)
	{
		/*if (bJump[i] == false)
		{
			if (m_Fever[i].pos.y < fFeverY)
				m_Fever[i].pos.y -= 0.5f;
		}
		else if(bJump[i] == true)
		{
			if (m_Fever[i].pos.y > 70.0f)
				m_Fever[i].pos.y -= 2.0f;
			else
			{
				bJump[i] = false;
				bJump[(i + 1) == 3 ? 0 : i + 1] = true;
			}
		}*/

		if (m_Fever[i].Dead == true)
		{
			if (m_Fever[i].pos.y > fFeverY - 30.0f)
				m_Fever[i].pos.y -= 2.0f;
			else
				m_Fever[i].Dead = false;
			//if (m_Fever[i].pos.y < fFeverY - 50.0f)
				//m_Fever[i].Dead = false;
		}
		if (m_Fever[i].Dead == false)
		{
			if(m_Fever[i].pos.y < fFeverY)
				m_Fever[i].pos.y += 1.0f;
			//if (m_Fever[i].pos.y > fFeverY)
				//m_Fever[i].pos.y = fFeverY;
		}

		m_pFever[i]->Update(m_Fever[i].pos, 0.0f);
	}
}