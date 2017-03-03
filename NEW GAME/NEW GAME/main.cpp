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


Camera					g_camera;				//�J�����B
std::list<PuyoPuyo*>	puyopuyoList;			//�Ղ�Ղ�̃��X�g�B
PuyoPuyo*				nowPuyoPuyo = NULL;
Feild					g_feild;
Light					light;
int						puyopuyoTimer = 120;
KeyBoard				g_keyboard;
ShadowMap				g_shadowmap;
Bloom					bloom;
Puyo					puyo;
CParticleEmitter		g_particleEmitter;		//�p�[�e�B�N���G�~�b�^�[
CRenderTarget*			mainRenderTarget;		//���C�������_�����O�^�[�Q�b�g�B
CPrimitive*				quadPrimitive;			//�l�p�`�̔|���v���~�e�B�u�B
LPD3DXEFFECT			copyEffect;				//�R�s�[���s���V�F�[�_�[�B
LPD3DXEFFECT			monochromeEffect;		//���m�N���t�B���^�[��������V�F�[�_�[�B
Unity					unity;

//�|����`��
void DrawQuadPrimitive()
{
	// ���_�X�g���[��0�Ԃɔ|���̒��_�o�b�t�@��ݒ肷��B
	g_pd3dDevice->SetStreamSource(
		0,												//���_�X�g���[���̔ԍ��B
		quadPrimitive->GetVertexBuffer()->GetBody(),	//���_�o�b�t�@�B
		0,												//���_�o�b�t�@�̓ǂݍ��݂��J�n����I�t�Z�b�g�̃o�C�g���B
		//����͐擪����ǂݍ��ނ̂�0�ł����B
		quadPrimitive->GetVertexBuffer()->GetStride()	//���_����̃T�C�Y�B�P�ʂ̓o�C�g�B
		);
	// �C���f�b�N�X�o�b�t�@��ݒ�B
	g_pd3dDevice->SetIndices(quadPrimitive->GetIndexBuffer()->GetBody());
	// ���_��`��ݒ肷��B
	g_pd3dDevice->SetVertexDeclaration(quadPrimitive->GetVertexDecl());
	//�@�������������̂ŕ`��B
	g_pd3dDevice->DrawIndexedPrimitive(
		quadPrimitive->GetD3DPrimitiveType(),	//�v���~�e�B�u�̎�ނ��w�肷��B
		0,										//�x�[�X���_�C���f�b�N�X�B0�ł����B
		0,										//�ŏ��̒��_�C���f�b�N�X�B0�ł����B
		quadPrimitive->GetNumVertex(),			//���_�̐��B
		0,										//�J�n�C���f�b�N�X�B0�ł����B
		quadPrimitive->GetNumPolygon()			//�v���~�e�B�u�̐��B
		);
}
//���C�������_�����O�^�[�Q�b�g��������
void InitMainRenderTarget()
{
	mainRenderTarget = new CRenderTarget;
	mainRenderTarget->Create(
		FRAME_BUFFER_WITDH,			//�����_�����O�^�[�Q�b�g�̕��ƍ����̓t���[���o�b�t�@�Ɠ����ɂ��Ă����B(�K�����������ł���K�v�͂Ȃ��I�I�I)
		FRAME_BUFFER_HEIGHT,
		1,							//�~�b�v�}�b�v���x���B�����1�ł����B�~�b�v�}�b�v�o���Ă܂����H
		D3DFMT_A16B16G16R16F,		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B16bit�̕��������_�o�b�t�@���w�肷��B
		D3DFMT_D24S8,				//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B��ʓI��16bit��24bit�t�H�[�}�b�g���g���邱�Ƃ������B�����24bit�t�H�[�}�b�g���g���B
		D3DMULTISAMPLE_NONE,		//�}���`�T���v�����O�̎�ށB����̓}���`�T���v�����O�͍s��Ȃ��̂�D3DMULTISAMPLE_NONE�ł����B
		0							//�}���`�T���v�����O�̕i�����x���B����̓}���`�T���v�����O�͍s��Ȃ��̂�0�ł����B
		);
}
//�|���̏�����
void InitQuadPrimitive()
{
	quadPrimitive = new CPrimitive;
	//���_�̍\���́B
	struct SVertex {
		float pos[4];	//���_���W�B
		float uv[2];		//UV���W�B
	};
	//���_�o�b�t�@�B
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
	//���_���C�A�E�g�B���_�o�b�t�@�̍\�����ǂ̂悤�ɂȂ��Ă��邩��GPU�ɋ����邽�߂̂��́B
	static const D3DVERTEXELEMENT9 scShapeVertex_PT_Element[] = {
		//���_�X�g���[����0�Ԗڂ�0�o�C�g�ڂ���float�^��4���̃f�[�^�͍��W(D3DDECLUSAGE_POSITION)�ł��邱�Ƃ������Ă���B
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		//���_�X�g���[����0�Ԗڂ�16�o�C�g�ڂ���float�^��2���̃f�[�^��UV���W(D3DDECLUSAGE_TEXCOORD)�ł��邱�Ƃ������Ă���B
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	//�C���f�b�N�X�o�b�t�@�B
	unsigned short indexBuffer[] = { 0, 1, 2, 3 };
	quadPrimitive->Create(
		CPrimitive::eTriangleStrip,	//����̓v���~�e�B�u�̎�ނ̓g���C�A���O���X�g���b�v�B
		4,							//���_�̐��B�l�p�`�̔|���Ńg���C�A���O���X�g���b�v�Ȃ̂łS�B
		sizeof(SVertex),			//���_�X�g���C�h�B��̒��_�̑傫���B�P�ʂ̓o�C�g�B
		scShapeVertex_PT_Element,	//���_���C�A�E�g�B
		vertexBuffer,				//���_�o�b�t�@�B
		4,							//�C���f�b�N�X�̐��B
		D3DFMT_INDEX16,				//�C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g�B�����16bit�t�H�[�}�b�g���w�肷��B
		//�C���f�b�N�X�̐���65536�ȉ��̏ꍇ��16bit�ł������A����𒴂���ꍇ��D3DFMT_INDEX32���w�肷��B
		indexBuffer					//�C���f�b�N�X�o�b�t�@�B
		);
}
//���C�������_�����O�^�[�Q�b�g�̓��e�����݂̃����_�����O�^�[�Q�b�g�ɃR�s�[
void CopyMainRTToCurrentRT()
{

	// Z�e�X�g�Ŏ��s���Ă�������獢��̂ŁAZ�e�X�g�͖����ɂ��Ă����B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ���u�����h������Ȃ��B
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	LPD3DXEFFECT shader = copyEffect;			//18-3 �R�s�[���s���V�F�[�_�[���g���ꍇ�͂�����B
	//LPD3DXEFFECT shader = monochromeEffect;		//18-4 ���m�N���t�B���^�[��������ꍇ�͂�������g�p����B
	//�V�F�[�_�[��ݒ�B
	shader->SetTechnique("Default");
	shader->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	shader->BeginPass(0);
	//�V�[���e�N�X�`����ݒ肷��
	shader->SetTexture("g_tex", mainRenderTarget->GetTexture());
	//�萔���W�X�^�ւ̕ύX���R�~�b�g����B
	shader->CommitChanges();
	DrawQuadPrimitive();
	shader->EndPass();
	shader->End();
	// �ύX���������_�����O�X�e�[�g�����ɖ߂��B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}
//�V�F�[�_�[�����[�h
void LoadShaders()
{
	//18-3 �R�s�[���s���V�F�[�_�[�����[�h�B
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
	//18-4 ���m�N���t�B���^�[���s���V�F�[�_�[�����[�h�B
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

	//���C�������_�����O�^�[�Q�b�g���쐬����B
	InitMainRenderTarget();
	//�l�p�`�̔|���v���~�e�B�u���쐬�B
	InitQuadPrimitive();
	//�V�F�[�_�[�����[�h�B
	LoadShaders();
	

	//���C�g���������B
	light.Init();

	//�p�[�e�B�N��������
	SParicleEmitParameter param;
	param.texturePath = "star.png";
	param.w = 0.5f;
	param.h = 0.5f;
	param.intervalTime = 0.2f;
	param.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	param.life = 5.0f;
	g_particleEmitter.Init(param);
	
	//�J�����̏������B
	g_camera.Init();
	//�t�B�[���h�̏�����
	g_feild.Init(g_pd3dDevice);
	
	//bloom�̏�����
	bloom.Init("bloom.fx", g_pd3dDevice);

	//unity����񏉊���
	unity.Init(g_pd3dDevice);
}

void Render()
{
	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();

	//���ƂŖ߂����߂Ƀt���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���o�b�t�@�̃o�b�N�A�b�v�����B
	LPDIRECT3DSURFACE9 frameBufferRT;
	LPDIRECT3DSURFACE9 frameBufferDS;
	g_pd3dDevice->GetRenderTarget(0, &frameBufferRT);
	g_pd3dDevice->GetDepthStencilSurface(&frameBufferDS);

	//�����_�����O�^�[�Q�b�g���t���[���o�b�t�@����ύX����B
	g_pd3dDevice->SetRenderTarget(
		0,									
		mainRenderTarget->GetRenderTarget()	
		);
	// �f�v�X�X�e���V���o�b�t�@���ύX����B
	g_pd3dDevice->SetDepthStencilSurface(mainRenderTarget->GetDepthStencilBuffer());

	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	g_shadowmap.Draw();

	//�p�[�e�B�N���`��B
	//g_particleEmitter.Render(g_camera.GetViewMatrix(), g_camera.GetProjectionMatrix());

	//unity�����`��
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

	//puyopuyo��`��B
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
	 
	//�t�B�[���h��`��
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

	//bloom�`��
	bloom.Render();

	//�V�[���̕`�悪���������̂Ń����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂��B
	g_pd3dDevice->SetRenderTarget(0, frameBufferRT);
	g_pd3dDevice->SetDepthStencilSurface(frameBufferDS);

	//�Q�ƃJ�E���^�������Ă���̂ŊJ���B
	frameBufferRT->Release();
	frameBufferDS->Release();

	//�I�t�X�N���[�������_�����O�����G���t���[���o�b�t�@�ɓ\��t����B
	CopyMainRTToCurrentRT();


	// �V�[���̕`��I���B
	g_pd3dDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Update()
{
	// �L�[��Ԃ̍X�V�B
	g_keyboard.Update();

	g_shadowmap.Update();

	g_particleEmitter.Update();

	//unity�����X�V
	unity.Update();

	//�Ղ�Ղ�̐���
	if (nowPuyoPuyo == NULL)
	{
		PuyoPuyo* puyopuyo = new PuyoPuyo;
		nowPuyoPuyo = puyopuyo;
		puyopuyo->Init(g_pd3dDevice);
		puyopuyoList.push_back(puyopuyo);
	}
		
	//���C�g�̍X�V�B
	light.Update();
	
	//�v���[���[�̍X�V�B
	nowPuyoPuyo->Update();
	//�t�B�[���h�̍X�V
	g_feild.Update(nowPuyoPuyo);
	//�Ղ�Ղ悪��ԉ��A�Ղ�̏�ɗ����Ď~�܂�����nowPuyoPuyo��Update���Ȃ�����NULL�ɂ���B
	if (nowPuyoPuyo->GetIsDownEnd() == false){
		nowPuyoPuyo = NULL;
	}
	//�J�����̍X�V
	g_camera.Update();
}

void Terminate()
{
	//�v���[���[�̉��
	for (auto& puyopuyo : puyopuyoList){
		puyopuyo->Release();
		delete(puyopuyo);
	}
	//�t�B�[���h�̉��
	g_feild.Release();

	//unity�������
	unity.Release();

}