#include "stdafx.h"
#include "camera.h"
#include "puyopuyo.h"
#include "feild.h"
#include "light.h"
#include "system.h"
#include <list>


Camera g_camera;				//カメラ。
std::list<PuyoPuyo*>	puyopuyoList;	//ぷよぷよのリスト。
PuyoPuyo* nowPuyoPuyo = NULL;
PuyoPuyo* nextPuyoPuyo = NULL;
Feild g_feild;
Light light;
int puyopuyoTimer = 120;

void Init()
{
	//ライトを初期化。
	light.Init();
	
	//カメラの初期化。
	g_camera.Init();
	//フィールドの初期化
	g_feild.Init(g_pd3dDevice);
}

void Render()
{
	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//シーンの描画開始。
	g_pd3dDevice->BeginScene();
	//プレーヤーを描画。
	for (auto& puyopuyo : puyopuyoList)
	{
		puyopuyo->Render(g_pd3dDevice,
			g_camera.GetViewMatrix(),
			g_camera.GetProjectionMatrix(),
			light.GetLightDirection(),
			light.GetLightColor(),
			light.GetambientLight(),
			LIGHT_NUM
			);
	}
	 

	//フィールドを描画
	g_feild.Render(g_pd3dDevice,
		g_camera.GetViewMatrix(),
		g_camera.GetProjectionMatrix(),
		light.GetLightDirection(),
		light.GetLightColor(),
		light.GetambientLight(),
		LIGHT_NUM
		);

	// シーンの描画終了。
	g_pd3dDevice->EndScene();
	// バックバッファとフロントバッファを入れ替える。
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Update()
{

	if (nowPuyoPuyo == NULL)
	{
		PuyoPuyo* puyopuyo = new PuyoPuyo;
		nowPuyoPuyo = puyopuyo;
		puyopuyo->Init(g_pd3dDevice);
		puyopuyoList.push_back(puyopuyo);
	}
	
	//ライトの更新。
	light.Update();
	
	//プレーヤーの更新。
	nowPuyoPuyo->Update();
	//フィールドの更新
	g_feild.Update(nowPuyoPuyo);
	if (nowPuyoPuyo->GetDownFlg() == true){
		nowPuyoPuyo = NULL;
	}
	//カメラの更新
	g_camera.Update();
}
void Terminate()
{
	//プレーヤーの解放
	for (auto& puyopuyo : puyopuyoList){
		puyopuyo->Release();
		delete(puyopuyo);
	}
	//フィールドの解放
	g_feild.Release();
}