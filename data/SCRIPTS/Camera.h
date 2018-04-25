// �J���� �N���X��`
#pragma once

#include "BaseObj.h"

enum ECamPos {
	CP_FIXED = 0,	// �Œ�
	CP_FPVIEW,		// 1�l��
	CP_TPVIEW,		// 3�l��

	MAX_VIEW
};

class CCamera : public CBaseObj
{
protected:
	D3DXMATRIX	m_proj;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_view;		// �r���[�}�g���b�N�X

	ECamPos		m_cp;		// �J�������
	D3DXVECTOR3	m_pos;		// ���_
	D3DXVECTOR3	m_at;		// �����_
	D3DXVECTOR3 m_up;		// ����x�N�g��
	D3DXVECTOR3	m_pos2;		// �ŏI���_
	D3DXVECTOR3	m_at2;		// �ŏI�����_
	D3DXVECTOR3 m_up2;		// �ŏI����x�N�g��
	float		m_fTime;	// �؂�ւ�����

	float		m_fFovy;	// ����p
	float		m_fAspect;	// �A�X�y�N�g��
	float		m_fZNear;	// �O���N���b�v����
	float		m_fZFar;	// ����N���b�v����

public:
	CCamera(CScene* pScene);
	virtual ~CCamera(void);

	virtual bool Init();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();
	virtual void Fin();
	void PostUpdate();		// �X�V�㏈��
	void PreDraw();			// �`��O����
};