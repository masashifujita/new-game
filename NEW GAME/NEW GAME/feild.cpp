#include "stdafx.h"
#include "feild.h"

//�R���X�g���N�^
Feild::Feild()
{
	//�������B
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRotation);
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
}
//�f�X�g���N�^
Feild::~Feild()
{
	Release();
}
//���W��ݒ�B
void Feild::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
//�������B
void Feild::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	model.Init(g_pd3dDevice, "court.x");
}
//�X�V�B
void Feild::Update()
{
	//���[���h�s��̍X�V�B
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);

}
//�`��B
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
//�J���B
void Feild::Release()
{
	model.Release();
}