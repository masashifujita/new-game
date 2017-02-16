#pragma once

class Puyo;

#define NUM 2
#define NAMELIST_MAX 5	// Puyoクラスのモデル読み込み時に使用するファイル名のリストの要素数

class PuyoPuyo
{
public:
	//コンストラクタ
	PuyoPuyo();
	//デストラクタ
	~PuyoPuyo();
	//初期化。
	void Init(LPDIRECT3DDEVICE9);
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

	D3DXVECTOR3 GetPos()
	{
		return position;
	}

	bool GetDownFlg()
	{
		return downflg;
	}
	
	//puyoの先頭アドレスを取得する関数
	Puyo** GetPuyoArray()
	{
		return puyo;
	}

private:
	Puyo*					puyo[NUM];
	D3DXVECTOR3				position;		//座標。
	D3DXVECTOR3				movespeed;
	float					targetAngleY;
	D3DXQUATERNION			rot;
	bool					downflg;
	
public:
	static const char nameList[NAMELIST_MAX][FILENAME_MAX];
};