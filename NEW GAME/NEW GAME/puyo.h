#pragma once
#include "model.h"
#include "feild.h"

class Puyo
{
public:
	//コンストラクタ
	Puyo();
	//デストラクタ
	~Puyo();
	//初期化。
	void Init(LPDIRECT3DDEVICE9,LPCSTR);
	//更新。
	void Update();
	//描画。
	void Render(
		LPDIRECT3DDEVICE9 pd3dDevice,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix,
		D3DXVECTOR4* diffuseLightDirection,
		D3DXVECTOR4* diffuseLightColor,
		D3DXVECTOR4	 ambientLight,
		int numDiffuseLight
		);
	//開放。
	void Release();

	void UpdateWorldMatrix(const D3DXVECTOR3& trans,const D3DXQUATERNION& rot,const D3DXVECTOR3& scale);

	void DownMove();

	float GetiPos_X()
	{
		return ipos_X;
	}

	float GetiPos_Y()
	{
		return ipos_Y;
	}

	void SetPos_X(int x)
	{
		ipos_X = x;
	}

	bool GetMoveFlg()
	{
		return downmoveflg;
	}

	//時間を管理する関数
	bool Time();

	void SideMove();


private:
	D3DXVECTOR3				position;		//座標。
	D3DXVECTOR3				movespeed;
	float					targetAngleY;
	D3DXQUATERNION			rot;
	Model                   model;
	LPD3DXMESH				mesh;			//メッシュ。
	LPDIRECT3DTEXTURE9*		textures;		//テクスチャ。
	DWORD					numMaterial;	//マテリアルの数。
	ID3DXEffect*			effect;			//エフェクト。
	const float				cTurnSpeed = D3DXToRadian(20.0f);
	float					turnSpeed = 0.0f;
	D3DXMATRIX				worldMatrix;										
	D3DXMATRIX				world;
	D3DXMATRIX				rotation;
	bool					downmoveflg;
	int						ipos_X;
	int						ipos_Y;
	int						ipos_Z;
	D3DXVECTOR2				lefttop;
	float					time;
	Feild*					feild;
	bool					checkdownflg;
	int						pos_X;
	int						pos_Y;
};