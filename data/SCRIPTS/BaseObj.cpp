// ゲームオブジェクト実装
#include "BaseObj.h"
#include "Scene.h"

// コンストラクタ
CBaseObj::CBaseObj(CScene* pScene) : _pScene(pScene)
{
	_uID = ID_BASEOBJ;

	_pScene->Add(this);

	_pBB = _pBS = NULL;
}

// デストラクタ
CBaseObj::~CBaseObj()
{
	ReleaseBBox();
	ReleaseBSphere();

	_pScene->Del(this);
}

// 初期化
bool CBaseObj::Init()
{
	D3DXMatrixIdentity(&_mWorld);
	_vSpeed = D3DXVECTOR3(0, 0, 0);
	_vAccel = D3DXVECTOR3(0, 0, 0);
	_vCenter = D3DXVECTOR3(0, 0, 0);
	_vBBox = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	_fRadius = 1.0f;
	return true;
}

// 更新前処理
void CBaseObj::PreUpdate()
{
}

// 更新
void CBaseObj::Update()
{
	_vSpeed += _vAccel;
	_mWorld._41 += _vSpeed.x;
	_mWorld._42 += _vSpeed.y;
	_mWorld._43 += _vSpeed.z;
}

// 更新後処理
void CBaseObj::PostUpdate()
{
}

// 描画
void CBaseObj::PreDraw()
{
}
void CBaseObj::Draw()
{
}
void CBaseObj::DrawAlpha()
{
	// 境界球or境界ボックス描画
	if (_pBS || _pBB) {
		LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
		pDevice->SetMaterial(&_mate);
		pDevice->SetTexture(0, NULL);
		D3DXVECTOR3 vCenter;
		D3DXVec3TransformCoord(&vCenter, &_vCenter, &_mWorld);
		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, vCenter.x, vCenter.y, vCenter.z);
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		if (_pBS)	// 境界球
			_pBS->DrawSubset(0);
		if (_pBB)	// AABB
			_pBB->DrawSubset(0);
	}
}
void CBaseObj::PostDraw()
{
}

// 終了処理
void CBaseObj::Fin()
{
}

// ワールドマトリックス設定
void CBaseObj::SetWorld(D3DXMATRIX* pWorld)
{
	if (pWorld) {
		_mWorld = *pWorld;
		return;
	}
	D3DXMatrixIdentity(&_mWorld);
}

// 座標取得
D3DXVECTOR3 CBaseObj::GetPos()
{
	return D3DXVECTOR3(_mWorld._41, _mWorld._42, _mWorld._43);
}

// 右方向ベクトル取得
D3DXVECTOR3 CBaseObj::GetRight()
{
	return D3DXVECTOR3(_mWorld._11, _mWorld._12, _mWorld._13);
}

// 上方向ベクトル取得
D3DXVECTOR3 CBaseObj::GetUp()
{
	return D3DXVECTOR3(_mWorld._21, _mWorld._22, _mWorld._23);
}

// 前方ベクトル取得
D3DXVECTOR3 CBaseObj::GetForward()
{
	return D3DXVECTOR3(_mWorld._31, _mWorld._32, _mWorld._33);
}

// 座標設定
void CBaseObj::SetPos(D3DXVECTOR3* pPos)
{
	if (pPos) {
		_mWorld._41 = pPos->x;
		_mWorld._42 = pPos->y;
		_mWorld._43 = pPos->z;
		return;
	}
	_mWorld._41 = _mWorld._42 = _mWorld._43 = 0.0f;
}

// 速度設定
void CBaseObj::SetSpeed(D3DXVECTOR3* pSpeed)
{
	_vSpeed = (pSpeed) ? *pSpeed : D3DXVECTOR3(0, 0, 0);
}

// 加速度設定
void CBaseObj::SetAccel(D3DXVECTOR3* pAccel)
{
	_vAccel = (pAccel) ? *pAccel : D3DXVECTOR3(0, 0, 0);
}

// 親オブジェクトの設定
void CBaseObj::SetParent(CBaseObj* pParent)
{
	CBaseObj* pOldParent = _pParent;
	_pParent = pParent;
	if (pOldParent) {
		// 接続解除

	}
	if (_pParent) {
		// 新規接続

	}
}

// 球どうしの当たり判定
bool CBaseObj::CollisionSphere(CBaseObj* pObj)
{
	// TODO: ここに球どうしの当たり判定を記述.
	return false;
}

// AABBどうしの当たり判定
bool CBaseObj::CollisionAABB(CBaseObj* pObj)
{
	// TODO: ここにAABBどうしの当たり判定を記述.
	return false;
}

// デバッグ表示用境界球生成
void CBaseObj::CreateBSphere()
{
	ReleaseBSphere();
	D3DXCreateSphere(CGraphics::GetDevice(), _fRadius, 16, 8, &_pBS, NULL);
}

// デバッグ表示用境界球解放
void CBaseObj::ReleaseBSphere()
{
	if (_pBS) {
		_pBS->Release();
		_pBS = NULL;
	}
}

// デバッグ表示用境界ボックス生成
void CBaseObj::CreateBBox()
{
	ReleaseBBox();
	D3DXCreateBox(CGraphics::GetDevice(),
		_vBBox.x * 2.0f, _vBBox.y * 2.0f, _vBBox.z * 2.0f, &_pBB, NULL);
}

// デバッグ表示用境界ボックス解放
void CBaseObj::ReleaseBBox()
{
	if (_pBB) {
		_pBB->Release();
		_pBB = NULL;
	}
}

// オブジェクト検索
CBaseObj* CBaseObj::Find(UINT uID, CBaseObj* p)
{
	if (!_pScene) return NULL;
	return _pScene->Find(uID, p);
}
