#include "Enemy.h"
#include "Texture.h"
#include "Graphics.h"
#include "Input.h"
#include "GameWnd.h"
#include "CharacterBase.h"
#include "Party.h"

const float fDefaultX = 800.0f;
const float fDefaultY = 0.0f;
const float fMoveDisX = 50.0f;
const int nDefaultHp = 10;
const int nDefaultAtt = 10;
const float fDistFromStage = -50.0f;
CParty* m_pParty;

CEnemy::CEnemy(CScene* pScene, int nNumber) :CCharacterBase(pScene)
{
	_uID = ID_ENEMY;													// 生成する際に利用するID　scene.h を参照

	m_pDevice = CGraphics::GetDevice();
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/Jelly.png");	// テキストコントラスタ　第二引数にテクスチャリソース

	m_nThisNumber = nNumber;
	
	m_CharaStatus.pos = D3DXVECTOR3(fDefaultX + fMoveDisX * nNumber, 0.0f, 0.0f);							// 座標などの宣言　CharcterBace.hを参照
	m_CharaStatus.scale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_CharaStatus.Hp = nDefaultHp;
	m_CharaStatus.Attack = nDefaultAtt;
	m_CharaStatus.Dead = false;
	m_pTexture->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, SPOOK_SIZE_X, SPOOK_SIZE_Y, true);
	m_pTexture->SetAnime(SPOOK_PATTERN_MAX, SPOOK_DIVIDE_X, SPOOK_DIVIDE_Y, m_nPatternNo, m_nMotion);

	m_bDamege = m_bBack = m_bMove = m_bDead = false;
	m_fDisFromStage = fDistFromStage;
	m_nFindTimer = m_nDeathTimer = 0;
	m_fTime = 0.0f;


	CCharacterBase::m_vEnemyFlag.push_back(false);
	ite = m_vEnemyFlag.end() - 1;
}

CEnemy::~CEnemy()
{
}

bool CEnemy::Init()
{
	return false;
}

void CEnemy::PreUpdate()
{

}

void CEnemy::Update()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	if (m_bDead == false) {
		DeadAnim();
		Anim();
		CollisionToStage();
	}

	if (m_CharaStatus.Dead == false) {
		GetKey();
		CollisionToPlayer();
		Attack();
		DamegeEff();
	}

	m_pTexture->Update(m_CharaStatus.pos, D3DXToRadian(m_CharaStatus.angle));

}

void CEnemy::PostUpdate()
{
}

void CEnemy::PreDraw()
{
}

void CEnemy::Draw()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	if (m_bDead == true)return;

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pTexture->Draw();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CEnemy::DrawAlpha()
{
}

void CEnemy::PostDraw()
{
}

void CEnemy::Fin()
{
	m_pTexture->Uninit();
	m_pParty = nullptr;
	m_pStage = nullptr;
	delete m_pTexture;
}

void CEnemy::GetKey()
{
	// デバッグ用

	if (CCharacterBase::m_bMoveFlag == true) {
		m_CharaStatus.pos.x -= MOVE_VALUE;

		//m_nMotion = MOTION_WALK;
	}


	/*if (CCharacterBase::m_bMoveFlag == false && m_nMotion != MOTION_WAIT)
	{
		m_nMotion = MOTION_WAIT;
	}*/
}

void CEnemy::CollisionToStage()
{
	if (m_bMove == true)return;
	// 床との当たり判定
	m_pStage = (CStage*)Find(ID_STAGE);

	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x;
	Seg.start.y = m_CharaStatus.pos.y + SPOOK_SIZE_Y;
	Seg.start.z = 0.0f;
	Seg.vec = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	Seg1.start = m_pStage->GetPos();
	Seg1.start.y += m_fDisFromStage;
	Seg1.vec = D3DXVECTOR3(STAGE_WIDTH * 3, 0.0f, 0.0f);

	// 左点の処理
	if (m_pStage && ColSegments(Seg, Seg1, &vCross)) {// テクスチャの左下頂点の当たり判定
		m_CharaStatus.pos.y = vCross.y - SPOOK_SIZE_Y;
	}
	else {
		Seg.start.x = m_CharaStatus.pos.x + SPOOK_SIZE_Y;
		if (m_pStage && ColSegments(Seg, Seg1, &vCross)) // テクスチャの右下頂点の当たり判定
			m_CharaStatus.pos.y = vCross.y - SPOOK_SIZE_Y;
		else
			m_CharaStatus.pos.y += 10.0f;		// 当たってないときの移動ベクトル
	}
}

