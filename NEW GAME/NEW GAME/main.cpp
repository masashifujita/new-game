#include "stdafx.h"
#include "camera.h"
#include "player.h"
#include "feild.h"
#include "light.h"
#include "system.h"

Camera camera;				//�J�����B
Unity unity;		//�v���[���[
Feild feild;
Light light;


void Init()
{
	//���C�g���������B
	light.Init();
	//�v���C���[�̏�����
	unity.Init(g_pd3dDevice);
	//�J�����̏������B
	camera.Init(&unity);
	//�t�B�[���h�̏�����
	feild.Init(g_pd3dDevice);
}

void Render()
{
	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();
	//�v���[���[��`��B
	unity.Render(
		g_pd3dDevice,
		camera.GetViewMatrix(),
		camera.GetProjectionMatrix(),
		light.GetLightDirection(),
		light.GetLightColor(),
		light.GetambientLight(),
		LIGHT_NUM
		);

	//�t�B�[���h��`��
	feild.Render(g_pd3dDevice,
		camera.GetViewMatrix(),
		camera.GetProjectionMatrix(),
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
	//���C�g�̍X�V�B
	light.Update();

	//�v���[���[�̍X�V�B
	unity.Update();
	//�J�����̍X�V
	camera.Update();
	//�t�B�[���h�̍X�V
	feild.Update();
}
void Terminate()
{
	//�v���[���[�̉��
	unity.Release();
	//�t�B�[���h�̉��
	feild.Release();
}

