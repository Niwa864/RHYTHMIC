#include "CharacterBase.h"
#include "Character.h"

bool CCharacterBase::m_bMoveFlag = false;
std::vector<bool>CCharacterBase::m_vFlagVector;
std::vector<bool>CCharacterBase::m_vEnemyFlag;

CCharacterBase::CCharacterBase(CScene* pScene) : CBaseObj(pScene)
{
	_uID = ID_CHARBASE;
}


CCharacterBase::~CCharacterBase()
{
}

bool CCharacterBase::ColSegments(const Segment &seg1, const Segment &seg2, /*float *outT1, float *outT2, */D3DXVECTOR3 *outPos)
{

	D3DXVECTOR3 v = seg2.start - seg1.start;
	float Crs_v1_v2 = D3DXVec2Cross(seg1.vec, seg2.vec);
	if (Crs_v1_v2 == 0.0f) {
		// ïΩçsèÛë‘
		return false;
	}

	float Crs_v_v1 = D3DXVec2Cross(v, seg1.vec);
	float Crs_v_v2 = D3DXVec2Cross(v, seg2.vec);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	/*if (outT1)
		*outT1 = Crs_v_v2 / Crs_v1_v2;
	if (outT2)
		*outT2 = Crs_v_v1 / Crs_v1_v2;*/

	const float eps = 0.001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
		// åç∑ÇµÇƒÇ¢Ç»Ç¢
		return false;
	}

	if (outPos)
		*outPos = seg1.start + seg1.vec * t1;

	return true;
}