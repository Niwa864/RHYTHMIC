#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

typedef struct{
	D3DXVECTOR3 start; // �n�_
	D3DXVECTOR3 vec; // �����x�N�g���i�����̒������S���̂Ő��K�����Ȃ��悤�ɁI�j
}Segment;

typedef struct {
	D3DXMATRIX world;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;

	float angle;
	float gravity;
	float accel;
	float jump;

	int	  Hp;
	int   Attack;

	bool Dead;
}CHARASTATUS;

typedef struct
{
	float firstX;
	float firstY;
	float lastX;
	float lastY;
}TextureOffset;


typedef enum {
	GAME_TITLE = 0,
	GAME_SELECT,
	GAME_CHARSELECT,
	GAME_MAIN,
	GAME_CLEAR,
	GAME_OVER,

	GAME_MODE_MAX
}GAME_MODE;