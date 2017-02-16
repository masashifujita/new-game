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


Camera g_camera;				//�J�����B
std::list<PuyoPuyo*>	puyopuyoList;	//�Ղ�Ղ�̃��X�g�B
PuyoPuyo* nowPuyoPuyo = NULL;
Feild g_feild;
Light light;
int puyopuyoTimer = 120;
KeyBoard g_keyboard;
ShadowMap g_shadowmap;
Puyo puyo;
CParticleEmitter		g_particleEmitter;	//�p�[�e�B�N���G�~�b�^�[


void Init()
{
	g_shadowmap.Init();

	//���C�g���������B
	light.Init();
	
	//�J�����̏������B
	g_camera.Init();

	SParicleEmitParameter param;
	param.texturePath = "star.png";
	param.w = 0.5f;
	param.h = 0.5f;
	param.intervalTime = 0.2f;
	param.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	param.life = 5.0f;
	g_particleEmitter.Init(param);

	//�t�B�[���h�̏�����
	g_feild.Init(g_pd3dDevice);
}

void Render()
{

	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();


	g_shadowmap.Draw();

	g_particleEmitter.Render(g_camera.GetViewMatrix(), g_camera.GetProjectionMatrix());


	//�v���[���[��`��B
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
	 
	//�t�B�[���h��`��
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
}