void CEnemy::CollisionToPlayer() {
	// 敵が見つからなかったら

	if (m_nFindTimer >= 30)return;
	if (m_pParty == nullptr)
		m_pParty = (CParty*)Find(ID_PARTY);

	// 敵が死んでいたら
	while (!(m_pParty->GetDead() == false))
	{
		m_pParty = (CParty*)Find(ID_PARTY);
		m_nFindTimer++;

		if (m_nFindTimer >= 30)
			return;
	}
	if (m_pParty->GetMove() == true)return;

	// 敵との当たり判定　索敵範囲
	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x + CHARACTER_SIZE_X / 2;
	Seg.start.y = m_CharaStatus.pos.y + CHARACTER_SIZE_Y / 2;
	Seg.start.z = 0.0f;
	Seg.vec = { -500.0f, 0.0f, 0.0f };		//　線分の計算に行う場所

	Seg1.start = m_pParty->GetPos() + D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	Seg1.vec = { 0.0f, -800.0f, 0.0f };

	if (m_pParty && ColSegments(Seg, Seg1, &vCross)) {
		if (vCross == m_vCrossParty)return;
		if (m_bMove == false) {
			m_vOldPos = m_CharaStatus.pos;
			m_vCrossParty = vCross;

			m_bMove = true;
		}
	}
}

void CEnemy::Attack() 
{
	if (m_bMove == false)return;

	D3DXVECTOR3 vTemp;

	// 進む処理
	if (m_bBack == false) {
		m_vCrossParty.y = m_CharaStatus.pos.y;
		D3DXVec3Lerp(&vTemp, &m_CharaStatus.pos, &(m_vCrossParty + D3DXVECTOR3(50.0f, 0.0f, 0.0f)), m_fTime);

		//if(m_fTime == 0.0f)
		m_nMotion = MOTION_ATTACK;
		if (m_fTime >= 1.0f)
		{
			m_fTime = 0.0f;
			m_bBack = true;
			m_pParty->SetHp(&m_CharaStatus.Attack);
		}
	}
	// 戻る処理
	else if (m_bBack == true) {
		D3DXVec3Lerp(&vTemp, &m_CharaStatus.pos, &m_vOldPos, m_fTime);

		m_nMotion = MOTION_WALK;

		if (m_fTime >= 0.9f)
		{
			m_fTime = 0.0f;
			m_bBack = false;
			m_bMove = false;
		}
	}

	// Lerpの補正
	m_fTime += 0.01f;

	m_CharaStatus.pos = vTemp;
}

D3DXVECTOR3 CEnemy::GetPos()
{
	return m_CharaStatus.pos;
}

bool CEnemy::GetDead()
{
	return m_CharaStatus.Dead;
}

int CEnemy::GetNumber()
{
	return m_nThisNumber;
}

void CEnemy::SetHp(int* fDamage)
{
	m_CharaStatus.Hp -= *fDamage;
	if (m_CharaStatus.Hp <= 0) {
		m_CharaStatus.Dead = true;
		m_nMotion = MOTION_DIE;
	}
	m_bDamege = true;
}

void CEnemy::DeadAnim()
{
	if (m_CharaStatus.Dead == true)
	{
		if (m_nDeathTimer <= 30) {
			m_nDeathTimer++;
			m_fDisFromStage += 0.1f;
		}
		else
		{
			m_bDead = true;
			*ite = true;
		}
	}
}

void CEnemy::Anim() 
{
	m_nAnimeCount++;

	if ((m_nAnimeCount % INTERVAL_CHANGE_PATTERN) == 0)				// 画像切り替えタイミングの判断
	{
		m_nPatternNo = (m_nPatternNo + 1) % PATTERN_MAX;		// パターン切り替え
		m_pTexture->SetAnime(SPOOK_PATTERN_MAX, SPOOK_DIVIDE_X, SPOOK_DIVIDE_Y, m_nPatternNo, m_nMotion);
		m_nAnimeCount = 0;
	}

}

void CEnemy::DamegeEff() {

	if (m_bDamege == false)return;

#define TIMER_MAX (5)

	static bool bDamegeFlag = false;
	static int  nCounter = 0;
	static int	nTimer = 0;

	if (bDamegeFlag == false)
	{

		m_pTexture->SetColor({ 255,255,255,0 });

		nTimer++;

		if (nTimer == TIMER_MAX)
			bDamegeFlag = !bDamegeFlag;
	}
	else if (bDamegeFlag == true)
	{
		m_pTexture->SetColor({ 255,255,255,255 });

		nTimer--;

		if (nTimer == 0) {
			bDamegeFlag = !bDamegeFlag;
			nCounter++;
		}
	}

	if (nCounter > 2) {
		m_bDamege = false;
		bDamegeFlag = false;
		nCounter = 0;
		nTimer = 0;
	}

#undef TIMER_MAX

}