#include "stdafx.h"
#include "camera.h"
#include "puyopuyo.h"
#include "feild.h"
#include "light.h"
#include "system.h"
#include <list>


Camera g_camera;				//�J�����B
std::list<PuyoPuyo*>	puyopuyoList;	//�Ղ�Ղ�̃��X�g�B
PuyoPuyo* nowPuyoPuyo = NULL;
PuyoPuyo* nextPuyoPuyo = NULL;
Feild g_feild;
Light light;
int puyopuyoTimer = 120;

void Init()
{
	//���C�g���������B
	light.Init();
	
	//�J�����̏������B
	g_camera.Init();
	//�t�B�[���h�̏�����
	g_feild.Init(g_pd3dDevice);
}

void Render()
{
	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();
	//�v���[���[��`��B
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
	 

	//�t�B�[���h��`��
	g_feild.Render(g_pd3dDevice,
		g_camera.GetViewMatrix(),
		g_camera.GetProjectionMatrix(),
		light.GetLightDirection(),
		light.GetLightColor(),
		light.GetambientLight(),
		LIGHT_NUM
		);

	// �V�[���̕`��I���B
	g_pd3dDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
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
	
	//���C�g�̍X�V�B
	light.Update();
	
	//�v���[���[�̍X�V�B
	nowPuyoPuyo->Update();
	//�t�B�[���h�̍X�V
	g_feild.Update(nowPuyoPuyo);
	if (nowPuyoPuyo->GetDownFlg() == true){
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