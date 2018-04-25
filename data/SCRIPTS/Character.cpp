#include "Character.h"
#include "Texture.h"
#include "Graphics.h"
#include "Input.h"
#include "GameWnd.h"
#include "Boss.h"

CBoss* m_pBoss;

CCharacter::CCharacter(CScene* pScene):CCharacterBase(pScene)
{
	_uID = ID_CHARACTER;													// 生成する際に利用するID　scene.h を参照

	m_pDevice = CGraphics::GetDevice();	
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/Vick.png");	// テキストコントラスタ　第二引数にテクスチャリソース
	m_pEnemyFound = new CTexture(m_pDevice, "../data/TEXTURE/EnemyFound.png");
	
	m_CharaStatus.pos = D3DXVECTOR3( 50.0f, 0.0f, 0.0f );							// 座標などの宣言　CharcterBace.hを参照
	m_CharaStatus.scale = D3DXVECTOR3( 1.0f, 1.0f, 0.0f );

	m_pTexture->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, CHARACTER_SIZE_X, CHARACTER_SIZE_Y, true);
	m_pTexture->SetAnime(PATTERN_MAX, TEX_DIVIDE_X, TEX_DIVIDE_Y, m_nPatternNo, m_nMotion);
	m_pEnemyFound->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, CHARACTER_SIZE_X, 75.0f, false);

	m_nTimer = 0;
	m_fRay = 0.0f;
	m_Found = m_CharaStatus;
	m_bDead = m_bEnemyFound = false;

	m_Sound = (CSound*)Find(ID_SOUND);
}


CCharacter::~CCharacter()
{
}

bool CCharacter::Init()
{
	return false;
}

void CCharacter::PreUpdate()
{
	
}

void CCharacter::Update()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	m_pStage = (CStage*)Find(ID_STAGE);

	GetKey();
	CollisionToEnemy();
	CollisionToStage();
	CollisionToBoss();

	m_nAnimeCount ++;

	if ((m_nAnimeCount % INTERVAL_CHANGE_PATTERN) == 0)				// 画像切り替えタイミングの判断
{
		m_nPatternNo = (m_nPatternNo + 1) % PATTERN_MAX;		// パターン切り替え
		m_pTexture->SetAnime(PATTERN_MAX, TEX_DIVIDE_X, TEX_DIVIDE_Y, m_nPatternNo, m_nMotion);
		m_nAnimeCount = 0;
	}

	m_pTexture->Update(m_CharaStatus.pos, D3DXToRadian(m_CharaStatus.angle));

	if(m_bEnemyFound == true)
		m_pEnemyFound->Update(m_Found.pos, D3DXToRadian(m_Found.angle));
}

void CCharacter::PostUpdate()
{
}

void CCharacter::PreDraw()
{
}

void CCharacter::Draw()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pTexture->Draw();

	if (m_bEnemyFound == true)
		m_pEnemyFound->Draw();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CCharacter::DrawAlpha()
{
}

void CCharacter::PostDraw()
{
}

void CCharacter::Fin()
{
	m_pEnemyFound->Uninit();
	m_pTexture->Uninit();
	m_pBoss = nullptr;
	m_pEnemy = nullptr;
	m_pStage = nullptr;
	delete m_pEnemyFound;
	delete m_pTexture;
}

void CCharacter::GetKey()
{
	if (CCharacterBase::m_bMoveFlag == true)
	{
		m_nMotion = MOTION_WALK;
	}

	if ((CInput::GetKeyPress(DIK_A) || CInput::GetKeyPress(DIK_D)) && m_nMotion == MOTION_WAIT && CCharacterBase::m_bMoveFlag == false)
	{
		m_nMotion = MOTION_SHAKE;
	}

	if (CCharacterBase::m_bMoveFlag == false && m_nMotion == MOTION_WALK) // 変更するかも？
	{
		m_nMotion = MOTION_WAIT;
	}
}


void CCharacter::CollisionToStage()
{
	//----- 当たり判定 -----
	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x;
	Seg.start.y = m_CharaStatus.pos.y + CHARACTER_SIZE_Y;
	Seg.start.z = 0.0f;
	Seg.vec = { 0.0f, 10.0f, 0.0f };		//　線分の計算に行う場所　移動ベクトルと同じ設定にする必要

	Seg1.start = m_pStage->GetPos();
	Seg1.vec = { 1024.0f, 0.0f, 0.0f };

	// 左点の処理
	if (m_pStage && ColSegments(Seg, Seg1, &vCross)) {// テクスチャの左下頂点の当たり判定
		m_CharaStatus.pos.y = vCross.y - CHARACTER_SIZE_Y;
	}
	else {
		Seg.start.x = m_CharaStatus.pos.x + CHARACTER_SIZE_X;
		if (m_pStage && ColSegments(Seg, Seg1, &vCross)) // テクスチャの右下頂点の当たり判定
			m_CharaStatus.pos.y = vCross.y - CHARACTER_SIZE_Y;
		else
			m_CharaStatus.pos.y += 8.0f;		// 当たってないときの移動ベクトル
	}
}

