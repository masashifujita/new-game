#include "stdafx.h"
#include "camera.h"
#include "puyopuyo.h"
#include "feild.h"
#include "light.h"
#include "system.h"
#include <list>
#include "keyboard.h"
#include "shadowmap.h"
#include "puyo.h"
#include "particleemitter.h"


Camera g_camera;				//カメラ。
std::list<PuyoPuyo*>	puyopuyoList;	//ぷよぷよのリスト。
PuyoPuyo* nowPuyoPuyo = NULL;
Feild g_feild;
Light light;
int puyopuyoTimer = 120;
KeyBoard g_keyboard;
ShadowMap g_shadowmap;
Puyo puyo;
CParticleEmitter		g_particleEmitter;	//パーティクルエミッター


void Init()
{
	g_shadowmap.Init();

	//ライトを初期化。
	light.Init();
	
	//カメラの初期化。
	g_camera.Init();

	SParicleEmitParameter param;
	param.texturePath = "star.png";
	param.w = 0.5f;
	param.h = 0.5f;
	param.intervalTime = 0.2f;
	param.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	param.life = 5.0f;
	g_particleEmitter.Init(param);

	//フィールドの初期化
	g_feild.Init(g_pd3dDevice);
}

void Render()
{

	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//シーンの描画開始。
	g_pd3dDevice->BeginScene();


	g_shadowmap.Draw();

	g_particleEmitter.Render(g_camera.GetViewMatrix(), g_camera.GetProjectionMatrix());


	//プレーヤーを描画。
	for (auto& puyopuyo : puyopuyoList)
	{
		puyopuyo->Render(g_pd3dDevice,
			g_camera.GetViewMatrix(),
			g_camera.GetProjectionMatrix(),
			light.GetLightDirection(),
			light.GetLightColor(),
			light.GetambientLight(),
			LIGHT_NUM,
			false,
			false
			);
	}
	 
	//フィールドを描画
	g_feild.Render(g_pd3dDevice,
		g_camera.GetViewMatrix(),
		g_camera.GetProjectionMatrix(),
		light.GetLightDirection(),
		light.GetLightColor(),
		light.GetambientLight(),
		LIGHT_NUM,
		false,
		true
		);

	// シーンの描画終了。
	g_pd3dDevice->EndScene();
	// バックバッファとフロントバッファを入れ替える。
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Update()
{
	// キー状態の更新。
	g_keyboard.Update();

	g_shadowmap.Update();

	g_particleEmitter.Update();


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
	if (nowPuyoPuyo->GetIsDownEnd() == false){
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