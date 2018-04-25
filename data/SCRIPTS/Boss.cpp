#include "Boss.h"
#include "Texture.h"
#include "Graphics.h"
#include "Input.h"
#include "GameWnd.h"
#include "Party.h"

const int nDefaultHp = 20;
const int nDefaultAtt = 10;

CBoss::CBoss(CScene* pScene) :CCharacterBase(pScene)
{
	_uID = ID_BOSS;													// 生成する際に利用するID　scene.h を参照

	m_pDevice = CGraphics::GetDevice();
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/Alon.png");	// テキストコントラスタ　第二引数にテクスチャリソース
	m_pStageClear = new CTexture(m_pDevice, "../data/TEXTURE/STAGECLEAR.png");

	m_CharaStatus.pos = D3DXVECTOR3(SCREEN_WIDTH + 100.0f, 0.0f, 0.0f);							// 座標などの宣言　CharcterBace.hを参照
	m_StageClear.pos = D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 0.0f);
	m_StageClear.scale = m_CharaStatus.scale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_CharaStatus.Hp = nDefaultHp;
	m_CharaStatus.Attack = nDefaultAtt;
	m_pTexture->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, BOSS_SIZE_X, BOSS_SIZE_Y, true); // アニメ有りになおすこと
	m_pStageClear->Init(m_StageClear.pos, m_StageClear.scale, 0.0f, 500.0f, 100.0f, false);
	m_nMotion = MOTION_WALK;
	m_pTexture->SetAnime(BOSS_PATTERN_MAX, BOSS_DIVIDE_X, BOSS_DIVIDE_Y, m_nPatternNo, m_nMotion);

	m_StageClear.Dead = false;

	m_bDead = m_CharaStatus.Dead = m_bAttackAnim = m_bBack = m_bMove = m_bGoToPos = m_bActive = false;
	m_nDeathTimer = m_nAnimeCount = m_nFindTimer = 0;
	m_fTime = 0.0f;
}


CBoss::~CBoss()
{
}

bool CBoss::Init()
{
	return false;
}

void CBoss::PreUpdate()
{

}

void CBoss::Update()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	GameClear();
	Awack();
	Anim();
	DeadAnim();
	CollisionToStage();
	if (m_CharaStatus.Dead == true)return;
	AttackAnim();
	CollisionToPlayer();
	Attack();
	DamegeEff();

	m_pTexture->Update(m_CharaStatus.pos, D3DXToRadian(m_CharaStatus.angle));

}
void CBoss::PostUpdate()
{
}

void CBoss::PreDraw()
{
}

void CBoss::Draw()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	
	if (m_bActive == false) return;

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (m_bDead == false) {
		m_pTexture->Draw();
	}

	if (m_StageClear.Dead == true)
	{
		m_pStageClear->Draw();
	}

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CBoss::DrawAlpha()
{
}

void CBoss::PostDraw()
{
}

void CBoss::Fin()
{
	m_pTexture->Uninit();
	m_pFrontEff = nullptr;
	m_pParty = nullptr;
	m_pStage = nullptr;
	delete m_pTexture;
}

void CBoss::GetKey()
{
}

void CBoss::GameClear()
{
	if (m_StageClear.Dead == false)return;

	m_StageClear.pos.y -= 1.0f;

	if(m_StageClear.pos.y <= -100.0f)
		m_pFrontEff->SetFade(GAME_TITLE);

	m_pStageClear->Update(m_StageClear.pos, 0.0f);
}

void CBoss::CollisionToStage()
{
	m_pStage = (CStage*)Find(ID_STAGE);

	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x;
	Seg.start.y = m_CharaStatus.pos.y + BOSS_SIZE_Y + 125.0f;
	Seg.start.z = 0.0f;
	Seg.vec = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	Seg1.start = m_pStage->GetPos();
	Seg1.vec = D3DXVECTOR3(STAGE_WIDTH * 2, 0.0f, 0.0f);

	// 左点の処理
	if (m_pStage && ColSegments(Seg, Seg1, &vCross)) {// テクスチャの左下頂点の当たり判定
		m_CharaStatus.pos.y = vCross.y - BOSS_SIZE_Y - 125.0f;
	}
	else {
		Seg.start.x = m_CharaStatus.pos.x + BOSS_SIZE_X;
		if (m_pStage && ColSegments(Seg, Seg1, &vCross)) // テクスチャの右下頂点の当たり判定
			m_CharaStatus.pos.y = vCross.y - BOSS_SIZE_Y - 125.0f;
		else
			m_CharaStatus.pos.y += 10.0f;		// 当たってないときの移動ベクトル
	}
}

