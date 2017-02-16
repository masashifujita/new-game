#include "stdafx.h"
#include "puyopuyo.h"
#include "puyo.h"


#define SAFE_DELETE(instance) 		if (instance != NULL){delete instance; instance = NULL;}

const char PuyoPuyo::nameList[NAMELIST_MAX][FILENAME_MAX] = { 
	"kyu_1.x",
	"kyu_2.x",
	"kyu_5.x",
	"kyu_3.x",
	"kyu_4.x",
};


PuyoPuyo::PuyoPuyo()
{
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

PuyoPuyo::~PuyoPuyo(){}


void PuyoPuyo::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	rot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	for (short i = 0; i < NUM; i++)
	{
		this->puyo[i] = new Puyo;
		float pos = 0.0f;
		pos = puyo[i]->GetiPos_X();
		pos += 1.0f * i;
		puyo[i]->SetPos_X(pos);
		int rnd;
		srand((unsigned int)time(NULL));					//rand‚Ì‰Šú‰»
		rnd = rand() % NAMELIST_MAX;
		this->puyo[i]->Init(pd3dDevice,nameList[rnd]);
		//this->puyo[i]->Init(pd3dDevice, nameList[i]);
	}
}

void PuyoPuyo::Update()
{
	for (short i = 0; i < NUM; i++)
	{
		this->puyo[i]->Update();
		if (puyo[i]->GetMoveFlg() == false)
		{
			downflg = true;
		}
	}
}

void PuyoPuyo::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight
	)
{
	for (short i = 0; i < NUM; i++)
	{
		this->puyo[i]->Render(
			pd3dDevice,
			viewMatrix,
			projMatrix,
			diffuseLightDirection,
			diffuseLightColor,
			ambientLight,
			numDiffuseLight
			);
	}

}

void PuyoPuyo::Release()
{
	for (short i = 0; i < NUM; i++)
	{
		SAFE_DELETE(puyo[i]);
	}
}