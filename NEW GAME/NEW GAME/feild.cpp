#include "stdafx.h"
#include "feild.h"

//コンストラクタ
Feild::Feild()
{
	//初期化。
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRotation);
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
}
//デストラクタ
Feild::~Feild()
{
	Release();
}
//座標を設定。
void Feild::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
//初期化。
void Feild::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	model.Init(g_pd3dDevice, "court.x");
}
//更新。
void Feild::Update()
{
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
void Feild::Release()
{
	model.Release();
}