#pragma once
#include "CharacterBase.h"
#include "Character.h"
#include "Sound.h"
#include <vector>

class CRhythm :
	public CCharacterBase
{
private:
	void GetKey();

	void RhythmTimer();

	void Combo();

	void Fever();

	bool m_bGetKey;
	bool m_bCollect;

	int m_nTimer;
	int m_nRhythmTimer;
	int m_nNowRhythm;
	int m_fFade;
	int m_nNowCombo;
	int m_nComboFade;

	std::vector<int> m_vRhythmArray;

	CCharacter* m_pChar;
	CSound*		m_pSound;
	CTexture*	m_pRhythm[4];
	CHARASTATUS m_RhyStatus[4];
	CTexture*	m_pPostEff;
	CHARASTATUS m_PostEff;
	CFrontEffect* m_pFrontEff;
	CTexture*	m_pNumber;
	CHARASTATUS m_Number;
	CTexture*	m_pCombo;
	CHARASTATUS m_Combo;
	CTexture*	m_pFever[3];
	CHARASTATUS m_Fever[3];
public:
	CRhythm(CScene* pScene);
	~CRhythm();

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

