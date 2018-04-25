#pragma once
#include "CharacterBase.h"
#include "Stage.h"
#include "Enemy.h"

#define MOVE_VALUE (2.0f)
#define GRAVITY    (2.0f)
const float PARTY_SIZE_X = 360.0f / 4;
const float PARTY_SIZE_Y = 280.0f / 4;
const float PARTY_DEFAULT_SIZE_X = 360.0f;
const float PARTY_DEFAULT_SIZE_Y = 280.0f;
const int PARTY_DIVIDE_X = (int)PARTY_DEFAULT_SIZE_X / (int)PARTY_SIZE_X;
const int PARTY_DIVIDE_Y = (int)PARTY_DEFAULT_SIZE_Y / (int)PARTY_SIZE_Y;
const int PARTY_PATTERN_MAX = PARTY_DIVIDE_X;
const int PARTY_CHANGE_FRAME = 10;	//フレーム単位

typedef enum
{
	UNIT_SPOOK,
	UNIT_OGRE,
	UNIT_MAX,

} UNIT;

class CBoss;
class CParty :public CCharacterBase
{
private:
	typedef enum
	{
		MOTION_WALK,
		MOTION_WALK2,
		MOTION_ATTACK,
		MOTION_DIE,
		MOTION_MAX,

	} MOTION;

	void GetKey();
	void CollisionToEnemy();
	void CollisionToBoss();
	void CollisionToStage();
	void AttackToEnemy();
	void DeadAnim();
	void Anim();

	void DamegeEff();

	CStage* m_pStage;
	CEnemy* m_pEnemy;
	int m_nAnimeCount;
	int m_nPatternNo;
	int m_nMotion;
	int m_nDeathTimer;

	bool m_bDead;
	bool m_bMove;
	bool m_bBack;
	bool m_bDamege;
	bool m_bAttack;
	bool m_bDeadAnim;
	float m_fTime;
	float m_fDistanceFromStage;

	D3DXVECTOR3 m_vCrossEnemy;
	D3DXVECTOR3 m_vOldPos;

	std::vector<bool>::iterator m_vFlagIte;

	//bool m_bDirection = 1; // 0:Left 1:Right そもそも主人公に左を向かせることがあるのか？
public:
	CParty(CScene* pScene, int nNumber);
	~CParty();

	bool Init();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void PreDraw();
	void Draw();
	void DrawAlpha();
	void PostDraw();
	void Fin();

	D3DXVECTOR3 GetPos();
	bool GetDead();
	bool GetMove();
	void SetHp(int *Att);
};