void CBoss::CollisionToPlayer() {
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
	Seg.vec = { -700.0f, 0.0f, 0.0f };		//　線分の計算に行う場所

	Seg1.start = m_pParty->GetPos() + D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	Seg1.vec = { 0.0f, -800.0f, 0.0f };

	if (m_pParty && ColSegments(Seg, Seg1, &vCross)) {
		if (vCross == m_vCrossParty)return;
		if (m_bMove == false && IdolAnim() == true) {
			m_vOldPos = m_CharaStatus.pos;
			m_vCrossParty = vCross;

			m_bMove = true;
		}
	}
}

void CBoss::Attack()
{

	if (m_bMove == false)return;

	static D3DXVECTOR3 vTemp;

	// 進む処理
	if (m_bBack == false) {
		m_vCrossParty.y = m_CharaStatus.pos.y;
		if (m_fTime == 0.0f)
			vTemp = m_CharaStatus.pos;
		D3DXVec3Lerp(&m_CharaStatus.pos, &vTemp, &(m_vCrossParty + D3DXVECTOR3(50.0f, 0.0f, 0.0f)), m_fTime);

		if(m_nMotion != MOTION_WALK)
			m_nMotion = MOTION_WALK;

		// Lerpの補正
		m_fTime += 0.01f;
		
		if (m_fTime >= 1.0f)
		{
			m_fTime = 0.0f;
			m_bBack = true;
			m_bAttackAnim = true;
			m_nMotion = MOTION_ATTACK;
		}
	}
	// 戻る処理
	if (m_bBack == true && m_bAttackAnim == false) {
		if (m_fTime == 0.0f)
			vTemp = m_CharaStatus.pos;
		D3DXVec3Lerp(&m_CharaStatus.pos, &vTemp, &m_vOldPos, m_fTime);

		if (m_nMotion != MOTION_WALK)
			m_nMotion = MOTION_WALK;

		// Lerpの補正
		m_fTime += 0.01f;

		if (m_fTime >= 1.0f)
		{
			m_fTime = 0.0f;

			m_nMotion = MOTION_IDOL;
			m_bBack = false;
			m_bMove = false;
		}
	}
}

bool CBoss::IdolAnim() {
	static int nCount;
	static int	nRand = rand() % 30 + 30;
	if(m_nPatternNo == 0)
		nCount++;
	if (nCount >= nRand)
	{
		nCount = 0;
		nRand = rand() % 30 + 30;
		return true;
	}
	return false;
}

void CBoss::AttackAnim() {
	if (m_bAttackAnim == false)return;
	m_fTime++;

	if (m_fTime >= 40)
	{
		m_fTime = 0;
		m_pParty->SetHp(&m_CharaStatus.Attack);
		m_bAttackAnim = false;
	}
}

void CBoss::Awack()
{

	for (std::vector<bool>::iterator ite = CCharacter::m_vEnemyFlag.begin();
		ite < CCharacterBase::m_vEnemyFlag.end(); ite++) {
		if (*ite == false)return;
	}

	if (m_bActive == false) {
		m_bActive = true;
		m_bGoToPos = true;
		m_pFrontEff = (CFrontEffect*)Find(ID_FRONTEFF);
		m_pFrontEff->SetBossEff(true);

		m_nMotion = MOTION_IDOL;
	}

	if (m_bGoToPos == true)
	{
		m_CharaStatus.pos.x -= 2;

		if (m_CharaStatus.pos.x <= 500.0f) {
			m_bGoToPos = false;
			m_nMotion = MOTION_IDOL;
			m_pFrontEff->SetBossEff(false);
		}
	}
}

void CBoss::Anim() {

	m_nAnimeCount++;

	if ((m_nAnimeCount % INTERVAL_CHANGE_BOSS) == 0)				// 画像切り替えタイミングの判断
	{
		m_nPatternNo = (m_nPatternNo + 1) % BOSS_PATTERN_MAX;		// パターン切り替え
		m_pTexture->SetAnime(BOSS_PATTERN_MAX, BOSS_DIVIDE_X, BOSS_DIVIDE_Y, m_nPatternNo, m_nMotion);
		m_nAnimeCount = 0;
	}

}

void CBoss::SetHp(int * Att)
{
	m_CharaStatus.Hp -= *Att;
	if (m_CharaStatus.Hp <= 0) {
		m_CharaStatus.Dead = true;
		m_nMotion = MOTION_WALK2;
	} 
	m_bDamege = true;
}

void CBoss::DeadAnim()
{
	if (m_CharaStatus.Dead == true)
	{
		if (m_nDeathTimer >= 30)
		{
			m_bDead = true;
		}
		if (m_nDeathTimer >= 35)
		{
			m_StageClear.Dead = true;
		}
		m_nDeathTimer++;
	}
}

D3DXVECTOR3 CBoss::GetPos()
{
	return m_CharaStatus.pos;
}

void CBoss::DamegeEff() {

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