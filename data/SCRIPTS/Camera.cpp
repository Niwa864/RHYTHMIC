// カメラ クラス実装
#include "Camera.h"
#include "GameWnd.h"
#include "Input.h"

// 固定カメラ位置
#define CAMERA_POSX		0.0f
#define CAMERA_POSY		0.0f
#define CAMERA_POSZ		-1.0f
// 固定カメラ注視点
#define LOOK_POSX		0.0f
#define LOOK_POSY		0.0f
#define LOOK_POSZ		0.0f
/// 固定カメラ上方向ベクトル
#define UP_DIRX			0.0f
#define UP_DIRY			1.0f
#define UP_DIRZ			0.0f

// コンストラクタ
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

// デストラクタ
CCamera::~CCamera(void)
{
}

// 初期化
bool CCamera::Init()
{
	bool bResult = CBaseObj::Init();
	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, CAMERA_POSX, CAMERA_POSY, CAMERA_POSZ);
	SetWorld(&world);
	return bResult;
}

// 更新
void CCamera::Update()
{
	// 何もしない
}

// 更新後処理
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

// 描画前処理
void CCamera::PreDraw()
{
	LPDIRECT3DDEVICE9 pD = CGraphics::GetDevice();
	pD->SetTransform(D3DTS_PROJECTION, &m_proj);
	pD->SetTransform(D3DTS_VIEW, &m_view);
	//if (m_pSky) {
		//m_pSky->PreDraw();
	//}
}

// 描画
void CCamera::Draw()
{
	// 何もしない
}

// 半透明描画
void CCamera::DrawAlpha()
{
	// 何もしない
}

// 終了処理
void CCamera::Fin()
{
	// なし
}