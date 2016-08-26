#include "stdafx.h"
#include "camera.h"
#include "player.h"
#include "feild.h"
#include "light.h"
#include "system.h"

Camera camera;				//カメラ。
Unity unity;		//プレーヤー
Feild feild;
Light light;


void Init()
{
	//ライトを初期化。
	light.Init();
	//プレイヤーの初期化
	unity.Init(g_pd3dDevice);
	//カメラの初期化。
	camera.Init(&unity);
	//フィールドの初期化
	feild.Init(g_pd3dDevice);
}

void Render()
{
	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//シーンの描画開始。
	g_pd3dDevice->BeginScene();
	//プレーヤーを描画。
	unity.Render(
		g_pd3dDevice,
		camera.GetViewMatrix(),
		camera.GetProjectionMatrix(),
		light.GetLightDirection(),
		light.GetLightColor(),
		light.GetambientLight(),
		LIGHT_NUM
		);

	//フィールドを描画
	feild.Render(g_pd3dDevice,
		camera.GetViewMatrix(),
		camera.GetProjectionMatrix(),
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
	//ライトの更新。
	light.Update();

	//プレーヤーの更新。
	unity.Update();
	//カメラの更新
	camera.Update();
	//フィールドの更新
	feild.Update();
}
void Terminate()
{
	//プレーヤーの解放
	unity.Release();
	//フィールドの解放
	feild.Release();
}

