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
	//初期化。
	void Init(LPDIRECT3DDEVICE9 pd3dDevice,const char* model);
	//更新
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
	
	void SetNormalMap(LPDIRECT3DTEXTURE9 normalMap)
	{
		this->normalMap = normalMap;
	}
private:
	LPD3DXMESH				mesh;			//メッシュ。
	LPDIRECT3DTEXTURE9*		textures;		//テクスチャ。
	DWORD					numMaterial;	//マテリアルの数。
	ID3DXEffect*			effect;			//エフェクト。
	Camera*					camera;
	LPDIRECT3DTEXTURE9 normalMap = NULL;		//法線マップ。
};