//======================================================================================
//
// テクスチャ基底クラス[Texture.cpp]
//
//======================================================================================

#include "Texture.h"

#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット(画面上座標 | 頂点カラー | テクスチャ座標)

//=====定数定義=====
// 共有定数は#define
// cppオンリーの場合はconst

const int NUM_VERTEX = 4;			// 頂点数
const int NUM_POLYGON = 2;			// ポリゴン数


CTexture::CTexture(LPDIRECT3DDEVICE9 _pDevice, char* _cTextureID)
{
	tex.pDevice = _pDevice;

	D3DXCreateTextureFromFile(tex.pDevice,						// デバイスへのポインタ
							  _cTextureID,					// ファイル名
							  &tex.pTextureFile);	// 読み込み
}

CTexture::~CTexture(){
}

bool CTexture::Init(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vScale, float _vAngle, 
				    float _fSizeX, float _fSizeY, bool _bAnime, 
					COLOR _Color, float _fFirstPointX, float _fLastPointX, float _fFirstPointY, float _fLastPointY)
{
	tex.vPolygonPos = _vPos;
	tex.vScale = _vScale;
	tex.vAngle = _vAngle;
	tex.fPolySizeX = _fSizeX;
	tex.fPolySizeY = _fSizeY;
	tex.fPosXLeft = _fFirstPointX;
	tex.fPosXRight = _fLastPointX;
	tex.fPosYUp = _fFirstPointY;
	tex.fPosYDown = _fLastPointY;
	tex.bAnime = _bAnime;
	tex.vCol = _Color;
	MakeVertex();

	return true;
}

void CTexture::SetAnime(int _nPatternMax, int _nTexDivideX, int _nTexDivideY, int _nPatternNo, int _nMotionNo)
{
	tex.nPatternMax = _nPatternMax;
	tex.nTexDivideX = _nTexDivideX;
	tex.nTexDivideY = _nTexDivideY;
	tex.nPatternNo = _nPatternNo;
	tex.nMotionNo = _nMotionNo;

	SetTexture();
}

void CTexture::SetTexOffSet(float fPosXLeft, float fPosYUp, float fPosXRight, float fPosYDown)
{
	tex.fPosXLeft = fPosXLeft;
	tex.fPosXRight = fPosXRight;
	tex.fPosYUp = fPosYUp;
	tex.fPosYDown = fPosYDown;

}

void CTexture::Uninit(){
	tex.pTextureFile->Release();
	tex.pDevice->Release();
}

void CTexture::Update(D3DXVECTOR3 _vPos, float _fAngle, D3DXVECTOR3 scale)
{
	tex.vPolygonPos = _vPos;
	tex.vAngle = _fAngle;
	tex.vScale = scale;
	SetTexture();
}

void CTexture::Draw()
{
	// 頂点バッファをデバイスのデータストリームにバインド
	tex.pDevice->SetStreamSource(0, tex.pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	tex.pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定

	tex.pDevice->SetTexture(0, tex.pTextureFile);		// テクスチャ設定
	

	//----- ポリゴン描画 -----
	tex.pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

void CTexture::Scroll(float _fMove)
{
	tex.fPosXLeft += _fMove;
	tex.fPosXRight += _fMove;

	if (tex.fPosXRight < 0.0f)
	{
		tex.fPosXRight += 1.0f;
		tex.fPosXLeft += 1.0f;
	}
	
	if (tex.fPosXLeft > 1.0f)
	{
		tex.fPosXRight -= 1.0f;
		tex.fPosXLeft -= 1.0f;
	}

	VERTEX_2D	*pVtx;

	tex.pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得

	//----- テクスチャ座標の設定 -----
	pVtx[0].tex = D3DXVECTOR2(tex.fPosXLeft, tex.fPosYUp);
	pVtx[1].tex = D3DXVECTOR2(tex.fPosXRight, tex.fPosYUp);
	pVtx[2].tex = D3DXVECTOR2(tex.fPosXLeft, tex.fPosYDown);
	pVtx[3].tex = D3DXVECTOR2(tex.fPosXRight, tex.fPosYDown);

	//----- 頂点データのアンロック -----
	tex.pVtxBuffPolygon->Unlock();
}

HRESULT CTexture::MakeVertex() 
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(tex.pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,			  // バッファ使用法
		FVF_VERTEX_2D,				  // 頂点フォーマット
		D3DPOOL_MANAGED,				  // 保持するメモリクラス
		&tex.pVtxBuffPolygon,	  // 頂点バッファインターフェース
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファの内容を埋める
	{
		VERTEX_2D	*pVtx;

		//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		tex.pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		//----- 頂点座標の設定 -----
		pVtx[0].pos = D3DXVECTOR3(tex.vPolygonPos.x					, tex.vPolygonPos.y, tex.vPolygonPos.z);
		pVtx[1].pos = D3DXVECTOR3(tex.vPolygonPos.x + tex.fPolySizeX, tex.vPolygonPos.y, tex.vPolygonPos.z);
		pVtx[2].pos = D3DXVECTOR3(tex.vPolygonPos.x					, tex.vPolygonPos.y + tex.fPolySizeY, tex.vPolygonPos.z);
		pVtx[3].pos = D3DXVECTOR3(tex.vPolygonPos.x + tex.fPolySizeX, tex.vPolygonPos.y + tex.fPolySizeY, tex.vPolygonPos.z);

		//----- rhw設定 -----
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		//----- 頂点カラー設定 -----
		pVtx[0].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);
		pVtx[1].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);
		pVtx[2].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);
		pVtx[3].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);

		//----- テクスチャ座標の設定 -----
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//----- 頂点データのアンロック -----
		tex.pVtxBuffPolygon->Unlock();

	}
	return S_OK;
}

