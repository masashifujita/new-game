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
#include "primitive.h"
#include "rendertarget.h"
#include "bloom.h"
#include "unity.h"


Camera					g_camera;				//カメラ。
std::list<PuyoPuyo*>	puyopuyoList;			//ぷよぷよのリスト。
PuyoPuyo*				nowPuyoPuyo = NULL;
Feild					g_feild;
Light					light;
int						puyopuyoTimer = 120;
KeyBoard				g_keyboard;
ShadowMap				g_shadowmap;
Bloom					bloom;
Puyo					puyo;
CParticleEmitter		g_particleEmitter;		//パーティクルエミッター
CRenderTarget*			mainRenderTarget;		//メインレンダリングターゲット。
CPrimitive*				quadPrimitive;			//四角形の板ポリプリミティブ。
LPD3DXEFFECT			copyEffect;				//コピーを行うシェーダー。
LPD3DXEFFECT			monochromeEffect;		//モノクロフィルターをかけるシェーダー。
Unity					unity;

//板ポリを描画
void DrawQuadPrimitive()
{
	// 頂点ストリーム0番に板ポリの頂点バッファを設定する。
	g_pd3dDevice->SetStreamSource(
		0,												//頂点ストリームの番号。
		quadPrimitive->GetVertexBuffer()->GetBody(),	//頂点バッファ。
		0,												//頂点バッファの読み込みを開始するオフセットのバイト数。
		//今回は先頭から読み込むので0でいい。
		quadPrimitive->GetVertexBuffer()->GetStride()	//頂点一つ分のサイズ。単位はバイト。
		);
	// インデックスバッファを設定。
	g_pd3dDevice->SetIndices(quadPrimitive->GetIndexBuffer()->GetBody());
	// 頂点定義を設定する。
	g_pd3dDevice->SetVertexDeclaration(quadPrimitive->GetVertexDecl());
	//　準備が整ったので描画。
	g_pd3dDevice->DrawIndexedPrimitive(
		quadPrimitive->GetD3DPrimitiveType(),	//プリミティブの種類を指定する。
		0,										//ベース頂点インデックス。0でいい。
		0,										//最小の頂点インデックス。0でいい。
		quadPrimitive->GetNumVertex(),			//頂点の数。
		0,										//開始インデックス。0でいい。
		quadPrimitive->GetNumPolygon()			//プリミティブの数。
		);
}
//メインレンダリングターゲットを初期化
void InitMainRenderTarget()
{
	mainRenderTarget = new CRenderTarget;
	mainRenderTarget->Create(
		FRAME_BUFFER_WITDH,			//レンダリングターゲットの幅と高さはフレームバッファと同じにしておく。(必ずしも同じである必要はない！！！)
		FRAME_BUFFER_HEIGHT,
		1,							//ミップマップレベル。これは1でいい。ミップマップ覚えてますか？
		D3DFMT_A16B16G16R16F,		//レンダリングターゲットのフォーマット。16bitの浮動小数点バッファを指定する。
		D3DFMT_D24S8,				//デプスステンシルバッファのフォーマット。一般的に16bitと24bitフォーマットが使われることが多い。今回は24bitフォーマットを使う。
		D3DMULTISAMPLE_NONE,		//マルチサンプリングの種類。今回はマルチサンプリングは行わないのでD3DMULTISAMPLE_NONEでいい。
		0							//マルチサンプリングの品質レベル。今回はマルチサンプリングは行わないので0でいい。
		);
}
//板ポリの初期化
void InitQuadPrimitive()
{
	quadPrimitive = new CPrimitive;
	//頂点の構造体。
	struct SVertex {
		float pos[4];	//頂点座標。
		float uv[2];		//UV座標。
	};
	//頂点バッファ。
	SVertex vertexBuffer[] = {
		{
			-1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f
		},
		{
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f
		},
		{
			-1.0f, -1.0f, 0.0f, 1.0f,
			0.0f, 1.0f
		},
		{
			1.0f, -1.0f, 0.0f, 1.0f,
			1.0f, 1.0f
		},
	};
	//頂点レイアウト。頂点バッファの構造がどのようになっているかをGPUに教えるためのもの。
	static const D3DVERTEXELEMENT9 scShapeVertex_PT_Element[] = {
		//頂点ストリームの0番目の0バイト目からfloat型の4つ分のデータは座標(D3DDECLUSAGE_POSITION)であることを示している。
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		//頂点ストリームの0番目の16バイト目からfloat型の2つ分のデータはUV座標(D3DDECLUSAGE_TEXCOORD)であることを示している。
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	//インデックスバッファ。
	unsigned short indexBuffer[] = { 0, 1, 2, 3 };
	quadPrimitive->Create(
		CPrimitive::eTriangleStrip,	//今回はプリミティブの種類はトライアングルストリップ。
		4,							//頂点の数。四角形の板ポリでトライアングルストリップなので４。
		sizeof(SVertex),			//頂点ストライド。一つの頂点の大きさ。単位はバイト。
		scShapeVertex_PT_Element,	//頂点レイアウト。
		vertexBuffer,				//頂点バッファ。
		4,							//インデックスの数。
		D3DFMT_INDEX16,				//インデックスバッファのフォーマット。今回は16bitフォーマットを指定する。
		//インデックスの数が65536以下の場合は16bitでいいが、それを超える場合はD3DFMT_INDEX32を指定する。
		indexBuffer					//インデックスバッファ。
		);
}
//メインレンダリングターゲットの内容を現在のレンダリングターゲットにコピー
void CopyMainRTToCurrentRT()
{

	// Zテストで失敗してもらったら困るので、Zテストは無効にしておく。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// αブレンドもいらない。
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	LPD3DXEFFECT shader = copyEffect;			//18-3 コピーを行うシェーダーを使う場合はこちら。
	//LPD3DXEFFECT shader = monochromeEffect;		//18-4 モノクロフィルターをかける場合はこちらを使用する。
	//シェーダーを設定。
	shader->SetTechnique("Default");
	shader->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	shader->BeginPass(0);
	//シーンテクスチャを設定する
	shader->SetTexture("g_tex", mainRenderTarget->GetTexture());
	//定数レジスタへの変更をコミットする。
	shader->CommitChanges();
	DrawQuadPrimitive();
	shader->EndPass();
	shader->End();
	// 変更したレンダリングステートを元に戻す。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}
//シェーダーをロード
void LoadShaders()
{
	//18-3 コピーを行うシェーダーをロード。
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(
		g_pd3dDevice,
		"Copy.fx",
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&copyEffect,
		&compileErrorBuffer
		);
	if (FAILED(hr)) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
	//18-4 モノクロフィルターを行うシェーダーをロード。
	hr = D3DXCreateEffectFromFile(
		g_pd3dDevice,
		"MonochromeFilter.fx",
		NULL,
		NULL,
		D3DXSHADER_DEBUG,
		NULL,
		&monochromeEffect,
		&compileErrorBuffer
		);
	if (FAILED(hr)) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
}



void Init()
{
	g_shadowmap.Init();

	//メインレンダリングターゲットを作成する。
	InitMainRenderTarget();
	//四角形の板ポリプリミティブを作成。
	InitQuadPrimitive();
	//シェーダーをロード。
	LoadShaders();
	

	//ライトを初期化。
	light.Init();

	//パーティクル初期化
	SParicleEmitParameter param;
	param.texturePath = "star.png";
	param.w = 0.5f;
	param.h = 0.5f;
	param.intervalTime = 0.2f;
	param.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	param.life = 5.0f;
	g_particleEmitter.Init(param);
	
	//カメラの初期化。
	g_camera.Init();
	//フィールドの初期化
	g_feild.Init(g_pd3dDevice);
	
	//bloomの初期化
	bloom.Init("bloom.fx", g_pd3dDevice);

	//unityちゃん初期化
	unity.Init(g_pd3dDevice);
}

void Render()
{
	//シーンの描画開始。
	g_pd3dDevice->BeginScene();

	//あとで戻すためにフレームバッファのレンダリングターゲットとデプスステンシルバッファのバックアップを取る。
	LPDIRECT3DSURFACE9 frameBufferRT;
	LPDIRECT3DSURFACE9 frameBufferDS;
	g_pd3dDevice->GetRenderTarget(0, &frameBufferRT);
	g_pd3dDevice->GetDepthStencilSurface(&frameBufferDS);

	//レンダリングターゲットをフレームバッファから変更する。
	g_pd3dDevice->SetRenderTarget(
		0,									
		mainRenderTarget->GetRenderTarget()	
		);
	// デプスステンシルバッファも変更する。
	g_pd3dDevice->SetDepthStencilSurface(mainRenderTarget->GetDepthStencilBuffer());

	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	g_shadowmap.Draw();

	//パーティクル描画。
	//g_particleEmitter.Render(g_camera.GetViewMatrix(), g_camera.GetProjectionMatrix());

	//unityちゃん描画
	unity.Render(
		g_pd3dDevice,
		g_camera.GetViewMatrix(),
		g_camera.GetProjectionMatrix(),
		light.GetLightDirection(),
		light.GetLightColor(),
		light.GetambientLight(),
		LIGHT_NUM,
		false,
		false
		);

	//puyopuyoを描画。
	for (auto& puyopuyo : puyopuyoList)
	{
		puyopuyo->Render(
			g_pd3dDevice,
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
	g_feild.Render(
		g_pd3dDevice,
		g_camera.GetViewMatrix(),
		g_camera.GetProjectionMatrix(),
		light.GetLightDirection(),
		light.GetLightColor(),
		light.GetambientLight(),
		LIGHT_NUM,
		false,
		true
		);

	//bloom描画
	bloom.Render();

	//シーンの描画が完了したのでレンダリングターゲットをフレームバッファに戻す。
	g_pd3dDevice->SetRenderTarget(0, frameBufferRT);
	g_pd3dDevice->SetDepthStencilSurface(frameBufferDS);

	//参照カウンタが増えているので開放。
	frameBufferRT->Release();
	frameBufferDS->Release();

	//オフスクリーンレンダリングした絵をフレームバッファに貼り付ける。
	CopyMainRTToCurrentRT();


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

	//unityちゃん更新
	unity.Update();

	//ぷよぷよの生成
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
	//ぷよぷよが一番下、ぷよの上に落ちて止まった時nowPuyoPuyoをUpdateしないためNULLにする。
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

	//unityちゃん解放
	unity.Release();

}