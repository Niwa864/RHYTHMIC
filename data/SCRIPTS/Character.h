#pragma once
#include "CharacterBase.h"
#include "Stage.h"
#include "Enemy.h"
#include "Utility.h"

#define MOVE_VALUE (2.0f)
#define GRAVITY    (2.0f)
const float CHARACTER_SIZE_X = 100.0f;
const float CHARACTER_SIZE_Y = 100.0f;
const float CHARACTER_DEFAULT_SIZE_X = 400.0f;
const float CHARACTER_DEFAULT_SIZE_Y = 300.0f;
const int TEX_DIVIDE_X = (int)CHARACTER_DEFAULT_SIZE_X / (int)CHARACTER_SIZE_X;
const int TEX_DIVIDE_Y = (int)CHARACTER_DEFAULT_SIZE_Y / (int)CHARACTER_SIZE_Y;
const int PATTERN_MAX = TEX_DIVIDE_X;
const int INTERVAL_CHANGE_PATTERN = 10;	//フレーム単位

typedef enum 
{
	MOTION_WAIT,
	MOTION_WALK,
	MOTION_SHAKE,
	MOTION_MAX,
	
} MOTION;

class CBoss;
class CCharacter :public CCharacterBase
{
private:
	void GetKey();

	void CollisionToStage();

	void CollisionToEnemy();

	void CollisionToBoss();

	void Dead();

	CStage* m_pStage;
	CEnemy* m_pEnemy;
	CTexture* m_pEnemyFound;
	CHARASTATUS m_Found;
	
	int m_nAnimeCount;
	int m_nPatternNo;
	int m_nMotion;
	int m_nTimer;
	float m_fRay;
	D3DXVECTOR3 m_vCrossEnemy;
	bool m_bEnemyFound;
	bool m_bDead;
	CSound* m_Sound;
	//bool m_bDirection = 1; // 0:Left 1:Right そもそも主人公に左を向かせることがあるのか？
public:
	CCharacter(CScene* pScene);
	~CCharacter();

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
};

