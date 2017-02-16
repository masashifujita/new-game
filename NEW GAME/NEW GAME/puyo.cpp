#include "stdafx.h"
#include "puyo.h"

extern Feild g_feild;

//コンストラクタ
Puyo::Puyo()
{
	//初期化。
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rotation);
	movespeed = D3DXVECTOR3(0.0f, 0.03f, 0.0f);
	lefttop = D3DXVECTOR2(0.0, TATE * GRIDSIZE);
	ipos_X = 0;
	ipos_Y = 0;
	ipos_Z = 0;
	rot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	checkdownflg = false;
	downmoveflg = true;
}

//デストラクタ
Puyo::~Puyo()
{
	Release();
}

void Puyo::UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
	D3DXMATRIX Trans, Scale;
	D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&Trans, trans.x, trans.y, trans.z);
	D3DXMatrixRotationQuaternion(&rotation, &rot);
	D3DXMATRIX Rot;
	D3DXMatrixRotationY(&Rot, targetAngleY);
	rotation = rotation*Rot;
	world = Scale*rotation*Trans;
}

//時間を管理する関数
bool Puyo::Time()
{
	time += 2.0f / 60.0f;
	if (time >= 1.0f)
	{
		time = 0.0f;			//時間のリセット
		return true;
	}
	return false;
}

//下に移動させる関数
void Puyo::DownMove()
{
	int movespeed_y = 1;
	if (Time() == true)
	{
		//何フレームかたったらここの処理を行う。
		ipos_Y += movespeed_y;

		// あたり判定
		if (feild->GetMap(ipos_X, ipos_Y,ipos_Z) == MapState::PUYO)
		{
			//現在のipos_Yの場所にpuyoがある場合の処理
			downmoveflg = false;

			//feild->SetMap(ipos_X,ipos_Y - 1):この処理でPuyoがあるipos_Yの一か所上にpuyoをセットする。
			feild->SetGameOverDec(feild->SetMap(ipos_X, ipos_Y - 1,ipos_Z));

			//updataでpositionを決める計算をするのでipos_Yを一か所上に移動させる必要がある。
			ipos_Y--;
		}
		else if (ipos_Y == TATE - 1)
		{
			//現在のipos_Yの場所が一番下の段だった場合の処理
			downmoveflg = false;

			feild->SetGameOverDec(feild->SetMap(ipos_X, ipos_Y,ipos_Z));
		}
	}
}

//右に移動させる関数
void Puyo::Add_X(short num)
{
	if (ipos_X + num < YOKO / 2)													//右側に行った時の判定
	{
		if (feild->GetMap(ipos_X + num + 1, ipos_Y,ipos_Z) != MapState::PUYO)		//右にpuyoがない時の判定
		{
			//右側にpuyoがない時puyopuyoを右に移動させる。
			ipos_X++;
		}
	}
}

//左に移動させる関数
void Puyo::Sub_X(short num)													//num :　現在のpuyoの要素番号？
{
	if (ipos_X - num > (YOKO / -2) + 1)													//左端に行った時の判定							
	{
		if (feild->GetMap(ipos_X - num - 1, ipos_Y,ipos_Z) != MapState::PUYO)		//左にpuyoがない時の判定
		{
			//左側にpuyoがない時puyopuyoを左に移動させる。
			ipos_X--;
		}	
	}
}

//後ろに1マス移動する関数
void Puyo::Add_Z()
{
	if (ipos_Z < NANAME / 2)
	{
		if (feild->GetMap(ipos_X, ipos_Y, ipos_Z + 1) != MapState::PUYO)
		{
			ipos_Z++;
		}
	}
}

//前に1マス移動する関数
void Puyo::Sub_Z()
{
	if (ipos_Z > (NANAME / -2) + 1){
		if (feild->GetMap(ipos_X, ipos_Y, ipos_Z - 1) != MapState::PUYO)		//左にpuyoがない時の判定
		{
			ipos_Z--;
		}
	}
}

void Puyo::Init(LPDIRECT3DDEVICE9 pd3dDevice,LPCSTR name)
{
	feild = &g_feild;

	model.Init(pd3dDevice,name);
}

void Puyo::Update()
{
	//DownMove()関数呼び出しm	
	DownMove();


	//ここでiposをpositionに変換する。
	position.x = ipos_X * GRIDSIZE + GRIDSIZE * 0.5;
	position.y = lefttop.y - (ipos_Y * GRIDSIZE) - GRIDSIZE * 0.5;
	position.z = ipos_Z;

	//ワールド行列の更新。
	UpdateWorldMatrix(position, rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

//描画。
void Puyo::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight
	)
{
	model.Render(pd3dDevice,
		viewMatrix,
		projMatrix,
		diffuseLightDirection,
		diffuseLightColor,
		ambientLight,
		numDiffuseLight,
		world,
		rotation
		);
}

//開放。
void Puyo::Release()
{
	model.Release();
}