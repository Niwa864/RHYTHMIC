// �Q�[���I�u�W�F�N�g����
#include "BaseObj.h"
#include "Scene.h"

// �R���X�g���N�^
CBaseObj::CBaseObj(CScene* pScene) : _pScene(pScene)
{
	_uID = ID_BASEOBJ;

	_pScene->Add(this);

	_pBB = _pBS = NULL;
}

// �f�X�g���N�^
CBaseObj::~CBaseObj()
{
	ReleaseBBox();
	ReleaseBSphere();

	_pScene->Del(this);
}

// ������
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

// �X�V�O����
void CBaseObj::PreUpdate()
{
}

// �X�V
void CBaseObj::Update()
{
	_vSpeed += _vAccel;
	_mWorld._41 += _vSpeed.x;
	_mWorld._42 += _vSpeed.y;
	_mWorld._43 += _vSpeed.z;
}

// �X�V�㏈��
void CBaseObj::PostUpdate()
{
}

// �`��
void CBaseObj::PreDraw()
{
}
void CBaseObj::Draw()
{
}
void CBaseObj::DrawAlpha()
{
	// ���E��or���E�{�b�N�X�`��
	if (_pBS || _pBB) {
		LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
		pDevice->SetMaterial(&_mate);
		pDevice->SetTexture(0, NULL);
		D3DXVECTOR3 vCenter;
		D3DXVec3TransformCoord(&vCenter, &_vCenter, &_mWorld);
		D3DXMATRIX mWorld;
		D3DXMatrixTranslation(&mWorld, vCenter.x, vCenter.y, vCenter.z);
		pDevice->SetTransform(D3DTS_WORLD, &mWorld);
		if (_pBS)	// ���E��
			_pBS->DrawSubset(0);
		if (_pBB)	// AABB
			_pBB->DrawSubset(0);
	}
}
void CBaseObj::PostDraw()
{
}

// �I������
void CBaseObj::Fin()
{
}

// ���[���h�}�g���b�N�X�ݒ�
void CBaseObj::SetWorld(D3DXMATRIX* pWorld)
{
	if (pWorld) {
		_mWorld = *pWorld;
		return;
	}
	D3DXMatrixIdentity(&_mWorld);
}

// ���W�擾
D3DXVECTOR3 CBaseObj::GetPos()
{
	return D3DXVECTOR3(_mWorld._41, _mWorld._42, _mWorld._43);
}

// �E�����x�N�g���擾
D3DXVECTOR3 CBaseObj::GetRight()
{
	return D3DXVECTOR3(_mWorld._11, _mWorld._12, _mWorld._13);
}

// ������x�N�g���擾
D3DXVECTOR3 CBaseObj::GetUp()
{
	return D3DXVECTOR3(_mWorld._21, _mWorld._22, _mWorld._23);
}

// �O���x�N�g���擾
D3DXVECTOR3 CBaseObj::GetForward()
{
	return D3DXVECTOR3(_mWorld._31, _mWorld._32, _mWorld._33);
}

// ���W�ݒ�
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

// ���x�ݒ�
void CBaseObj::SetSpeed(D3DXVECTOR3* pSpeed)
{
	_vSpeed = (pSpeed) ? *pSpeed : D3DXVECTOR3(0, 0, 0);
}

// �����x�ݒ�
void CBaseObj::SetAccel(D3DXVECTOR3* pAccel)
{
	_vAccel = (pAccel) ? *pAccel : D3DXVECTOR3(0, 0, 0);
}

// �e�I�u�W�F�N�g�̐ݒ�
void CBaseObj::SetParent(CBaseObj* pParent)
{
	CBaseObj* pOldParent = _pParent;
	_pParent = pParent;
	if (pOldParent) {
		// �ڑ�����

	}
	if (_pParent) {
		// �V�K�ڑ�

	}
}

// ���ǂ����̓����蔻��
bool CBaseObj::CollisionSphere(CBaseObj* pObj)
{
	// TODO: �����ɋ��ǂ����̓����蔻����L�q.
	return false;
}

// AABB�ǂ����̓����蔻��
bool CBaseObj::CollisionAABB(CBaseObj* pObj)
{
	// TODO: ������AABB�ǂ����̓����蔻����L�q.
	return false;
}

// �f�o�b�O�\���p���E������
void CBaseObj::CreateBSphere()
{
	ReleaseBSphere();
	D3DXCreateSphere(CGraphics::GetDevice(), _fRadius, 16, 8, &_pBS, NULL);
}

// �f�o�b�O�\���p���E�����
void CBaseObj::ReleaseBSphere()
{
	if (_pBS) {
		_pBS->Release();
		_pBS = NULL;
	}
}

// �f�o�b�O�\���p���E�{�b�N�X����
void CBaseObj::CreateBBox()
{
	ReleaseBBox();
	D3DXCreateBox(CGraphics::GetDevice(),
		_vBBox.x * 2.0f, _vBBox.y * 2.0f, _vBBox.z * 2.0f, &_pBB, NULL);
}

// �f�o�b�O�\���p���E�{�b�N�X���
void CBaseObj::ReleaseBBox()
{
	if (_pBB) {
		_pBB->Release();
		_pBB = NULL;
	}
}

// �I�u�W�F�N�g����
CBaseObj* CBaseObj::Find(UINT uID, CBaseObj* p)
{
	if (!_pScene) return NULL;
	return _pScene->Find(uID, p);
}
