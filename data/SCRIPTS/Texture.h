#pragma once

#include <d3dx9.h>		// �e�N�X�`���p
#include "BaseObj.h"

// ===============================================
// �\���̐錾
//================================================
typedef struct {
	int r;
	int g;
	int b;
	int a;
}COLOR;

typedef struct {
	D3DXMATRIX world;
	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffPolygon; // �e�N�X�`���o�b�t�@
	LPDIRECT3DTEXTURE9 pTextureFile;	// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 vPolygonPos;
	D3DXVECTOR3 vScale;
	COLOR   vCol;
	float vAngle;
	float fPolySizeX, fPolySizeY;
	float fPosXLeft, fPosXRight;		// U���W(0.0f ~ 1.0f)
	float fPosYUp, fPosYDown;			// V���W(0.0f ~ 1.0f)
	bool bAnime;						// false:�A�j���[�V�������Ȃ� true:�A�j���[�V��������

	// �A�j���[�V�����p
	int nPatternMax;				  // 1�A�j�����p�^�[����
	int nTexDivideX, nTexDivideY;	  // �e�N�X�`����������
	int nPatternNo;					  // �A�j���p�^�[��No
	int nMotionNo;					  // �v�����Y��
}Texture;

typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_2D;

class CTexture
{
private:
	Texture tex;

public:
	CTexture(LPDIRECT3DDEVICE9 _pDevice, char* _cTextureID);
	~CTexture();

	virtual bool Init(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vScale, float _vAngle, float _fSizeX, float _fSizeY, bool _bAnime, COLOR _Color = { 255, 255, 255, 255 },
		float _fFirstPointX = 0.0f, float _fLastPointX = 1.0f, float _fFirstPointY = 0.0f, float _fLastPoint = 1.0f);
	virtual void SetAnime(int _nPatternMax, int _nTexDivideX, int _nTexDivideY, int _nPatternNo, int _nMotionNo);
	virtual void SetTexOffSet(float fPosXLeft, float fPosYUp, float fPosXRight = 1.0f, float fPosYDown = 1.0f);
	virtual void SetColor(COLOR _Color) { tex.vCol = _Color; SetTexture();};

	virtual void Uninit();
	virtual void Update(D3DXVECTOR3 _vPos, float _fAngle, D3DXVECTOR3 scale = {1.0f, 1.0f, 0.0});
	virtual void Draw();
	virtual void Scroll(float _fMove);

	LPDIRECT3DTEXTURE9 GetTexP() { return tex.pTextureFile;};
	
private:
	HRESULT MakeVertex();
	void SetTexture();
};