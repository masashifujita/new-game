#include "stdafx.h"
#include  "unity.h"
#include "keyboard.h"

extern KeyBoard g_keyboard;

Unity::Unity()
{
	n_map = NULL;
	rot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	position = D3DXVECTOR3(-3.0f, 3.0f, -5.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

Unity::~Unity(){}

//void Unity::UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
//{
//	D3DXMATRIX mTrans, mScale;
//	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
//	D3DXMatrixTranslation(&mTrans, trans.x, trans.y, trans.z);
//	D3DXMatrixRotationQuaternion(&rotation, &rot);
//
//	worldMatrix = mScale * rotation * mTrans;
//
//}

void Unity::UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
	D3DXMATRIX Trans, Scale;
	D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&Trans, trans.x, trans.y, trans.z);
	D3DXMatrixRotationQuaternion(&rotation, &rot);
	D3DXMATRIX Rot;
	D3DXMatrixRotationY(&Rot, targetAngleY);
	rotation = rotation*Rot;
	world = Scale*rotation*Trans;
}

void Unity::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	//法線マップをロード。
	HRESULT hr = D3DXCreateTextureFromFileA(
		pd3dDevice,
		/*"utc_nomal.tga"*/"normal.jpg",
		&normalMap
		);
	//D3DXCreateTextureFromFileAの戻り値をチェック。
	if (FAILED(hr)) {
		//D3DXCreateTextureFromFileAで失敗した。
		MessageBox(NULL, "テクスチャのロードに失敗しました。指定したパスが正しいか確認をお願いします。", "エラー", MB_OK);
	}
	//モデルをロード。

	LPD3DXBUFFER pD3DXMtrlBuffer;

	//Xファイルのロード。
	D3DXLoadMeshFromX(
		"kyu_5.x",
		D3DXMESH_SYSTEMMEM,
		pd3dDevice,
		NULL,
		&pD3DXMtrlBuffer,
		NULL,
		&numMaterial,
		&mesh);

	// マテリアルバッファを取得。
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//テクスチャをロード。
	textures = new LPDIRECT3DTEXTURE9[numMaterial];
	for (DWORD i = 0; i < numMaterial; i++)
	{
		textures[i] = NULL;
		//テクスチャを作成する。
		HRESULT hr = D3DXCreateTextureFromFileA(pd3dDevice,
			d3dxMaterials[i].pTextureFilename,
			&textures[i]);
		if (hr != S_OK){
			std::abort();
		}
	}
	// マテリアルバッファを解放。
	pD3DXMtrlBuffer->Release();
	//シェーダーをコンパイル。
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//シェーダーをコンパイル。
	HRESULT hl = D3DXCreateEffectFromFile(
		pd3dDevice,
		"basic.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&effect,
		&compileErrorBuffer
		);
	if (hr != S_OK) 
	{
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
	camera = &g_camera;
}

void Unity::Update()
{
	if (GetAsyncKeyState('A')) 
	{
		//法線マップのオフ
		if (normalMap){
			n_map = normalMap;
			normalMap = NULL;
		}
	}
	else if (GetAsyncKeyState('S')) 
	{
		//法線マップのオン
		if (n_map){
			normalMap = n_map;
		}
	}
	UpdateWorldMatrix(position, rot, scale);
}

void Unity::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight,
	bool isDrawShadowMap,
	bool isRecieveShadow)
{
	//D3DXMatrixTranslation()
	if (!isDrawShadowMap)
	{
		effect->SetTechnique("SkinModel");
	}
	else
	{
		effect->SetTechnique("SkinModelRenderToShadowMap");
	}

	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	effect->BeginPass(0);
	//定数レジスタに設定するカラー。
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
	effect->SetInt("g_isShadowReciever", isRecieveShadow);
	//ワールド行列の転送。
	effect->SetMatrix("g_worldMatrix", &world);
	//ビュー行列の転送。
	effect->SetMatrix("g_viewMatrix", &viewMatrix);			//ビュー行列を転送。
	effect->SetMatrix("g_projectionMatrix", &projMatrix);	//プロジェクション行列の転送。
	effect->SetMatrix("g_rotationMatrix", &rotation);		//回転行列を転送。

	if (normalMap != NULL)
	{
		effect->SetTexture("g_normalTexture", normalMap);
		effect->SetInt("g_isHasNormalMap", 1);
	}
	else
	{
		effect->SetInt("g_isHasNormalMap", 0);
	}

	//ライトの向きを転送。
	effect->SetVectorArray("g_diffuseLightDirection", diffuseLightDirection, numDiffuseLight);
	//ライトのカラーを転送。
	effect->SetVectorArray("g_diffuseLightColor", diffuseLightColor, numDiffuseLight);
	//環境光を設定。
	effect->SetVector("g_ambientLight", &ambientLight);

	effect->SetVector("g_eyePos", &(D3DXVECTOR4)camera->GetEyePt());

	if (isRecieveShadow) {
		effect->SetTexture("g_shadowMapTexture", g_shadowmap.GetTexture());
		effect->SetMatrix("g_lightViewMatrix", &g_shadowmap.GetLightViewMatrix());
		effect->SetMatrix("g_lightProjectionMatrix", &g_shadowmap.GetLightProjectionMatrix());
	}

	for (DWORD i = 0; i < numMaterial; i++)
	{
		effect->SetTexture("g_diffuseTexture", textures[i]);
		effect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。

		mesh->DrawSubset(i);
	}
	effect->EndPass();
	effect->End();
}

void Unity::Release()
{

}