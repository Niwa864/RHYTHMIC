#pragma once
#include "CharacterBase.h"
#include "FrontEffect.h"
#include "Sound.h"

class CSelect :	public CCharacterBase
{
private:
	void GetKey();
	void LerpOffset();
	void DotShow();
	void CrossShow();
	CTexture* m_pTexture[1];
	CHARASTATUS m_CharaStatus[1];
	CTexture* m_pCross;
	CHARASTATUS m_Cross;
	CTexture* m_pDot[6];
	CHARASTATUS m_Dot[6];
	CFrontEffect* m_FrontEff;
	TextureOffset Offset;
	CSound* m_Sound;

	float m_fTime;
	float m_fDotTimer;
	int	 m_nFade;

	bool m_bDotFlag;
	bool m_bCrossFade;

public:
	CSelect(CScene* pScene);
	~CSelect();

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