#include "stdafx.h"
#include "player.h"

//�R���X�g���N�^
Unity::Unity()
{
	//�������B
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRotation);
	position.x = 0.0f;
	position.y = 0.8f;
	position.z = 0.0f;
}
//�f�X�g���N�^
Unity::~Unity()
{
	Release();
}
//���W��ݒ�B
void Unity::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
//�������B
void Unity::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	model.Init(g_pd3dDevice, "unitychan.x");
}
//�X�V�B
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

	//���[���h�s��̍X�V�B
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);

}


//�`��B
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


//�J���B
void Unity::Release()
{
	model.Release();
}