#include "stdafx.h"
#include "model.h"

extern Camera g_camera;

Model::Model()
{
	mesh = NULL;
	textures = NULL;
	numMaterial = 0;
	effect = NULL;
}

Model::~Model()
{

}

void Model::Init(LPDIRECT3DDEVICE9 pd3dDevice,const char* model)
{
	//Xファイルをロード。
	LPD3DXBUFFER pD3DXMtrlBuffer;

	//Xファイルのロード。
	D3DXLoadMeshFromX(
		model, 
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
	HRESULT hr = D3DXCreateEffectFromFile(
		pd3dDevice,
		"basic.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&effect,
		&compileErrorBuffer
		);
	if (hr != S_OK) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
	camera = &g_camera;
}

void Model::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight,
	D3DXMATRIX	World,
	D3DXMATRIX	Rotation
	)
{
	effect->SetTechnique("SkinModel");
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	effect->BeginPass(0);
	//定数レジスタに設定するカラー。
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
	//ワールド行列の転送。
	effect->SetMatrix("g_worldMatrix", &World);
	//ビュー行列の転送。
	effect->SetMatrix("g_viewMatrix", &viewMatrix);			//ビュー行列を転送。
	effect->SetMatrix("g_projectionMatrix", &projMatrix);	//プロジェクション行列の転送。
	effect->SetMatrix("g_rotationMatrix", &Rotation);		//回転行列を転送。
	//ライトの向きを転送。
	effect->SetVectorArray("g_diffuseLightDirection", diffuseLightDirection, numDiffuseLight);
	//ライトのカラーを転送。
	effect->SetVectorArray("g_diffuseLightColor", diffuseLightColor, numDiffuseLight);
	
	effect->SetVector("g_eyePos", &(D3DXVECTOR4)camera->GetEyePt());

	//環境光を設定。
	effect->SetVector("g_ambientLight", &ambientLight);
	for (DWORD i = 0; i < numMaterial; i++)
	{
		effect->SetTexture("g_diffuseTexture", textures[i]);
		effect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
		
		mesh->DrawSubset(i);
	}
	effect->EndPass();
	effect->End();

}

void Model::Release()
{
	//メッシュを開放。
	if (mesh != NULL) {
		mesh->Release();
		mesh = NULL;
	}
	//テクスチャを開放。
	if (textures != NULL) {
		for (unsigned int i = 0; i < numMaterial; i++) {
			if (textures[i] != NULL) {
				textures[i]->Release();
				textures[i] = NULL;
			}
		}
		delete[] textures;
		textures = NULL;
	}
	//エフェクトを開放。
	if (effect != NULL) {
		effect->Release();
		effect = NULL;
	}

}