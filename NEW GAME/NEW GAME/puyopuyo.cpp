#include "stdafx.h"
#include "puyopuyo.h"
#include "puyo.h"
#include "keyboard.h"
#include "feild.h"

#define SAFE_DELETE(instance) 		if (instance != NULL){delete instance; instance = NULL;}

extern KeyBoard g_keyboard;
extern Feild g_feild;


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

PuyoPuyo::~PuyoPuyo()
{
}

//ç∂âEÇ…à⁄ìÆÇ≥ÇπÇÈä÷êî
void PuyoPuyo::SideMove()
{
	for (short row = 0; row < MAX_HEIGHT; row++){
		for (short col = 0; col < MAX_WIDTH; col++){

			if (!puyo[row][col]->GetMoveFlg()){
				return;
			}
		}
	}
	if (g_keyboard.GetKeyTrigger(VK_LEFT))
	{
		for (short row = 0; row < MAX_HEIGHT; row++)
		{
			for (short col = 0; col < MAX_WIDTH; col++)
			{
				puyo[row][col]->Sub_X(col);
			}
		}
	}
	else if (g_keyboard.GetKeyTrigger(VK_RIGHT))
	{
		for (short row = 0; row < MAX_HEIGHT; row++) 
		{
			for (short col = 0; col < MAX_WIDTH; col++)
			{
				puyo[row][col]->Add_X(MAX_WIDTH - (col + 1));
			}
		}
	}
	//if (g_keyboard.GetKeyTrigger('K'))
	//{
	//	for (short row = 0; row < MAX_HEIGHT; row++)
	//	{
	//		for (short col = 0; col < MAX_WIDTH; col++)
	//		{
	//			puyo[row][col]->SetDead(true);
	//		}
	//	}
	//}
	//else if (g_keyboard.GetKeyTrigger('L'))
	//{
	//	for (short row = 0; row < MAX_HEIGHT; row++)
	//	{
	//		for (short col = 0; col < MAX_WIDTH; col++)
	//		{
	//			puyo[row][col]->SetDead(false);
	//		}
	//	}
	//}
}

void PuyoPuyo::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	rot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	for (short row = 0; row < MAX_HEIGHT; row++){
		for (short col = 0; col < MAX_WIDTH; col++)
		{
			this->puyo[row][col] = new Puyo;
			int pos_X = 0;
			pos_X = puyo[row][col]->GetiPos_X();
			pos_X += col;
			puyo[row][col]->SetiPos_X(pos_X);
			int pos_Y = puyo[row][col]->GetiPos_Y();
			pos_Y += row;
			puyo[row][col]->SetiPos_Y(pos_Y);
			int rnd;
			srand((unsigned int)time(NULL));					//randÇÃèâä˙âª
			rnd = rand() % NAMELIST_MAX;
			//if (strcmp("kyu_1.x", nameList[rnd]) == 0)
			//{
			//	num[row][col] = 1;
			//}
			//else if (strcmp("kyu_2.x", nameList[rnd]) == 0)
			//{
			//	num[row][col] = 2;
			//}
			//else if (strcmp("kyu_3.x", nameList[rnd]) == 0)
			//{
			//	num[row][col] = 3;
			//}
			//else if (strcmp("kyu_4.x", nameList[rnd]) == 0)
			//{
			//	num[row][col] = 4;
			//}
			//else if (strcmp("kyu_5.x", nameList[rnd]) == 0)
			//{
			//	num[row][col] = 5;
			//}
			//this->puyo[row][col]->Init(pd3dDevice, nameList[rnd]);

			num[row][col] = 1;
			this->puyo[row][col]->Init(pd3dDevice, nameList[1]);
		}
	}
}

void PuyoPuyo::Update()
{
	SideMove();



	bool MoveFlags[MAX_HEIGHT][MAX_WIDTH];
	for (short row = 0; row < MAX_HEIGHT;row++)
	{
		for (short col = 0; col < MAX_WIDTH;col++){
			if (!puyo[row][col]->GetDead()) 
			{
				puyo[row][col]->Update(num[row][col]);
				MoveFlags[row][col] = puyo[row][col]->GetMoveFlg();
			}
		}
	}

	IsDownEnd = false;
	for (short row = 0; row < MAX_HEIGHT; row++)
	{
		for (short col = 0; col < MAX_WIDTH; col++){
			if (MoveFlags[row][col] && !puyo[row][col]->GetDead()){
				IsDownEnd = true;
				break;
			}
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
	int numDiffuseLight,
	bool isDrawShadowMap,
	bool isRecieveShadow
	)
{
	for (short row = 0; row < MAX_HEIGHT; row++)
	{
		for (short col = 0; col < MAX_WIDTH; col++){
			if (!puyo[row][col]->GetDead()){
				puyo[row][col]->Render(
					pd3dDevice,
					viewMatrix,
					projMatrix,
					diffuseLightDirection,
					diffuseLightColor,
					ambientLight,
					numDiffuseLight,
					isDrawShadowMap,
					isRecieveShadow
					);
			}
		}
	}

}

void PuyoPuyo::Release()
{
	for (short row = 0; row < MAX_HEIGHT; row++)
	{
		for (short col = 0; col < MAX_WIDTH; col++){
			puyo[row][col]->Release();

			SAFE_DELETE(puyo[row][col]);
		}
	}
}