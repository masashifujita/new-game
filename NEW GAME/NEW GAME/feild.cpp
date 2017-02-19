#include "stdafx.h"
#include "feild.h"
#include "puyo.h"
#include "puyopuyo.h"
#include "keyboard.h"

extern Puyo puyo;
extern PuyoPuyo* nowPuyoPuyo;
extern KeyBoard g_keyboard;


//コンストラクタ
Feild::Feild()
{
	//初期化。
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRotation);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//デストラクタ
Feild::~Feild()
{
	Release();
}

//void Feild::Con()
//{
//	if (g_keyboard.GetKeyTrigger('A'))
//	{
//		for (short y = TATE - 1; y > 0; y--)
//		{
//			for (short x = 0; x < YOKO; x++)
//			{
//				map[y][x] = NULL;
//				Number[y][x] = NULL;
//				puyo.isDead = true;
//			}
//		}
//	}
//}


//ぷよが何個つながっているか探索する関数
int Feild::Conbine(int x,int y,int num)
{
	//調べたい位置のpuyoの種類が違う又はすでにチェック済みの場合0を返す。
	if (Number[y][x] != num || cmb[y][x] != 0)
	{
		return 0;
	}


	cmb[y][x] = 1;
	int ret = 1;

	if (y > 0)
	{
		ret += Conbine(x + 1, y, num);
		ret += Conbine(x - 1, y, num);
	}
	return ret;
}

//初期化。
void Feild::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for (short i = 0; i < TATE; i++)
	{
		for (short j = 0; j < YOKO; j++)
		{
			map[i][j] = MapState::None;
			Number[i][j] = MapState::None;
			cmb[i][j] = false;
		}
	}
	model.Init(g_pd3dDevice, "court.x");
	gameoverflg = false;
}

//更新。
void Feild::Update(PuyoPuyo* puyopuyo)
{
	//for (short y = TATE - 1; y > 0; y--)
	//{
	//	for (short x = 0; x < YOKO; x++)
	//	{
	//		int ret = 0, num = 0;
	//		num = Number[y][x];
	//		ret = Conbine(x, y,num);
	//	}
	//}

	
	for (short y = TATE - 1; y > 0; y--)
	{
		for (short x = 0; x < YOKO; x++)
		{
			if (Number[y][x] != 0)
			{
				int num;
				num = Number[y][x];
				int cnt = Conbine(x, y, num);
				if (cnt >= 0)
				{
					puyo.isDead = true;
				}
			}
		}
	}

	if (gameoverflg==true)
	{
		Puyo** puyo;
		puyo = puyopuyo->GetPuyoArray();
		for (short i = 0; i < 2; i++){
			if (GetMap(puyo[i]->GetiPos_X(), puyo[i]->GetiPos_Y()) == MapState::PUYO)
			{
				// ゲームオーバー処理
				MessageBox(nullptr, ("げーむおーばー"), ("Message"), MB_OK);
				abort();
				break;
			}
		}
	}

	//ワールド行列の更新。
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);
}

//描画。

void Feild::Render(
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
	model.Render(pd3dDevice,
		viewMatrix,
		projMatrix,
		diffuseLightDirection,
		diffuseLightColor,
		ambientLight,
		numDiffuseLight,
		mWorld,
		mRotation,
		isDrawShadowMap,
		isRecieveShadow
		);
}

//開放。
void Feild::Release()
{
	model.Release();
}

int Feild::GetMap(int x, int y)
{
	return map[y][x];
}

// マップの指定した位置にぷよを埋め込む関数
bool Feild::SetMap(int x, int y)
{
	map[y][x] = MapState::PUYO;
	if (y <= 0){
		return true;
	}
	return false;
}


void Feild::SetMapNull(int x, int y)
{
	map[y][x] = MapState::None;
}

void Feild::SetNumMap(int x, int y,int num)
{
	Number[y][x] = num;
}