void CCharacter::CollisionToEnemy() 
{
	for (std::vector<bool>::iterator ite = CCharacter::m_vEnemyFlag.begin();
		ite < CCharacterBase::m_vEnemyFlag.end(); ite++) {
		if (*ite == false)break;
		if (ite == CCharacterBase::m_vEnemyFlag.end() - 1)return;
	}

	if(m_fRay == 0.0f)
	for (std::vector<bool>::iterator ite = CCharacter::m_vFlagVector.begin();
		ite < CCharacterBase::m_vFlagVector.end() - 1; ite++) {
		if (*ite == false)
			m_fRay += 80.0f;
	}

	// 敵が見つからなかったら
	if (m_pEnemy == nullptr)
		m_pEnemy = (CEnemy*)Find(ID_ENEMY);

	// 敵が死んでいたら
	while (!(m_pEnemy->GetDead() == false))
	{
		m_pEnemy = (CEnemy*)m_pEnemy->GetNext();
	}

	// 当たり判定
	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = 80.0f;
	Seg.start.y = m_CharaStatus.pos.y + CHARACTER_SIZE_Y / 2;
	Seg.start.z = 0.0f;
	Seg.vec = { 500.0f + m_fRay, 0.0f, 0.0f };		//　線分の計算に行う場所

	Seg1.start = m_pEnemy->GetPos() + D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	Seg1.vec = { 0.0f, -800.0f, 0.0f };

	static bool bOnce = true;
	if (m_pEnemy && ColSegments(Seg, Seg1, &vCross)) 
	{
		if (vCross == m_vCrossEnemy)return;
		m_bEnemyFound = true;	// 攻撃可能状態
		m_Found.pos = CCharacter::GetPos() + D3DXVECTOR3(5.0f, -80.0f, 0.0f);	// びっくりマーク

		if (bOnce == true) {
			m_Sound->Play(SE_ENEMYFOUND);
			bOnce ^= bOnce;
		}
	}
	else
	{
		m_bEnemyFound = false;

		if(bOnce == false)
			bOnce ^= bOnce;
	}
}

void CCharacter::CollisionToBoss() {
	for (std::vector<bool>::iterator ite = CCharacter::m_vEnemyFlag.begin();
		ite < CCharacterBase::m_vEnemyFlag.end(); ite++) {
		if (*ite == false)return;;
	}

	// 敵が見つからなかったら
	if (m_pBoss == nullptr)
		m_pBoss = (CBoss*)Find(ID_BOSS);
	

	// 当たり判定
	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x + CHARACTER_SIZE_X / 2;
	Seg.start.y = m_CharaStatus.pos.y + CHARACTER_SIZE_Y / 2;
	Seg.start.z = 0.0f;
	Seg.vec = { 1024.0f * 2, 0.0f, 0.0f };		//　線分の計算に行う場所

	Seg1.start = m_pBoss->GetPos() + D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	Seg1.vec = { 0.0f, -1000.0f, 0.0f };

	static bool bOnce = true;
	if (m_pBoss && ColSegments(Seg, Seg1, &vCross))
	{
		if (vCross == m_vCrossEnemy)return;
		m_bEnemyFound = true;	// 攻撃可能状態
		m_Found.pos = CCharacter::GetPos() + D3DXVECTOR3(5.0f, -80.0f, 0.0f);	// びっくりマーク
		
		if (bOnce == true) {
			m_Sound->Play(SE_ENEMYFOUND);
			bOnce ^= bOnce;
		}
	}
	else
	{
		m_bEnemyFound = false;
		if (bOnce == false)
			bOnce ^= bOnce;
	}
}

void CCharacter::Dead() {
	/*for (std::vector<bool>::iterator ite = CCharacter::m_vFlagVector.begin(); ite < CCharacter::m_vFlagVector.end(); ite++) {
		if (*ite == false)return;
	}
	if (m_nTimer == 0)
		m_nMotion = MOTION_MAX;*/
}

D3DXVECTOR3 CCharacter::GetPos()
{
	return m_CharaStatus.pos;
}
