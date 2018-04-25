#pragma once
#include "CharacterBase.h"
#include "FrontEffect.h"
#include "Sound.h"

#define STAGE_HEIGHT (118.0f)
#define STAGE_WIDTH  (1024.0f)

class CStage :public CCharacterBase
{
private:
	void GetKey();
	LPDIRECT3DSURFACE9 m_pBackBuf;

	CFrontEffect* m_pFrontEff;
	CSound*		  m_Sound;
	CTexture*	  m_pUI[3];
	CHARASTATUS	  m_UI[3];
	bool m_bPause;
public:
	CStage(CScene* pScene);
	~CStage();

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

