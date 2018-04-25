#pragma once
#include "CharacterBase.h"
#include "FrontEffect.h"
#include "BackEffect.h"
#include "Sound.h"

const float SPOOK_SIZE_X = 70.0f;
const float SPOOK_SIZE_Y = 70.0f;
const float SPOOK_DEFAULT_SIZE_X = 280.0f;
const float SPOOK_DEFAULT_SIZE_Y = 210.0f;
const int SPOOK_DIVIDE_X = (int)SPOOK_DEFAULT_SIZE_X / (int)SPOOK_SIZE_X;
const int SPOOK_DIVIDE_Y = (int)SPOOK_DEFAULT_SIZE_Y / (int)SPOOK_SIZE_Y;
const int SPOOK_PATTERN_MAX = SPOOK_DIVIDE_X;
const int SPOOK_CHANGE_FRAME = 10;	//フレーム単位

class CSelectChara : public CCharacterBase
{
private:
	void GetKey();
	void Anim();
	CFrontEffect* m_FrontEff;
	CBackEffect* m_pBackEff;
	CTexture* m_pTexture[6 + 1]; // 0:背景 1:ステージ  2345 味方編成
	CHARASTATUS m_CharaStatus[6 + 1];
	CSound* m_Sound;

	int m_nAnimeCount;
	int m_nPatternNo;
	int m_nMotion;
	int m_nNowChar;

public:
	CSelectChara(CScene* pScene);
	~CSelectChara();

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

