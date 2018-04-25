#include "BackEffect.h"
#include "GameWnd.h"
#include "Graphics.h"

struct Vtx1 {
	float x, y, z, u, v;
};


static float w = 1024;
static float h = 768;
// �X�N���[�����W�n��z�肵�����[�J�����W
Vtx1 vtx1[4] = {
	{ 0.0f,   0.0f, 0.0f, 0.0f, 0.0f },  // UV�ǉ�
	{ w,   0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, h, 0.0f, 0.0f, 1.0f },
	{ w, h, 0.0f, 1.0f, 1.0f }
};

Vtx1 vtx2[4] = {
	{ 0.0f,   0.0f, 0.0f, 0.0f, 0.0f },  // UV�ǉ�
	{ 200.0f,   0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 200.0f, 0.0f, 0.0f, 1.0f },
	{ 200.0f, 200.0f, 0.0f, 1.0f, 1.0f }
};

// 2D�`��p�ˉe�ϊ��s��
D3DXMATRIX proj1(
	2 / w, 0.0f, 0.0f, 0.0f,
	0.0f, -2 / h, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f
);


// ���_�錾�쐬
static IDirect3DVertexDeclaration9 *decl1 = 0;

ID3DXEffect *effect1 = 0;
ID3DXBuffer *error1 = 0;

CBackEffect::CBackEffect()
{
	m_pDevice = CGraphics::GetDevice();
	m_pTexture[0] = new CTexture(m_pDevice, "../data/TEXTURE/test1.png");
	m_pTexture[1] = new CTexture(m_pDevice, "../data/TEXTURE/spot.png");

	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj1);

	// �V�F�[�_�쐬
	if (FAILED(D3DXCreateEffectFromFile(m_pDevice, _T("../data/SHADER/test.fx"), 0, 0, 0, 0, &effect1, &error1))) {
		OutputDebugStringA((const char*)error1->GetBufferPointer());
		//return 0;
	}
	D3DVERTEXELEMENT9 elems[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(float) * 3, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(elems, &decl1);
}


CBackEffect::~CBackEffect()
{
}

void CBackEffect::Draw()
{
	UINT numPass = 0;


	effect1->SetTechnique("Tech");
	effect1->Begin(&numPass, 0);
	effect1->BeginPass(0);

	effect1->SetMatrix("proj", &proj1);
	effect1->SetTexture("tex", m_pTexture[0]->GetTexP());
	effect1->SetTexture("tex2", m_pTexture[1]->GetTexP());
	effect1->SetFloat("screenW", w);
	effect1->SetFloat("screenH", h);
	m_pDevice->SetVertexDeclaration(decl1);

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx1, sizeof(Vtx1));

	effect1->EndPass();
	effect1->End();
}
