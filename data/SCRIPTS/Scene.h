//=======================================================================================
//
//	  �Q�[���V�[�� �N���X��`
//
//=======================================================================================
#pragma once

#include "Graphics.h"
#include "BaseObj.h"
#include "Camera.h"
#include "CharacterBase.h"
#include "Character.h"
#include "Enemy.h"
#include "Stage.h"
#include "StageBG.h"
#include "Rhythm.h"
//#include "Shader.h"
#include "FrontEffect.h"
#include "Boss.h"
#include "Party.h"
#include "Title.h"
#include "Select.h"
#include "Utility.h"
#include "SelectChara.h"
#include "Sound.h"


class CScene
{
private:
	CGraphics*	m_pGraph;			// �O���t�B�b�N�f�o�C�X
	CBaseObj*	m_pObj;				// �I�u�W�F�N�g�擪

	TCHAR		m_szDebug[1024];	// �f�o�b�O�p������
	int			m_FPS;				// �t���[�����J�E���g�p

	DWORD		m_dwTick;			// �o�ߎ���(�t���[����)

	static GAME_MODE	m_Game;

	static bool		m_bInit;

	//-------- �Q�[���p�I�u�W�F�N�g
	// ReadHere : �Q�[����ɕ\���������I�u�W�F�N�g�̃N���X��錾 ����:Scene.cpp LoopInit���Q��
	CBaseObj*	m_pTop;
	CCharacterBase* m_CharaBase;
	CCamera*	_pCamera;
	CCharacter* m_Character;
	CEnemy*		m_Enemy[5];
	CStage*		m_Stage;
	CStageBG*	m_StageBG;
	CRhythm*    m_Rhythm;
	//CShader*	m_Shader;
	CFrontEffect* m_FrontEff;
	CBoss*		m_pBoss;
	CParty*		m_pParty[4];
	CTitle*		m_pTitle;
	CSelect*	m_pSelect;
	CSelectChara* m_pSelectChar;
	CSound*		m_pSound;


public:
	CScene();
	virtual ~CScene();

	static CScene* Create(CGraphics* pGraph);
	void Release();
	void Render();
	void Update();
	void SetFPS(int nFPS) { m_FPS = nFPS; }
	void AddDebugStr(LPCTSTR psz) { lstrcat(m_szDebug, psz); }

	void Add(CBaseObj* pObj);
	void Del(CBaseObj* pObj);
	CBaseObj* Find(UINT uID, CBaseObj* p);
	void InitObj();
	void FinObj();
	void UpdateObj();
	void DrawObj();
	DWORD GetTick();

	static void SetGameMode(GAME_MODE game) { m_Game = game; m_bInit = true; }
	static GAME_MODE GetGameMode() { return m_Game; }

private:
	bool Initialize(CGraphics* pGraph);
	void Finalize();
	void Draw();

	void Draw2D();
	void Draw3D();
	CBaseObj * Find(const GAMEOBJ_ID & uID, CBaseObj * p) const;

	void LoopInit();
	void LoopUnInit();
};

//=======================================================================================
//	End of File
//=======================================================================================