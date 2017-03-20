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

	bool GetMap(int, int);

	bool SetMap(int, int);

	bool SetMap(int, int, bool);

	void SetMapNull(int,int);

	int tansaku(int x, int y, int num);

	void SetGameOverDec(bool flg){
		gameoverflg = flg;
	}

	int* GetNum()
	{
		return &Number[0][0];
	}

	void SetPuyo(int x,int y,Puyo* puyo);

	void SetNullNum(int, int);

	void sakujo();

	void mapdown(int x,int y);

	bool A();


private:
	D3DXVECTOR3				position;		//座標。
	D3DXVECTOR3				scale;
	D3DXMATRIX				mWorld;						//ワールド行列。
	D3DXMATRIX				mRotation;					//回転行列。
	Model                   model;
	LPD3DXMESH				mesh;						//メッシュ。
	LPDIRECT3DTEXTURE9*		textures;					//テクスチャ。
	DWORD					numMaterial;				//マテリアルの数。
	ID3DXEffect*			effect;						//エフェクト。
	bool					map[TATE][YOKO];			//ぷよがあるかどうか確認する配列。あるとき:true / ないとき:false
	bool					gameoverflg;
	int						Number[TATE][YOKO];			//どの種類のぷよがあるのか確認する配列。
	bool					cmb[TATE][YOKO];			//探索済みかどうか調べる配列。済：true / 未：false
	bool					deadflg[TATE][YOKO];		//死亡フラグ。死亡：true / 生存：false
	Puyo*					PuyoMap[TATE][YOKO];
};