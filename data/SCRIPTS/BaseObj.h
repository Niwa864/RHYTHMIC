#pragma once
#include <d3d9.h>
#include <d3dx9.h>

typedef enum {
	ID_BASEOBJ = 0,
	ID_CAMERA,		// カメラ
	ID_SOUND,

	ID_3D,			// 3Dオブジェクト
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
	UINT		_uID;		// 種別
	CScene*		_pScene;	// 属しているシーン
	D3DXMATRIX	_mWorld;	// ワールド変換行列
	D3DXVECTOR3	_vSpeed;	// 速度
	D3DXVECTOR3	_vAccel;	// 加速度
	D3DXVECTOR3	_vCenter;	// 境界球(ボックス)中心
	D3DXVECTOR3	_vBBox;	// 境界ボックスサイズ
	float		_fRadius;	// 境界球半径
	CBaseObj*	_pNext;	// 次のオブジェクト
	CBaseObj*	_pBack;	// 前のオブジェクト
	CBaseObj*	_pParent;	// 親のオブジェクト

	D3DMATERIAL9	_mate;	// 境界球/境界ボックスの色
	LPD3DXMESH		_pBB;	// 境界ボックス
	LPD3DXMESH		_pBS;	// 境界球

public:
	CBaseObj(CScene* pScene);
	~CBaseObj(void);

	virtual bool Init();		// 初期化
	virtual void PreUpdate();	// 更新前処理
	virtual void Update();		// 更新
	virtual void PostUpdate();	// 更新後処理
	virtual void PreDraw();		// 描画前処理
	virtual void Draw();		// 不透明部分描画
	virtual void DrawAlpha();	// 半透明部分描画
	virtual void PostDraw();	// 描画後処理
	virtual void Fin();			// 終了処理

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

