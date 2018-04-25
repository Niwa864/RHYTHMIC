#pragma once
#include "CharacterBase.h"
#include "Stage.h"

#define MOVE_VALUE (2.0f)
#define GRAVITY    (2.0f)

class CEnemy :public CCharacterBase
{
private:
	typedef enum
	{
		MOTION_WALK,
		MOTION_ATTACK,
		MOTION_DIE,
		MOTION_MAX,

	} MOTION;

	void GetKey();
	void CollisionToStage();
	void CollisionToPlayer();
	void Attack();
	void DeadAnim();

	void Anim();

	void DamegeEff();

	int m_nThisNumber;
	int m_nAnimeCount;
	int m_nPatternNo;
	int m_nMotion;
	int m_nFindTimer;

	bool m_bDead;
	bool m_bMove;
	bool m_bBack;
	bool m_bDamege;
	int m_nDeathTimer;

	float m_fTime;
	float m_fDisFromStage;

	D3DXVECTOR3 m_vCrossParty;
	D3DXVECTOR3 m_vOldPos;

	std::vector<bool>::iterator ite;
	
	CStage* m_pStage;
public:
	CEnemy(CScene* pScene, int nNumber);
	~CEnemy();

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
	int GetNumber();
	void SetHp(int* fDamage);
};

