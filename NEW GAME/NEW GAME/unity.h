#pragma once
#include "camera.h"
#include "shadowmap.h"
#include "light.h"

extern Camera					g_camera;
extern ShadowMap				g_shadowmap;
extern Light					light;


class Unity
{
public:
	Unity();
	~Unity();
	void Init(LPDIRECT3DDEVICE9);
	void Update();
	void Render(
		LPDIRECT3DDEVICE9 pd3dDevice,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix,
		D3DXVECTOR4* diffuseLightDirection,
		D3DXVECTOR4* diffuseLightColor,
		D3DXVECTOR4	 ambientLight,
		int numDiffuseLight,
		bool isDrawShadowMap,
		bool isRecieveShadow
		);
	void Release();
	void UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale);
	void SetEffect(ID3DXEffect** pEffect){
		effect = *pEffect;
	}
private:
	D3DXVECTOR3				position;		//���W�B
	D3DXQUATERNION			rot;
	D3DXVECTOR3				scale;
	LPD3DXMESH				mesh;			//���b�V���B
	LPDIRECT3DTEXTURE9*		textures;		//�e�N�X�`���B
	DWORD					numMaterial;	//�}�e���A���̐��B
	ID3DXEffect*			effect = nullptr;			//�G�t�F�N�g�B
	D3DXMATRIX				worldMatrix;
	D3DXMATRIX				world;
	D3DXMATRIX				rotation;
	LPDIRECT3DTEXTURE9		normalMap = NULL;		//�@���}�b�v�B
	Camera*                 camera;
	float					targetAngleY = -55.0f;
	LPDIRECT3DTEXTURE9		n_map;
};