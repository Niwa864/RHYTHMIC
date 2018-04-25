#include "StageBG.h"
#include "GameWnd.h"
#include "Input.h"

CStageBG::CStageBG(CScene* pScene) :CCharacterBase(pScene)
{
	_uID = ID_STAGEBG;

	m_pDevice = CGraphics::GetDevice();
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/BG2.png");
	m_pWave = new CTexture(m_pDevice, "../data/TEXTURE/Wave.png");
	m_pLight[0] = new CTexture(m_pDevice, "../data/TEXTURE/Light01.png");
	m_pLight[1] = new CTexture(m_pDevice, "../data/TEXTURE/Light02.png");

	float fScaleSize;

	for (int i = 0; i < BOBBLE_NUM; i++) {
		m_pBobble[i] = new CTexture(m_pDevice, "../data/TEXTURE/bubble.png");
		m_Bobble[i].pos = { 0.0f, 650.0f, 0.0f };
		fScaleSize = (rand() % 100) / 200.0f + 0.1f;
		m_Bobble[i].scale = { fScaleSize , fScaleSize , 0.0f };
		m_pBobble[i]->Init(m_Bobble[i].pos, m_Bobble[i].scale, 0.0f, 100.0f, 100.0f, false);
		m_Bobble[i].Dead = false;
	}

	for (int i = 0; i < 2; i++)
	{
		m_Light[i].pos = { 0.0f, 0.0f, 0.0f };
		m_Light[i].scale = { 1.0f, 1.0f, 0.0 };
		m_pLight[i]->Init(m_Light[i].pos, m_Light[i].scale, 0.0f, 1238.0f, 371.0f, false);
	}


	m_Wave.pos = D3DXVECTOR3(0.0f, -410.0f, 0.0f);
	m_CharaStatus.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Wave.scale =  m_CharaStatus.scale = D3DXVECTOR3(1.0f, 2.0f, 0.0f);
	m_pTexture->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, STAGEBG_SIZE_X, STAGEBG_SIZE_Y, false);
	m_pWave->Init(m_Wave.pos, m_Wave.scale, 0.0f, 1238.0f, 371.0f, false);

}


CStageBG::~CStageBG()
{
}

bool CStageBG::Init()
{
	return false;
}

void CStageBG::PreUpdate()
{
}

void CStageBG::Update()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	GetKey();

	BobbleActive();
	BobbleMove();
	WaveScale();
}

void CStageBG::PostUpdate()
{
}

void CStageBG::PreDraw()
{
}

void CStageBG::Draw()
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	m_pTexture->Draw();

	for (int i = 0; i < BOBBLE_NUM; i++)
		m_pBobble[i]->Draw();


	m_pWave->Draw();

	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);


	for (int i = 0; i < 2; i++)
		m_pLight[i]->Draw();

	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CStageBG::DrawAlpha()
{
}

void CStageBG::PostDraw()
{
}

void CStageBG::Fin()
{
	m_pTexture->Uninit();
	delete m_pTexture;


	for (int i = 0; i < BOBBLE_NUM; i++) {
		m_pBobble[i]->Uninit();
		delete m_pBobble[i];
	}

}

void CStageBG::GetKey()
{
	if (CCharacterBase::m_bMoveFlag == true)
		m_pTexture->Scroll(MOVE_VALUE / STAGEBG_SIZE_X);
}

D3DXVECTOR3 CStageBG::GetPos()
{
	return m_CharaStatus.pos;
}


void CStageBG::BobbleActive()
{

	if (rand() % 50 != 1)return;
	for (int i = 0; i < BOBBLE_NUM; i++)
	{
		if (m_Bobble[i].Dead == true)continue;

		m_Bobble[i].Dead = true;
		m_Bobble[i].pos.x = rand() % 1200;
		break;
	}
}

void CStageBG::BobbleMove()
{
	for (int i = 0; i < BOBBLE_NUM; i++)
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

void CStageBG::WaveScale()
{
	static bool bLerp = false;
	static float fTime = 0.0f;
	static D3DXVECTOR3 Temp1 = { 1.0f, 1.0f, 0.0f };
	static D3DXVECTOR3 Temp2 = { 1.0f, 1.5f, 0.0f };

	if (bLerp == false)
	{
		D3DXVec3Lerp(&m_Wave.scale, &Temp1, &Temp2, fTime);
	}
	else if(bLerp == true)
	{

		D3DXVec3Lerp(&m_Wave.scale, &Temp2, &Temp1, fTime);
	}

	fTime += 0.01f;

	if (fTime >= 1.0f) {
		fTime = 0.0f;
		bLerp = !bLerp;
	}

	m_pWave->Update(m_Wave.pos, 0.0f, m_Wave.scale);
}