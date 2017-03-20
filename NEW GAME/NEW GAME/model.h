#pragma once
#include "system.h"
#include "camera.h"
#include "shadowmap.h"

extern ShadowMap g_shadowmap;

class Model
{
public:
	Model();
	~Model();
	//�������B
	void Init(LPDIRECT3DDEVICE9 pd3dDevice,const char* model);
	//�X�V
	void Update();
	void Render(
		LPDIRECT3DDEVICE9 pd3dDevice,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix,
		D3DXVECTOR4* diffuseLightDirection,
		D3DXVECTOR4* diffuseLightColor,
		D3DXVECTOR4	 ambientLight,
		int numDiffuseLight,
		D3DXMATRIX	mWorld,
		D3DXMATRIX	mRotation,
		bool isDrawShadowMap,
		bool isRecieveShadow
		);
	LPD3DXMESH GetMesh()
	{
		return mesh;
	}
	void Release();
	
	void UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale);

private:
	LPD3DXMESH				mesh;			//���b�V���B
	LPDIRECT3DTEXTURE9*		textures;		//�e�N�X�`���B
	DWORD					numMaterial;	//�}�e���A���̐��B
	ID3DXEffect*			effect;			//�G�t�F�N�g�B
	Camera*					camera;
	LPDIRECT3DTEXTURE9		normalMap = NULL;		//�@���}�b�v�B
	D3DXMATRIX				worldMatrix;	//!<���[���h�s��B
	D3DXMATRIX				rotationMatrix;	//!<��]�s��B

};