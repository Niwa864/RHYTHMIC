#pragma once
#include "CharacterBase.h"
#include "Stage.h"
#include "FrontEffect.h"

#define MOVE_VALUE (2.0f)
#define GRAVITY    (2.0f)
const float BOSS_SIZE_X = 750.0f;
const float BOSS_SIZE_Y = 500.0f;
const float BOSS_DEFAULT_SIZE_X = 3000.0f;
const float BOSS_DEFAULT_SIZE_Y = 2000.0f;
const int BOSS_DIVIDE_X = (int)BOSS_DEFAULT_SIZE_X / (int)BOSS_SIZE_X;
const int BOSS_DIVIDE_Y = (int)BOSS_DEFAULT_SIZE_Y / (int)BOSS_SIZE_Y;
const int BOSS_PATTERN_MAX = BOSS_DIVIDE_X;
const int INTERVAL_CHANGE_BOSS = 10;	//ÉtÉåÅ[ÉÄíPà 

class CParty;
class CBoss :public CCharacterBase
{
private:
	typedef enum
	{
		MOTION_IDOL,
		MOTION_WALK,
		MOTION_WALK2,
		MOTION_ATTACK,
		MOTION_MAX,

	} MOTION;

	int m_nFindTimer;
	D3DXVECTOR3 m_vCrossParty;
	D3DXVECTOR3 m_vOldPos;
	float m_fTime;

	bool m_bActive;
	bool m_bGoToPos;
	bool m_bMove;
	bool m_bBack;
	bool m_bAttackAnim;
	bool m_bDead;
	bool m_bDamege;

	int m_nAnimeCount;
	int m_nPatternNo;
	int m_nMotion;
	int m_nDeathTimer;

	void GetKey();
	void GameClear();
	void CollisionToStage();
	void CollisionToPlayer();
	void Attack();
	bool IdolAnim();
	void AttackAnim();
	void Awack();
	void Anim();
	void DeadAnim();
	bool CanMove();
	void DamegeEff();

	CStage* m_pStage;
	CParty* m_pParty;
	CFrontEffect* m_pFrontEff;
	CTexture* m_pStageClear;
	CHARASTATUS m_StageClear;
public:
	CBoss(CScene* pScene);
	~CBoss();

	bool Init();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void PreDraw();
	void Draw();
	void DrawAlpha();
	void PostDraw();
	void Fin();

	void SetHp(int * Att);
	D3DXVECTOR3 GetPos();
	bool GetDead() { return m_CharaStatus.Dead; };
};

