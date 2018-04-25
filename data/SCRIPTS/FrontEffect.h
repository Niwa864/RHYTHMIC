#pragma once
#include "CharacterBase.h"
#include "Utility.h"

class CFrontEffect :	public CCharacterBase
{
private:
	void GetKey();		// 純粋仮想関数ため、実装　たぶん使わない
	
	int m_nPassId;
	int m_nBossEffTimer;

	float fAlpha;

	bool fFadeOut;
	bool bFadeStart;
	GAME_MODE m_GameMode;
	CTexture* m_Temp;
public:
	CFrontEffect(CScene* pScene);
	~CFrontEffect();

	bool Init();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void PreDraw();
	void Draw();
	void DrawAlpha();
	void PostDraw();
	void Fin();

	void SetFade(GAME_MODE nGameMode);
	void SetBossEff(bool flag);
	void SetPauseEff(bool flag, char* cPass);
	void SetCombo(bool bFlag);
	bool GetPauseEff();
};

