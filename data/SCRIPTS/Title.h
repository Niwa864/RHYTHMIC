#pragma once
#include "CharacterBase.h"
#include "FrontEffect.h"
#include "Sound.h"

#define BOBBLE_MAX (30) 

class CTitle :	public CCharacterBase
{
private:
	void GetKey();
	void BobbleActive();
	void BobbleMove();
	CTexture* m_pTexture[4];
	CHARASTATUS m_CharaStatus[4];

	CTexture* m_pBobble[BOBBLE_MAX];
	CHARASTATUS m_Bobble[BOBBLE_MAX];

	CFrontEffect* CFrontEff;
	CSound* m_Sound;

public:
	CTitle(CScene* pScene);
	~CTitle();

	bool Init();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void PreDraw();
	void Draw();
	void DrawAlpha();
	void PostDraw();
	void Fin();
};