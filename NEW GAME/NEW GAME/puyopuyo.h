#pragma once
#include <iostream>
#include <vector>

class Puyo;

#define MAX_HEIGHT 1	//puyoの個数(一列分)。
#define MAX_WIDTH 2			//puyoの個数(一行分)。
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

	bool GetIsDownEnd()
	{
		return IsDownEnd;
	}
	
	//puyoの先頭アドレスを取得する関数
	Puyo** GetPuyoArray()
	{
		return &puyo[0][0];
	}

private:
	Puyo*					puyo[MAX_HEIGHT][MAX_WIDTH];
	D3DXVECTOR3				position;		//座標。
	D3DXVECTOR3				movespeed;
	float					targetAngleY;
	D3DXQUATERNION			rot;
	bool					IsDownEnd;	// 落下終了か。
public:
	static const char nameList[NAMELIST_MAX][FILENAME_MAX];
private:
	//メンバ関数
	void SideMove();

};