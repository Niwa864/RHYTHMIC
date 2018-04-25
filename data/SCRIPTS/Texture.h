#pragma once

#include <d3dx9.h>		// テクスチャ用
#include "BaseObj.h"

// ===============================================
// 構造体宣言
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
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffPolygon; // テクスチャバッファ
	LPDIRECT3DTEXTURE9 pTextureFile;	// テクスチャへのポインタ
	D3DXVECTOR3 vPolygonPos;
	D3DXVECTOR3 vScale;
	COLOR   vCol;
	float vAngle;
	float fPolySizeX, fPolySizeY;
	float fPosXLeft, fPosXRight;		// U座標(0.0f ~ 1.0f)
	float fPosYUp, fPosYDown;			// V座標(0.0f ~ 1.0f)
	bool bAnime;						// false:アニメーションしない true:アニメーションする

	// アニメーション用
	int nPatternMax;				  // 1アニメ分パターン数
	int nTexDivideX, nTexDivideY;	  // テクスチャ内分割数
	int nPatternNo;					  // アニメパターンNo
	int nMotionNo;					  // 要するにY軸
}Texture;

typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float		rhw;	// テクスチャのパースペクティブコレクト
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
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