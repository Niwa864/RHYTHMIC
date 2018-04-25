#include "FrontEffect.h"
#include "Graphics.h"
#include "GameWnd.h"
#include <math.h>

struct Vtx {
	float x, y, z, u, v;
};

static float w = SCREEN_WIDTH;
static float h = SCREEN_HEIGHT;

// スクリーン座標系を想定したローカル座標
Vtx vtx[4] = {
	{ 0.0f,   0.0f, 0.0f, 0.0f, 0.0f },  // UV追加
	{ SCREEN_WIDTH,   0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f },
	{ SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, 1.0f }
};

// 2D描画用射影変換行列
D3DXMATRIX proj(
	2/w, 0.0f, 0.0f, 0.0f,
	0.0f,-2/h, 0.0f, 0.0f,
	0.0f, 0.0f,1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f,1.0f
);

// 頂点宣言作成
static IDirect3DVertexDeclaration9 *decl = 0;

ID3DXEffect *effect = 0;
ID3DXBuffer *error = 0;

enum EffectMode {
	Noise = 0,
	Fade,
	Mosaic,
	Combo,
};

CFrontEffect::CFrontEffect(CScene* pScene) :CCharacterBase(pScene)
{
	_uID = ID_FRONTEFF;

	m_pDevice = CGraphics::GetDevice();
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/test.png");

	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	// シェーダ作成
	if (FAILED(D3DXCreateEffectFromFile(m_pDevice, _T("../data/SHADER/FrontEffect.fx"), 0, 0, 0, 0, &effect, &error))) {
		OutputDebugStringA((const char*)error->GetBufferPointer());
		//return 0;
	}

	D3DVERTEXELEMENT9 elems[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(float) * 3, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(elems, &decl);

	m_nPassId = 1;
	m_nBossEffTimer = 0;
	fAlpha = 0.0f;
	bFadeStart = fFadeOut = false;

}

CFrontEffect::~CFrontEffect()
{
}

bool CFrontEffect::Init()
{
	return false;
}

void CFrontEffect::PreUpdate()
{
}

void CFrontEffect::Update()
{
}

void CFrontEffect::PostUpdate()
{
}

void CFrontEffect::PreDraw()
{
}

void CFrontEffect::Draw()
{
	if (bFadeStart == false)return;
	UINT numPass = 0;

	effect->SetTechnique("Tech");
	effect->Begin(&numPass, 0);
	effect->BeginPass(m_nPassId);

	if (m_nPassId == Noise) { // 半透明ノイズ
		fAlpha = 0.5f;
		effect->SetFloat("time",rand());
	}

	if (m_nPassId == Noise || m_nPassId == Fade) {
		effect->SetTexture("tex", m_pTexture->GetTexP());
	}
	else if(m_nPassId == Mosaic)
	{
		effect->SetTexture("tex", m_Temp->GetTexP());
		effect->SetFloat("AddU", 0.009f);
		effect->SetFloat("AddV", 0.009f);
	}

	effect->SetMatrix("proj", &proj);
	effect->SetFloat("alpha", fAlpha);
	effect->SetFloat("screenW", w);
	effect->SetFloat("screenH", h);
	m_pDevice->SetVertexDeclaration(decl);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(Vtx));

	effect->EndPass();
	effect->End();


	if (m_nPassId == Noise) 
	{
		m_nBossEffTimer++;
		if (m_nBossEffTimer <= 60) {
			//m_nBossEffTimer = 0;
			//bFadeStart = false;
		}
		return;
	}

	if (fFadeOut == false && m_nPassId == Fade)
	{
		fAlpha += 0.01f;
		if (fAlpha >= 1.0f) {
			fFadeOut = true;
			CScene::SetGameMode(m_GameMode);
		}
	}
	else if (fFadeOut == true && m_nPassId == Fade) {
		fAlpha -= 0.01f;
		if (fAlpha <= 0.0f) {
			fFadeOut = false;
			bFadeStart = false;
		}
	}
}

void CFrontEffect::DrawAlpha()
{
}

void CFrontEffect::PostDraw()
{
}

void CFrontEffect::Fin()
{
}

void CFrontEffect::GetKey()
{
}

void CFrontEffect::SetFade(GAME_MODE nGameMode)
{
	if (bFadeStart == true)return;
	m_GameMode = nGameMode;
	m_nPassId = Fade;
	bFadeStart = true;
}

void CFrontEffect::SetBossEff(bool flag)
{
	bFadeStart = flag;
	m_nPassId = Noise;
}

void CFrontEffect::SetPauseEff(bool flag, char* cPass) {

	m_Temp = new CTexture(m_pDevice, cPass);

	bFadeStart = flag;
	m_nPassId = Mosaic;
}

void CFrontEffect::SetCombo(bool bFlag)
{
	if (bFadeStart == true && (m_nPassId == Noise || m_nPassId == Fade))return;
	bFadeStart = bFlag;
	m_nPassId = Combo;
}

bool CFrontEffect::GetPauseEff()
{
	if (m_nPassId == Mosaic && bFadeStart == true)
		return true;
	return false;
}
