//=======================================================================================
//
//	  �Q�[���p�E�B���h�E �N���X��`
//
//=======================================================================================
#include "GameWnd.h"
#include "define.h"
#include "Input.h"

#pragma comment(lib, "imm32")

//---------------------------------------------------------------------------------------
//	�E�B���h�E������
//---------------------------------------------------------------------------------------
bool CGameWindow::InitInstance()
{
	m_pGraph = NULL;
	m_pScene = NULL;

	::ImmAssociateContext(GetHwnd(), NULL);

	if (!CWindow::InitInstance()) return false;

	m_bWindow = true;
	//int nID = MessageBox(_T("�E�B���h�E���[�h�Ŏ��s���܂����H"), _T("��ʃ��[�h"), MB_YESNOCANCEL);
	//if (nID == IDCANCEL) return false;
	//if (nID == IDYES) m_bWindow = true;

	::timeBeginPeriod(1);	// �^�C�}�̕���\���ŏ��ɃZ�b�g

							// �O���t�B�b�N�f�o�C�X������
	m_pGraph = CGraphics::Create(GetHwnd(), SCREEN_WIDTH, SCREEN_HEIGHT, m_bWindow);
	if (!m_pGraph) return false;

	// DirectInput������
	CInput::Init(GetHwnd());

	// �V�[��������
	m_pScene = CScene::Create(m_pGraph);
	if (!m_pScene) return false;
	m_pScene->InitObj();

	m_dwFPSLastTime = ::timeGetTime();
	m_dwExecLastTime = m_dwFPSLastTime - FRAME_RATE;// �K���ŏ��� m_pScene->Update() �����s
	m_dwFrameCount = 0;

	return true;
}

//---------------------------------------------------------------------------------------
//	�E�B���h�E���
//---------------------------------------------------------------------------------------
int CGameWindow::ExitInstance()
{
	// �V�[�����
	SAFE_RELEASE(m_pScene);

	// DirectInput���
	CInput::Fin();

	// �O���t�B�b�N �f�o�C�X���
	SAFE_RELEASE(m_pGraph);

	::timeEndPeriod(1);				// �^�C�}�̕���\�����ɖ߂�

	return CWindow::ExitInstance();	// ���s�t�@�C���̖߂�l
}

//---------------------------------------------------------------------------------------
//	�A�C�h��������
//---------------------------------------------------------------------------------------
bool CGameWindow::OnIdle(long lCount)
{
	// ���̕ӂŃt���[�����J�E���g
	DWORD dwCurrentTime = ::timeGetTime();			// ���݂̃^�C�}�l���擾
	if (dwCurrentTime - m_dwFPSLastTime >= 500) {	// 0.5 �b���ƂɌv��
													// �t���[�������v�Z
		if (m_pScene) {
			m_pScene->SetFPS(m_dwFrameCount * 1000 / (dwCurrentTime - m_dwFPSLastTime));
		}
		m_dwFPSLastTime = dwCurrentTime;	// �^�C�}�l���X�V
		m_dwFrameCount = 0;					// �t���[���J�E���^�����Z�b�g
	}
	// ���̕ӂŎ��ԊǗ�
	while (dwCurrentTime - m_dwExecLastTime >= FRAME_RATE)
	{	// ��莞�Ԃ��o�߂�����c
		m_dwExecLastTime += FRAME_RATE;							// �^�C�}�l���X�V
		if (m_pScene)
		{
			m_pScene->Update();									// �Q�[�� ���C������
		}


	}

	m_dwFrameCount++;					// �t���[���J�E���g�{�P

										// �p�\�R���̏������ƂɃ����_�����O������
	if (m_pScene) {
		m_pScene->Render();				// �����_�����O����
	}
	return true;
}

//---------------------------------------------------------------------------------------
//	WM_KEYDOWN �n���h��
//---------------------------------------------------------------------------------------
void CGameWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// [Esc] ��������Ă�����
	if (nChar == VK_END) {
		// �E�B���h�E�����
		OnClose();
	}
}

//---------------------------------------------------------------------------------------
//	WM_ERASEBKGND �n���h��
//---------------------------------------------------------------------------------------
bool CGameWindow::OnEraseBkgnd(HDC hdc)
{
	// �w�i�������Ȃ��ŏ����ς݂Ƃ��ĕԂ�
	return true;
}

//---------------------------------------------------------------------------------------
//	WM_SETCURSOR �n���h��
//---------------------------------------------------------------------------------------
bool CGameWindow::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	// �S��ʃ��[�h���A�}�E�X�J�[�\�����W���N���C�A���g�̈���Ȃ�
	if (!m_bWindow || nHitTest == HTCLIENT) {
		// �}�E�X�J�[�\������
		::SetCursor(NULL);
		return true;
	}
	return false;
}

//=======================================================================================
//	End of File
//=======================================================================================