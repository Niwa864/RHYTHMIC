#include "Stage.h"
#include "Input.h"

#define TexturePass (("../data/SCREENSHOT/screenshot.png"))

const float fUIDis = 150.0f;
const int	nMaxUI = 3;

D3DXVECTOR3 CStage::GetPos()
{
	return m_CharaStatus.pos;
}

CStage::CStage(CScene* pScene):CCharacterBase(pScene)
{
	_uID = ID_STAGE;

	m_pDevice = CGraphics::GetDevice();
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/stage.png");
	if (CInput::GetNumJoystick() > 0) {
		m_pUI[0] = new CTexture(m_pDevice, "../data/TEXTURE/UI1.png");
		m_pUI[1] = new CTexture(m_pDevice, "../data/TEXTURE/UI2.png");
		m_pUI[2] = new CTexture(m_pDevice, "../data/TEXTURE/UI3.png");
	}
	else{
		m_pUI[0] = new CTexture(m_pDevice, "../data/TEXTURE/UI11.png");
		m_pUI[1] = new CTexture(m_pDevice, "../data/TEXTURE/UI12.png");
		m_pUI[2] = new CTexture(m_pDevice, "../data/TEXTURE/UI13.png");
	}

	m_CharaStatus.pos = D3DXVECTOR3( 0.0f, 650.0f, 0.0f );
	m_CharaStatus.scale = D3DXVECTOR3( 1.0f, 1.0f, 0.0f );

	for (int i = 0; i < nMaxUI; i++)
	{
		m_UI[i].pos = { 50.0f + fUIDis * i, 700.0f - 25.0f, 0.0f };
		m_UI[i].scale = { 1.0f, 1.0f, 1.0f };

		m_pUI[i]->Init(m_UI[i].pos, m_UI[i].scale, 0.0f, 400.0f / 3, 200.0f / 3, false);
	}
	m_pTexture->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, STAGE_WIDTH, STAGE_HEIGHT, false);

	m_bPause = false;

	m_Sound = (CSound*)Find(ID_SOUND);
	m_Sound->Play(BGM_MAIN);
}


CStage::~CStage()
{
}

bool CStage::Init()
{
	return false;
}

void CStage::PreUpdate()
{
}

void CStage::Update()
{
	m_Sound->Update();
	GetKey();
}

void CStage::PostUpdate()
{
}

void CStage::PreDraw()
{
}

void CStage::Draw()
{
	m_pTexture->Draw();

	for (int i = 0; i < nMaxUI; i++) {
		m_pUI[i]->Draw();
	}
}

void CStage::DrawAlpha()
{
}

void CStage::PostDraw()
{
}

void CStage::Fin()
{
	if (m_pBackBuf != nullptr) {
		m_pBackBuf->Release();
		DeleteFile(TexturePass);
	}
	m_pTexture->Uninit();
	delete m_pTexture;

	m_Sound->Stop(BGM_MAIN);
	m_Sound = nullptr;
}

void CStage::GetKey()
{
	if ((CInput::GetKeyTrigger(DIK_ESCAPE) || CInput::GetJoyTrigger(0, 14 - 1)) && m_bPause == false)
	{// スクリーンショット
		m_pDevice->GetRenderTarget(0, &m_pBackBuf);
		D3DXSaveSurfaceToFile(TexturePass, D3DXIFF_PNG, m_pBackBuf, NULL, NULL);
		m_pFrontEff = (CFrontEffect*)Find(ID_FRONTEFF);
		m_pFrontEff->SetPauseEff(true, TexturePass);
		m_Sound->Play(SE_NEXT);

		m_bPause = true;
	}
	
	else if ((CInput::GetKeyTrigger(DIK_ESCAPE) || CInput::GetJoyTrigger(0, 14 - 1)) && m_bPause == true)
	{// リソースを消す
		m_pBackBuf->Release();
		DeleteFile(TexturePass);

		m_pFrontEff->SetPauseEff(false, TexturePass);

		m_bPause = false;
	}
}
