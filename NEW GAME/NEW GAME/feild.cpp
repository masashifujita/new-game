#include "stdafx.h"
#include "feild.h"
#include "puyo.h"

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

//初期化。
void Feild::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for (short i = 0; i < TATE; i++)
	{
		for (short j = 0; j < YOKO; j++)
		{
			for (short k = 0; k < NANAME; k++)
			{
				map[i][j][k] = MapState::None;
			}
		}
	}
	model.Init(g_pd3dDevice, "court.x");
	gameoverflg = false;
}

//更新。
void Feild::Update(PuyoPuyo* puyopuyo)
{
	if (gameoverflg==true)
	{
		Puyo** puyo;
		puyo = puyopuyo->GetPuyoArray();
		for (short i = 0; i < 2; i++){
			if (GetMap(puyo[i]->GetiPos_X(), puyo[i]->GetiPos_Y(),puyo[i]->GetiPos_Z()) == MapState::PUYO)
			{
				// ゲームオーバー処理
				MessageBox(nullptr, ("ゲームはクリアされましたっと"), ("Message"), MB_OK);
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

int Feild::GetMap(int x, int y,int z)
{
	return map[y][x][z];
}

// マップの指定した位置にぷよを埋め込む関数
bool Feild::SetMap(int x, int y,int z)
{
	map[y][x][z] = MapState::PUYO;
	if (y <= 0){
		return true;
	}
	return false;
}
