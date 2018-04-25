#include "Party.h"
#include "Texture.h"
#include "Graphics.h"
#include "Input.h"
#include "GameWnd.h"
#include "Boss.h"

const int nAttack = 10;
const int nHp = 100;
const float fDistance = 80.0f;
const float fDistanceFromStage = -50.0f;
CBoss* m_pBossForChar;

CParty::CParty(CScene* pScene, int nNumber) :CCharacterBase(pScene)
{
	_uID = ID_PARTY;													// ��������ۂɗ��p����ID�@scene.h ���Q��

	m_pDevice = CGraphics::GetDevice();
	m_pTexture = new CTexture(m_pDevice, "../data/TEXTURE/Ghost.png");	// �e�L�X�g�R���g���X�^�@�������Ƀe�N�X�`�����\�[�X

	m_CharaStatus.pos = D3DXVECTOR3(10.0f + fDistance * nNumber, 0.0f, 0.0f);							// ���W�Ȃǂ̐錾�@CharcterBace.h���Q��
	m_CharaStatus.scale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_CharaStatus.Attack = nAttack;
	m_CharaStatus.Hp = nHp;
	m_CharaStatus.Dead = false;
	m_pTexture->Init(m_CharaStatus.pos, m_CharaStatus.scale, 0.0f, PARTY_SIZE_X, PARTY_SIZE_Y, true);
	m_pTexture->SetAnime(PARTY_PATTERN_MAX, PARTY_DIVIDE_X, PARTY_DIVIDE_Y, m_nPatternNo, m_nMotion);

	CCharacterBase::m_vFlagVector.push_back(false);
	m_vFlagIte = CCharacterBase::m_vFlagVector.end() - 1;

	m_nDeathTimer = 0;
	m_bDamege = m_bDeadAnim = m_bAttack = m_bDead = m_bBack = m_bMove = false;
	m_fDistanceFromStage = fDistanceFromStage;
	m_fTime = 0.0f;
}


CParty::~CParty()
{
}

bool CParty::Init()
{
	return false;
}

void CParty::PreUpdate()
{

}

void CParty::Update()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	if (m_bDead == false) 
	{
		DeadAnim();
		Anim();
		CollisionToStage();
	}

	if (m_CharaStatus.Dead == false) {
		GetKey();
		DamegeEff();
		CollisionToEnemy();
		CollisionToBoss();
		AttackToEnemy();
	}

	m_pTexture->Update(m_CharaStatus.pos, D3DXToRadian(m_CharaStatus.angle));
}

void CParty::PostUpdate()
{
}

void CParty::PreDraw()
{
}

void CParty::Draw()
{
	if (((CFrontEffect*)Find(ID_FRONTEFF))->GetPauseEff() == true)return;
	if (m_bDead == true)return;
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pTexture->Draw();
}

void CParty::DrawAlpha()
{
}

void CParty::PostDraw()
{
}

void CParty::Fin()
{
	m_pTexture->Uninit();
	m_pBossForChar = nullptr;
	m_pEnemy = nullptr;
	m_pStage = nullptr;
	delete m_pTexture;
}

void CParty::GetKey()
{
	// �f�o�b�O�p
}

void CParty::CollisionToEnemy()
{
	for (std::vector<bool>::iterator ite = CCharacter::m_vEnemyFlag.begin();
		ite < CCharacterBase::m_vEnemyFlag.end(); ite++) {
		if (*ite == false)break;
		if (ite == CCharacterBase::m_vEnemyFlag.end() - 1)return;
	}

	// �G��������Ȃ�������
	if(m_pEnemy == nullptr)
		m_pEnemy = (CEnemy*)Find(ID_ENEMY);

	// �G������ł�����
	while (!(m_pEnemy->GetDead() == false))
	{
		m_pEnemy = (CEnemy*)m_pEnemy->GetNext();
	}

	// �G�Ƃ̓����蔻��@���G�͈�
	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x + CHARACTER_SIZE_X / 2;
	Seg.start.y = m_CharaStatus.pos.y + CHARACTER_SIZE_Y / 2;
	Seg.start.z = 0.0f;
	Seg.vec = { 500.0f, 0.0f, 0.0f };		//�@�����̌v�Z�ɍs���ꏊ

	Seg1.start = m_pEnemy->GetPos() + D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	Seg1.vec = { 0.0f, -800.0f, 0.0f };

	if (m_pEnemy && ColSegments(Seg, Seg1, &vCross)) {
		if (vCross == m_vCrossEnemy || m_bAttack == true)return;
			m_vCrossEnemy = vCross;
		if (m_bMove == false) {

			m_vOldPos = m_CharaStatus.pos;
			m_bMove = true;
		}
	}
	else
		if (m_bMove == false)
			*m_vFlagIte = false;
}

void CParty::CollisionToBoss() {
	// �G��������Ȃ�������
	for (std::vector<bool>::iterator ite = CCharacter::m_vEnemyFlag.begin();
		ite < CCharacterBase::m_vEnemyFlag.end(); ite++) {
		if (*ite == false)return;
	}

	if (m_pBossForChar == nullptr)
		m_pBossForChar = (CBoss*)Find(ID_BOSS);

	// �G�Ƃ̓����蔻��@���G�͈�
	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x + CHARACTER_SIZE_X / 2;
	Seg.start.y = m_CharaStatus.pos.y + CHARACTER_SIZE_Y / 2;
	Seg.start.z = 0.0f;
	Seg.vec = { 500.0f, 0.0f, 0.0f };		//�@�����̌v�Z�ɍs���ꏊ

	Seg1.start = m_pBossForChar->GetPos() + D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	Seg1.vec = { 0.0f, -1000.0f, 0.0f };

	if (m_pBossForChar && ColSegments(Seg, Seg1, &vCross)) {
		if (vCross == m_vCrossEnemy || m_bAttack == true)return;
			m_vCrossEnemy = vCross;
		if (m_bMove == false) {

			m_vOldPos = m_CharaStatus.pos;
			m_bMove = true;
		}
	}
}

