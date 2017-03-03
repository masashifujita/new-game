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

	//�@���}�b�v�����[�h�B
	HRESULT hr = D3DXCreateTextureFromFileA(
		pd3dDevice,
		/*"utc_nomal.tga"*/"normal.jpg",
		&normalMap
		);
	//D3DXCreateTextureFromFileA�̖߂�l���`�F�b�N�B
	if (FAILED(hr)) {
		//D3DXCreateTextureFromFileA�Ŏ��s�����B
		MessageBox(NULL, "�e�N�X�`���̃��[�h�Ɏ��s���܂����B�w�肵���p�X�����������m�F�����肢���܂��B", "�G���[", MB_OK);
	}
	//���f�������[�h�B

	LPD3DXBUFFER pD3DXMtrlBuffer;

	//X�t�@�C���̃��[�h�B
	D3DXLoadMeshFromX(
		"kyu_5.x",
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
		//�@���}�b�v�̃I�t
		if (normalMap){
			n_map = normalMap;
			normalMap = NULL;
		}
	}
	else if (GetAsyncKeyState('S')) 
	{
		//�@���}�b�v�̃I��
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
	//�萔���W�X�^�ɐݒ肷��J���[�B
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
	effect->SetInt("g_isShadowReciever", isRecieveShadow);
	//���[���h�s��̓]���B
	effect->SetMatrix("g_worldMatrix", &world);
	//�r���[�s��̓]���B
	effect->SetMatrix("g_viewMatrix", &viewMatrix);			//�r���[�s���]���B
	effect->SetMatrix("g_projectionMatrix", &projMatrix);	//�v���W�F�N�V�����s��̓]���B
	effect->SetMatrix("g_rotationMatrix", &rotation);		//��]�s���]���B

	if (normalMap != NULL)
	{
		effect->SetTexture("g_normalTexture", normalMap);
		effect->SetInt("g_isHasNormalMap", 1);
	}
	else
	{
		effect->SetInt("g_isHasNormalMap", 0);
	}

	//���C�g�̌�����]���B
	effect->SetVectorArray("g_diffuseLightDirection", diffuseLightDirection, numDiffuseLight);
	//���C�g�̃J���[��]���B
	effect->SetVectorArray("g_diffuseLightColor", diffuseLightColor, numDiffuseLight);
	//������ݒ�B
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
		effect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B

		mesh->DrawSubset(i);
	}
	effect->EndPass();
	effect->End();
}

void Unity::Release()
{

}