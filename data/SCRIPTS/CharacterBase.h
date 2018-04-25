#pragma once

#include "BaseObj.h"
#include "Texture.h"
#include "Graphics.h"
#include "Utility.h"
#include <vector>

class CCharacterBase :public CBaseObj
{
protected:

	CTexture* m_pTexture;
	CHARASTATUS  m_CharaStatus;
	LPDIRECT3DDEVICE9 m_pDevice;

	virtual void GetKey() = 0;

	
private:

	float D3DXVec2Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2) {
		return v1.x * v2.y - v1.y * v2.x;
	};

public:
	CCharacterBase(CScene* pScene);
	~CCharacterBase();

	static bool m_bMoveFlag;
	static std::vector<bool> m_vFlagVector;
	static std::vector<bool> m_vEnemyFlag;

	bool ColSegments(
		const Segment &seg1,          // 線分1
		const Segment &seg2,          // 線分2
		//float* outT1 = 0,			  // 線分1の内分比（出力）
		//float* outT2 = 0,			  // 線分2の内分比（出力)
		D3DXVECTOR3* outPos = nullptr // 交点（出力）
	);
};