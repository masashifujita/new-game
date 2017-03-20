#include "stdafx.h"
#include "model.h"

extern Camera g_camera;

Model::Model()
{
	mesh = NULL;
	textures = NULL;
	numMaterial = 0;
	effect = NULL;
	normalMap = NULL;
}

Model::~Model()
{

}

void Model::Init(LPDIRECT3DDEVICE9 pd3dDevice,const char* model)
{
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//�@���}�b�v�����[�h
	HRESULT hr = D3DXCreateTextureFromFileA(
		pd3dDevice,
		"normal.jpg",
		&normalMap
		);
	//D3DXCreateTextureFromFileA�̖߂�l���`�F�b�N�B
	if (FAILED(hr)) {
		//D3DXCreateTextureFromFileA�Ŏ��s�����B
		MessageBox(NULL, "�e�N�X�`���̃��[�h�Ɏ��s���܂����B�w�肵���p�X�����������m�F�����肢���܂��B", "�G���[", MB_OK);
	}

	//X�t�@�C�������[�h�B
	LPD3DXBUFFER pD3DXMtrlBuffer;

	//X�t�@�C���̃��[�h�B
	D3DXLoadMeshFromX(
		model, 
		D3DXMESH_SYSTEMMEM,
		pd3dDevice, 
		NULL,
		&pD3DXMtrlBuffer, 
		NULL, 
		&numMaterial,
		&mesh);

	// �}�e���A���o�b�t�@���擾�B
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//�e�N�X�`�������[�h�B
	textures = new LPDIRECT3DTEXTURE9[numMaterial];
	for (DWORD i = 0; i < numMaterial; i++)
	{
		textures[i] = NULL;
		//�e�N�X�`�����쐬����B
		HRESULT hr = D3DXCreateTextureFromFileA(pd3dDevice,
			d3dxMaterials[i].pTextureFilename,
			&textures[i]);
		if (hr != S_OK){
			std::abort();
		}
	}
	// �}�e���A���o�b�t�@������B
	pD3DXMtrlBuffer->Release();
	//�V�F�[�_�[���R���p�C���B
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//�V�F�[�_�[���R���p�C���B
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
	if (hl != S_OK) {
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
	D3DXMATRIX	Rotation,
	bool isDrawShadowMap,
	bool isRecieveShadow
	)
{
	//D3DXMatrixTranslation()
	if (!isDrawShadowMap)
	{
		effect->SetTechnique("SkinModel");
	}
	else
	{
		effect->SetTechnique("SkinModelRenderToShadowMap");
		//return;
	}
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	effect->BeginPass(0);
	//�萔���W�X�^�ɐݒ肷��J���[�B
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
	effect->SetInt("g_isShadowReciever", isRecieveShadow);
	//���[���h�s��̓]���B
	effect->SetMatrix("g_worldMatrix", &World);
	//�r���[�s��̓]���B
	effect->SetMatrix("g_viewMatrix", &viewMatrix);			//�r���[�s���]���B
	effect->SetMatrix("g_projectionMatrix", &projMatrix);	//�v���W�F�N�V�����s��̓]���B
	effect->SetMatrix("g_rotationMatrix", &Rotation);		//��]�s���]���B
	//���C�g�̌�����]���B
	effect->SetVectorArray("g_diffuseLightDirection", diffuseLightDirection, numDiffuseLight);
	//���C�g�̃J���[��]���B
	effect->SetVectorArray("g_diffuseLightColor", diffuseLightColor, numDiffuseLight);
	
	effect->SetVector("g_eyePos", &(D3DXVECTOR4)camera->GetEyePt());

	if (normalMap != NULL)
	{
		effect->SetTexture("g_normalTexture", normalMap);
		effect->SetInt("g_isHasNormalMap", 1);
	}
	else
	{
		effect->SetInt("g_isHasNormalMap", 0);
	}

	//������ݒ�B
	effect->SetVector("g_ambientLight", &ambientLight);
	if (isRecieveShadow) {
		effect->SetTexture("g_shadowMapTexture", g_shadowmap.GetTexture());
		effect->SetMatrix("g_lightViewMatrix", &g_shadowmap.GetLightViewMatrix());
		effect->SetMatrix("g_lightProjectionMatrix", &g_shadowmap.GetLightProjectionMatrix());
	}
	for (DWORD i = 0; i < numMaterial; i++)
	{
		effect->SetTexture("g_diffuseTexture", textures[i]);
		effect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
		
		mesh->DrawSubset(i);
	}
	effect->EndPass();
	effect->End();
}


void Model::Release()
{
	//���b�V�����J���B
	if (mesh != NULL) {
		mesh->Release();
		mesh = NULL;
	}
	//�e�N�X�`�����J���B
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
	//�G�t�F�N�g���J���B
	if (effect != NULL) {
		effect->Release();
		effect = NULL;
	}
}