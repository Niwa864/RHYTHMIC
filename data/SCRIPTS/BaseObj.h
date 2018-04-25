#pragma once
#include <d3d9.h>
#include <d3dx9.h>

typedef enum {
	ID_BASEOBJ = 0,
	ID_CAMERA,		// �J����
	ID_SOUND,

	ID_3D,			// 3D�I�u�W�F�N�g
	// 3D
	ID_3DMAX,

	ID_2D,
	//2D
	ID_CHARBASE,
	ID_FRONTEFF,
	ID_TITLE,
	ID_SELECT,
	ID_SELECTCHAR,
	ID_RHYTHM,
	ID_CHARACTER,
	ID_PARTY,
	ID_ENEMY,
	ID_BOSS,
	ID_STAGE,
	ID_STAGEBG,
	ID_2DMAX,

	ID_MAX
} GAMEOBJ_ID;

class CScene;
class CBaseObj
{
protected:
	UINT		_uID;		// ���
	CScene*		_pScene;	// �����Ă���V�[��
	D3DXMATRIX	_mWorld;	// ���[���h�ϊ��s��
	D3DXVECTOR3	_vSpeed;	// ���x
	D3DXVECTOR3	_vAccel;	// �����x
	D3DXVECTOR3	_vCenter;	// ���E��(�{�b�N�X)���S
	D3DXVECTOR3	_vBBox;	// ���E�{�b�N�X�T�C�Y
	float		_fRadius;	// ���E�����a
	CBaseObj*	_pNext;	// ���̃I�u�W�F�N�g
	CBaseObj*	_pBack;	// �O�̃I�u�W�F�N�g
	CBaseObj*	_pParent;	// �e�̃I�u�W�F�N�g

	D3DMATERIAL9	_mate;	// ���E��/���E�{�b�N�X�̐F
	LPD3DXMESH		_pBB;	// ���E�{�b�N�X
	LPD3DXMESH		_pBS;	// ���E��

public:
	CBaseObj(CScene* pScene);
	~CBaseObj(void);

	virtual bool Init();		// ������
	virtual void PreUpdate();	// �X�V�O����
	virtual void Update();		// �X�V
	virtual void PostUpdate();	// �X�V�㏈��
	virtual void PreDraw();		// �`��O����
	virtual void Draw();		// �s���������`��
	virtual void DrawAlpha();	// �����������`��
	virtual void PostDraw();	// �`��㏈��
	virtual void Fin();			// �I������

	virtual UINT GetID() { return _uID; }
	virtual D3DXMATRIX& GetWorld() { return _mWorld; }
	virtual void SetWorld(D3DXMATRIX* pWorld);
	virtual D3DXVECTOR3 GetPos();
	virtual D3DXVECTOR3 GetRight();
	virtual D3DXVECTOR3 GetUp();
	virtual D3DXVECTOR3 GetForward();
	virtual void SetPos(D3DXVECTOR3* pPos);
	virtual D3DXVECTOR3& GetSpeed() { return _vSpeed; }
	virtual void SetSpeed(D3DXVECTOR3* pSpeed);
	virtual D3DXVECTOR3& GetAccel() { return _vAccel; }
	virtual void SetAccel(D3DXVECTOR3* pAccel);
	virtual float GetRadius() { return _fRadius; }
	virtual D3DXVECTOR3& GetCenter() { return _vCenter; }
	virtual D3DXVECTOR3& GetBBox() { return _vBBox; }
	virtual CScene* GetScene() { return _pScene; }
	virtual CBaseObj* GetNext() { return _pNext; }
	virtual CBaseObj* GetBack() { return _pBack; }
	virtual CBaseObj* GetParent() { return _pParent; }
	virtual void SetParent(CBaseObj* pParent);
	virtual void SetBack(CBaseObj* pBack) { _pBack = pBack; }
	virtual void SetNext(CBaseObj* pNext) { _pNext = pNext; }
	bool CollisionSphere(CBaseObj* pObj);
	bool CollisionAABB(CBaseObj* pObj);
	void CreateBSphere();
	void ReleaseBSphere();
	void CreateBBox();
	void ReleaseBBox();
	CBaseObj* Find(UINT uID, CBaseObj* p = NULL);
};

