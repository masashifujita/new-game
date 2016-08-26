#include "stdafx.h"
#include "player.h"

//コンストラクタ
Unity::Unity()
{
	//初期化。
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRotation);
	position.x = 0.0f;
	position.y = 0.8f;
	position.z = 0.0f;
}
//デストラクタ
Unity::~Unity()
{
	Release();
}
//座標を設定。
void Unity::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
//初期化。
void Unity::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	model.Init(g_pd3dDevice, "unitychan.x");
}
//更新。
void Unity::Update()
{
	if (GetAsyncKeyState(VK_UP))
	{
		position.z += 0.5f;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		position.z -= 0.5f;
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		position.x -= 0.5f;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		position.x += 0.5f;
	}

	//ワールド行列の更新。
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);

}


//描画。
void Unity::Render(
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
		mWorld,
		mRotation);
}


//開放。
void Unity::Release()
{
	model.Release();
}