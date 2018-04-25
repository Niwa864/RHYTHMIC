#pragma once
#include "CharacterBase.h"

#define STAGEBG_SIZE_X	(2565.0f)
#define STAGEBG_SIZE_Y	(SCREEN_HEIGHT)
#define MOVE_VALUE (2.0f)

#define BOBBLE_NUM 10

class CStageBG :public CCharacterBase
{
private:
	void GetKey();

	CTexture* m_pBobble[BOBBLE_NUM];
	CHARASTATUS m_Bobble[BOBBLE_NUM];
	CTexture* m_pWave;
	CHARASTATUS m_Wave;
	CTexture* m_pLight[2];
	CHARASTATUS m_Light[2];

	void BobbleActive();
	void BobbleMove();

	void WaveScale();

	std::vector<bool>::iterator m_vFlagIte;

public:
	CStageBG(CScene* pScene);
	~CStageBG();

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