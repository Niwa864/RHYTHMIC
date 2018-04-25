#include "Title.h"
#include "GameWnd.h"
#include "Input.h"
#include "define.h"

const float fDeflautTitleX = SCREEN_WIDTH / 8;
const float fDeflautEnterX = fDeflautTitleX + 200.0f;
const float fDeflautTitleY = 200.0f;
const float fDeflautEnterY = 500.0f;
const int   nMaxTexture = 4;

CTitle::CTitle(CScene* pScene) :CCharacterBase(pScene)
{
	_uID = ID_TITLE;

	m_pDevice = CGraphics::GetDevice();
	m_pTexture[0] = new CTexture(m_pDevice, "../data/TEXTURE/BG.png");
	m_pTexture[1] = new CTexture(m_pDevice, "../data/TEXTURE/title.png");
	m_pTexture[2] = new CTexture(m_pDevice, "../data/TEXTURE/PressSpace.png");
	m_pTexture[3] = new CTexture(m_pDevice, "../data/TEXTURE/stage.png");

	float fScaleSize;

	for (int i = 0; i < BOBBLE_MAX; i++) {
		m_pBobble[i] = new CTexture(m_pDevice, "../data/TEXTURE/bubble.png");
		m_Bobble[i].pos = { 0.0f, 650.0f, 0.0f };
		fScaleSize = (rand() % 100) / 200.0f + 0.1f;
		m_Bobble[i].scale = { fScaleSize , fScaleSize , 0.0f};
		m_pBobble[i]->Init(m_Bobble[i].pos, m_Bobble[i].scale, 0.0f, 100.0f, 100.0f, false);
		m_Bobble[i].Dead = false;
	}

	m_CharaStatus[0].pos = {0.0f, 0.0f, 0.0f};
	m_CharaStatus[1].pos = { fDeflautTitleX, fDeflautTitleY, 0.0f };
	m_CharaStatus[2].pos = { fDeflautEnterX, fDeflautEnterY, 0.0f };
	m_CharaStatus[3].pos = { 0.0f, 650.0f, 0.0f };

	for( int i = 0; i < nMaxTexture; i++)
		m_CharaStatus[i].scale = { 1.0f, 1.0f, 1.0f };

	m_pTexture[0]->Init(m_CharaStatus[0].pos, m_CharaStatus[0].scale, 0.0f, SCREEN_WIDTH * 2, SCREEN_HEIGHT, false);
	m_pTexture[1]->Init(m_CharaStatus[1].pos, m_CharaStatus[1].scale, 0.0f, 720.0f, 180.0f, false);
	m_pTexture[2]->Init(m_CharaStatus[2].pos, m_CharaStatus[2].scale, 0.0f, 340.0f, 30.0f, false);
	m_pTexture[3]->Init(m_CharaStatus[3].pos, m_CharaStatus[3].scale, 0.0f, 1024.0f, 118.0f, false);

	m_Sound = (CSound*)Find(ID_SOUND);
	m_Sound->Play(BGM_TITLE);
}


CTitle::~CTitle()
{
}

bool CTitle::Init()
{
	return false;
}

void CTitle::PreUpdate()
{
}

void CTitle::Update()
{
	GetKey();
	m_Sound->Update();
	BobbleActive();
	BobbleMove();
	m_pTexture[0]->Scroll(MOVE_VALUE / STAGEBG_SIZE_X);
}

void CTitle::PostUpdate()
{
}

void CTitle::PreDraw()
{
}

void CTitle::Draw()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pTexture[0]->Draw();

	for (int i = 0; i < BOBBLE_MAX; i++)
		m_pBobble[i]->Draw();

	for (int i = 1; i < nMaxTexture; i++)
		m_pTexture[i]->Draw();

}

void CTitle::DrawAlpha()
{
}

void CTitle::PostDraw()
{
}

void CTitle::Fin()
{
	for (int i = 0; i < nMaxTexture; i++) {
		m_pTexture[i]->Uninit();
		delete m_pTexture[i];
	}

	for (int i = 0; i < BOBBLE_MAX; i++) {
		m_pBobble[i]->Uninit();
		delete m_pBobble[i];
	}

	m_Sound->Stop(BGM_TITLE);
	m_Sound = nullptr;
}


void CTitle::GetKey()
{
	if (CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetJoyTrigger(0, 3 - 1)) // to:do フェードイン/アウト
	{
		CFrontEff = (CFrontEffect*)Find(ID_FRONTEFF);
		m_Sound->Play(SE_NEXT);
		CFrontEff->SetFade(GAME_SELECT);
	}
}

void CTitle::BobbleActive() 
{

	if (rand() % 20 != 1)return;
	for (int i = 0; i < BOBBLE_MAX; i++)
	{
		if (m_Bobble[i].Dead == true)continue;

		m_Bobble[i].Dead = true;
		m_Bobble[i].pos.x = rand() % 1200;
		break;
	}
}

void CTitle::BobbleMove()
{
	for (int i = 0; i < BOBBLE_MAX; i++)
	{
		if (m_Bobble[i].Dead == false)continue;

		float temp = m_Bobble[i].pos.y;
		while (temp > 360)
		{
			temp -= 360.0f;
		}
		m_Bobble[i].pos.x += sinf(D3DXToRadian(temp));
		m_Bobble[i].pos.y -= 1.0f + (m_Bobble[i].scale.x);

		if (m_Bobble[i].pos.y <= -100.0f) {
			m_Bobble[i].pos.y = 650.0f;
			m_Bobble[i].Dead = false;
		}

		m_pBobble[i]->Update(m_Bobble[i].pos, 0.0f, m_Bobble[i].scale);
	}
}