// カメラ クラス定義
#pragma once

#include "BaseObj.h"

enum ECamPos {
	CP_FIXED = 0,	// 固定
	CP_FPVIEW,		// 1人称
	CP_TPVIEW,		// 3人称

	MAX_VIEW
};

class CCamera : public CBaseObj
{
protected:
	D3DXMATRIX	m_proj;		// プロジェクションマトリックス
	D3DXMATRIX	m_view;		// ビューマトリックス

	ECamPos		m_cp;		// カメラ種別
	D3DXVECTOR3	m_pos;		// 視点
	D3DXVECTOR3	m_at;		// 注視点
	D3DXVECTOR3 m_up;		// 上方ベクトル
	D3DXVECTOR3	m_pos2;		// 最終視点
	D3DXVECTOR3	m_at2;		// 最終注視点
	D3DXVECTOR3 m_up2;		// 最終上方ベクトル
	float		m_fTime;	// 切り替え時間

	float		m_fFovy;	// 視野角
	float		m_fAspect;	// アスペクト比
	float		m_fZNear;	// 前方クリップ距離
	float		m_fZFar;	// 後方クリップ距離

public:
	CCamera(CScene* pScene);
	virtual ~CCamera(void);

	virtual bool Init();
	virtual void Update();
	virtual void Draw();
	virtual void DrawAlpha();
	virtual void Fin();
	void PostUpdate();		// 更新後処理
	void PreDraw();			// 描画前処理
};