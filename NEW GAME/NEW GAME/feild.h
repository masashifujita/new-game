#pragma once
#include "model.h"
#include "puyopuyo.h"


enum MapState{None = 0,PUYO};

class Feild {
public:
	//コンストラクタ
	Feild();
	//デストラクタ
	~Feild();
	//初期化。
	void Init(LPDIRECT3DDEVICE9 pd3dDevice);
	//更新。
	void Update(PuyoPuyo*);
	//描画。
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
	//開放。
	void Release();
	//座標を設定。
	void SetPosition(D3DXVECTOR3 pos);

	int GetMap(int, int);

	bool SetMap(int, int);

	void SetMapNull(int,int);

	void Con();

	void SetGameOverDec(bool flg){
		gameoverflg = flg;
	}

	bool Sakujo();

	void SetNumMap(int, int,int);

	int Conbine(int,int,int);

	int* GetNum()
	{
		return &Number[0][0];
	}

private:
	D3DXVECTOR3				position;		//座標。
	D3DXVECTOR3				scale;
	D3DXMATRIX				mWorld;			//ワールド行列。
	D3DXMATRIX				mRotation;		//回転行列。
	Model                   model;
	LPD3DXMESH				mesh;			//メッシュ。
	LPDIRECT3DTEXTURE9*		textures;		//テクスチャ。
	DWORD					numMaterial;	//マテリアルの数。
	ID3DXEffect*			effect;			//エフェクト。
	int						map[TATE][YOKO];
	bool					gameoverflg;
	int						Number[TATE][YOKO];
	bool					cmb[TATE][YOKO];
};