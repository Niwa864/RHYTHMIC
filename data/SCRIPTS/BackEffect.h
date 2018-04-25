#pragma once
#include "Texture.h"

class CBackEffect
{
private:
	LPDIRECT3DDEVICE9 m_pDevice;
	CTexture* m_pTexture[2];

public:
	CBackEffect();
	~CBackEffect();
	void Draw();
};

