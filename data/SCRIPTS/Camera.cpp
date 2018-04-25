// �J���� �N���X����
#include "Camera.h"
#include "GameWnd.h"
#include "Input.h"

// �Œ�J�����ʒu
#define CAMERA_POSX		0.0f
#define CAMERA_POSY		0.0f
#define CAMERA_POSZ		-1.0f
// �Œ�J���������_
#define LOOK_POSX		0.0f
#define LOOK_POSY		0.0f
#define LOOK_POSZ		0.0f
/// �Œ�J����������x�N�g��
#define UP_DIRX			0.0f
#define UP_DIRY			1.0f
#define UP_DIRZ			0.0f

// �R���X�g���N�^
CCamera::CCamera(CScene* pScene) :
	CBaseObj(pScene),
	m_fFovy(FOVY),
	m_fAspect((float)SCREEN_WIDTH / SCREEN_HEIGHT),
	m_fZNear(NEAR_CLIP),
	m_fZFar(FAR_CLIP),
	m_cp(CP_TPVIEW),
	m_fTime(0.0f)
{
	_uID = ID_CAMERA;

	m_pos = D3DXVECTOR3(CAMERA_POSX, CAMERA_POSY, CAMERA_POSZ);
	m_pos2 = m_pos;
	m_at = D3DXVECTOR3(LOOK_POSX, LOOK_POSY, LOOK_POSZ);
	m_at2 = m_at;
	m_up = D3DXVECTOR3(UP_DIRX, UP_DIRY, UP_DIRZ);
	m_up2 = m_up;
}

// �f�X�g���N�^
CCamera::~CCamera(void)
{
}

// ������
bool CCamera::Init()
{
	bool bResult = CBaseObj::Init();
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, CAMERA_POSX, CAMERA_POSY, CAMERA_POSZ);
	SetWorld(&world);
	return bResult;
}

// �X�V
void CCamera::Update()
{
	// �������Ȃ�
}

// �X�V�㏈��
void CCamera::PostUpdate()
{
	D3DXVECTOR3 vX, vY, vZ, vP;

	m_fTime -= 1.0f / 60.0f;
	if (m_fTime < 0.0f) {
		m_fTime = 0.0f;
	}
	if (CInput::GetKeyTrigger(DIK_2) || CInput::GetKeyTrigger(DIK_NUMPAD2)) {
		m_cp = CP_FIXED;
		m_fTime = 1.0f;
	}
	if (CInput::GetKeyTrigger(DIK_1) || CInput::GetKeyTrigger(DIK_NUMPAD1)) {
		m_cp = CP_FPVIEW;
		m_fTime = 1.0f;
	}
	if (CInput::GetKeyTrigger(DIK_3) || CInput::GetKeyTrigger(DIK_NUMPAD3)) {
		m_cp = CP_TPVIEW;
		m_fTime = 1.0f;
	}
	/*if (m_pPlayer) {
		switch (m_cp) {
		case CP_FPVIEW:
			D3DXVec3TransformCoord(&m_pos2,
				&D3DXVECTOR3(0.0f, 480.0f, 100.0f),
				&m_pPlayer->GetWorld());
			D3DXVec3TransformCoord(&m_at2,
				&D3DXVECTOR3(0.0f, 480.0f, 1000.0f),
				&m_pPlayer->GetWorld());
			D3DXVec3TransformNormal(&m_up2,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				&m_pPlayer->GetWorld());
			break;
		case CP_TPVIEW:
			D3DXVec3TransformCoord(&m_pos2,
				&D3DXVECTOR3(0.0f, 500.0f, -800.0f),
				&m_pPlayer->GetWorld());
			D3DXVec3TransformCoord(&m_at2,
				&D3DXVECTOR3(0.0f, 400.0f, 0.0f),
				&m_pPlayer->GetWorld());
			D3DXVec3TransformNormal(&m_up2,
				&D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				&m_pPlayer->GetWorld());
			break;
		case CP_FIXED:
		default:
			m_at2 = m_pPlayer->GetPos();
			m_at2.y += 300.0f;
			m_up2 = D3DXVECTOR3(UP_DIRX, UP_DIRY, UP_DIRZ);
			break;
		}
		float fNew;
		fNew = 1.0f - m_fTime;
		m_pos = m_pos * m_fTime + m_pos2 * fNew;
		m_at = m_at * m_fTime + m_at2 * fNew;
		m_up = m_up * m_fTime + m_up2 * fNew;
		D3DXMatrixLookAtLH(&m_view, &m_pos, &m_at, &m_up);
		SetPos(&m_pos);
	}

	D3DXMatrixPerspectiveFovLH(&m_proj, m_fFovy, m_fAspect, m_fZNear, m_fZFar);*/
	//if (m_pSky) {
		//m_pSky->PostUpdate();
	//}
}

// �`��O����
void CCamera::PreDraw()
{
	LPDIRECT3DDEVICE9 pD = CGraphics::GetDevice();
	pD->SetTransform(D3DTS_PROJECTION, &m_proj);
	pD->SetTransform(D3DTS_VIEW, &m_view);
	//if (m_pSky) {
		//m_pSky->PreDraw();
	//}
}

// �`��
void CCamera::Draw()
{
	// �������Ȃ�
}

// �������`��
void CCamera::DrawAlpha()
{
	// �������Ȃ�
}

// �I������
void CCamera::Fin()
{
	// �Ȃ�
}