void CTexture::SetTexture()
{
	VERTEX_2D	*pVtx;

	tex.pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得

	D3DXVECTOR3 vCenter = D3DXVECTOR3(tex.vPolygonPos.x + tex.fPolySizeX / 2.0f, tex.vPolygonPos.y + tex.fPolySizeX / 2.0f, 0.0f );

	for (int VtxLoop = 0; VtxLoop < NUM_VERTEX; VtxLoop++)
		pVtx[VtxLoop].pos = vCenter;

	//----- 頂点座標の設定 -----
	pVtx[0].pos += D3DXVECTOR3((-tex.fPolySizeX / 2.0f) * tex.vScale.x * cosf(tex.vAngle) - (-tex.fPolySizeY / 2.0f) * tex.vScale.y * sinf(tex.vAngle),
							   (-tex.fPolySizeX / 2.0f) * tex.vScale.x * sinf(tex.vAngle) + (-tex.fPolySizeY / 2.0f) * tex.vScale.y * cosf(tex.vAngle), 0.0f);

	pVtx[1].pos += D3DXVECTOR3(( tex.fPolySizeX / 2.0f) * tex.vScale.x * cosf(tex.vAngle) - (-tex.fPolySizeY / 2.0f) * tex.vScale.y * sinf(tex.vAngle),
							   ( tex.fPolySizeX / 2.0f) * tex.vScale.x * sinf(tex.vAngle) + (-tex.fPolySizeY / 2.0f) * tex.vScale.y * cosf(tex.vAngle), 0.0f);

	pVtx[2].pos += D3DXVECTOR3((-tex.fPolySizeX / 2.0f) * tex.vScale.x * cosf(tex.vAngle) - ( tex.fPolySizeY / 2.0f) * tex.vScale.y * sinf(tex.vAngle),
							   (-tex.fPolySizeX / 2.0f) * tex.vScale.x * sinf(tex.vAngle) + ( tex.fPolySizeY / 2.0f) * tex.vScale.y * cosf(tex.vAngle), 0.0f);

	pVtx[3].pos += D3DXVECTOR3(( tex.fPolySizeX / 2.0f) * tex.vScale.x * cosf(tex.vAngle) - ( tex.fPolySizeY / 2.0f) * tex.vScale.y * sinf(tex.vAngle),
							   ( tex.fPolySizeX / 2.0f) * tex.vScale.x * sinf(tex.vAngle) + ( tex.fPolySizeY / 2.0f) * tex.vScale.y * cosf(tex.vAngle), 0.0f);


	/*pVtx[0].pos = D3DXVECTOR3(tex.vPolygonPos.x, tex.vPolygonPos.y, tex.vPolygonPos.z);
	pVtx[1].pos = D3DXVECTOR3(tex.vPolygonPos.x + tex.fPolySizeX, tex.vPolygonPos.y, tex.vPolygonPos.z);
	pVtx[2].pos = D3DXVECTOR3(tex.vPolygonPos.x, tex.vPolygonPos.y + tex.fPolySizeY, tex.vPolygonPos.z);
	pVtx[3].pos = D3DXVECTOR3(tex.vPolygonPos.x + tex.fPolySizeX, tex.vPolygonPos.y + tex.fPolySizeY, tex.vPolygonPos.z);
	*/
	//----- rhw設定 -----
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	//----- 頂点カラー設定 -----
	pVtx[0].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);
	pVtx[1].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);
	pVtx[2].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);
	pVtx[3].col = D3DCOLOR_RGBA(tex.vCol.r, tex.vCol.g, tex.vCol.b, tex.vCol.a);

	if (tex.bAnime) // 切り替え時でなくても入ってしまう　軽量化するならココをなおすこと
	{
		//----- テクスチャ座標の算出 -----   
		float fPatternSizeX, fPatternSizeY;
		fPatternSizeX = 1.0f / tex.nTexDivideX;
		fPatternSizeY = 1.0f / tex.nTexDivideY;
		tex.fPosXLeft = (float)(tex.nPatternNo % tex.nTexDivideX) * fPatternSizeX;
		tex.fPosXRight = (float)(tex.nPatternNo % tex.nTexDivideX) * fPatternSizeX + fPatternSizeX;
		tex.fPosYUp = (float)tex.nMotionNo * fPatternSizeY;
		tex.fPosYDown = (float)tex.nMotionNo * fPatternSizeY + fPatternSizeY;
	}

	//----- テクスチャ座標の設定 -----
	pVtx[0].tex = D3DXVECTOR2(tex.fPosXLeft,  tex.fPosYUp);
	pVtx[1].tex = D3DXVECTOR2(tex.fPosXRight, tex.fPosYUp);
	pVtx[2].tex = D3DXVECTOR2(tex.fPosXLeft,  tex.fPosYDown);
	pVtx[3].tex = D3DXVECTOR2(tex.fPosXRight, tex.fPosYDown);

	//----- 頂点データのアンロック -----
	tex.pVtxBuffPolygon->Unlock();
}