void CParty::CollisionToStage()
{
	if (m_bAttack == true)return;
	m_pStage = (CStage*)Find(ID_STAGE);

	//----- �����蔻�� -----
	Segment Seg, Seg1;
	D3DXVECTOR3 vCross;
	Seg.start.x = m_CharaStatus.pos.x;
	Seg.start.y = m_CharaStatus.pos.y + PARTY_SIZE_Y;
	Seg.start.z = 0.0f;
	Seg.vec = { 0.0f, 10.0f, 0.0f };		//�@�����̌v�Z�ɍs���ꏊ�@�ړ��x�N�g���Ɠ����ݒ�ɂ���K�v

	Seg1.start = m_pStage->GetPos();
	Seg1.start.y += m_fDistanceFromStage;
	Seg1.vec = { SCREEN_WIDTH, 0.0f, 0.0f };

	// ���_�̏���
	if (m_pStage && ColSegments(Seg, Seg1, &vCross)) {// �e�N�X�`���̍������_�̓����蔻��
		m_CharaStatus.pos.y = vCross.y - PARTY_SIZE_Y;
	}
	else {
		Seg.start.x = m_CharaStatus.pos.x + PARTY_SIZE_Y;
		if (m_pStage && ColSegments(Seg, Seg1, &vCross)) // �e�N�X�`���̉E�����_�̓����蔻��
			m_CharaStatus.pos.y = vCross.y - PARTY_SIZE_Y;
		else
			m_CharaStatus.pos.y += 10.0f;		// �������ĂȂ��Ƃ��̈ړ��x�N�g��
	}
}

void CParty::AttackToEnemy()
{
	if (m_bMove == false)return;
	if (*m_vFlagIte == false)return; // �U������
	//if (m_pEnemy->GetDead() == true)return;

	m_bAttack = true;

	D3DXVECTOR3 vTemp;

	// �i�ޏ���
	if (m_bBack == false) {
		m_vCrossEnemy.y = m_CharaStatus.pos.y;
		D3DXVec3Lerp(&vTemp, &m_CharaStatus.pos, &(m_vCrossEnemy - D3DXVECTOR3(50.0f, 0.0f, 0.0f)), m_fTime);

		//if(m_fTime == 0.0f)
		m_nMotion = MOTION_ATTACK;
		if (m_fTime >= 1.0f)
		{
			m_fTime = 0.0f;
			m_bBack = true;
			if (m_pBossForChar == nullptr)
				m_pEnemy->SetHp(&m_CharaStatus.Attack);
			else if (m_pBossForChar != nullptr)
				m_pBossForChar->SetHp(&m_CharaStatus.Attack);
		}
	}
	// �߂鏈��
	else if (m_bBack == true) {
		D3DXVec3Lerp(&vTemp, &m_CharaStatus.pos, &m_vOldPos, m_fTime);

		m_nMotion = MOTION_WALK;

		if (m_fTime >= 1.0f)
		{
			m_fTime = 0.0f;
			m_bBack = false;
			m_bMove = false;
			m_bAttack = false;
			*m_vFlagIte = false;
		}
	}
	
	// Lerp�̕␳
	m_fTime += 0.01f;

	m_CharaStatus.pos = vTemp;
}

D3DXVECTOR3 CParty::GetPos()
{
	return m_CharaStatus.pos;
}
bool CParty::GetDead() {
	return m_CharaStatus.Dead;
}

bool CParty::GetMove()
{
	return m_bAttack;
}

void CParty::SetHp(int * Att)
{
	m_CharaStatus.Hp -= *Att;
	if (m_CharaStatus.Hp <= 0)
	{
		m_CharaStatus.Dead = true;
		m_nMotion = MOTION_DIE;
	}
	m_bDamege = true;
}

void CParty::DeadAnim()
{
	if (m_CharaStatus.Dead == true)
	{
		if (m_nDeathTimer <= 30)
		{
			m_nDeathTimer++;
			m_fDistanceFromStage += 0.1f;
		}
		else
		{
			m_bDead = true;
		}

	}
}

void CParty::Anim()
{
	if (m_bDeadAnim == true)return;

	m_nAnimeCount++;

	if ((m_nAnimeCount % INTERVAL_CHANGE_PATTERN) == 0)				// �摜�؂�ւ��^�C�~���O�̔��f
	{
		m_nPatternNo = (m_nPatternNo + 1) % PATTERN_MAX;		// �p�^�[���؂�ւ�
		m_pTexture->SetAnime(PARTY_PATTERN_MAX, PARTY_DIVIDE_X, PARTY_DIVIDE_Y, m_nPatternNo, m_nMotion);
		m_nAnimeCount = 0;
	}
}

void CParty::DamegeEff() {

	if (m_bDamege == false)return;

#define TIMER_MAX (5)

	static bool bDamegeFlag = false;
	static int  nCounter = 0;
	static int	nTimer = 0;
	
	if (bDamegeFlag == false)
	{
		m_pTexture->SetColor({255,255,255,0});

		nTimer++;

		if (nTimer == TIMER_MAX)
			bDamegeFlag = !bDamegeFlag;
	}
	else if(bDamegeFlag == true)
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