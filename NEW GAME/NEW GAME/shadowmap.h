#pragma once
#include "stdafx.h"
#include "rendertarget.h"
//�V���h�E�}�b�v�N���X�B
class ShadowMap {
	CRenderTarget renderTarget;		//�V���h�E�}�b�v���������ރ����_�����O�^�[�Q�b�g�B
	D3DXMATRIX  lightViewMatrix;	//���C�g�r���[�}�g���N�X�B
	D3DXMATRIX	lightProjMatrix;	//���C�g�v���W�F�N�V�����}�g���N�X�B
	D3DXVECTOR3 viewPosition;		//���C�g�r���[�̎��_�B
	D3DXVECTOR3 viewTarget;			//���C�g�r���[�̒����_�B
public:
	ShadowMap();
	~ShadowMap();
	//�e�N�X�`�����擾�B
	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return renderTarget.GetTexture();
	}

	//�������B
	void Init();
	//���C�g�r���[�̎��_��ݒ�B
	void SetLightViewPosition(const D3DXVECTOR3& lightViewPosition)
	{
		viewPosition = lightViewPosition;
	}
	//���C�g�r���[�̒����_��ݒ�B
	void SetLightViewTarget(const D3DXVECTOR3& lightViewTarget)
	{
		viewTarget = lightViewTarget;
	}
	//���C�g�r���[�s����擾�B
	const D3DXMATRIX& GetLightViewMatrix()
	{
		return lightViewMatrix;
	}
	//���C�g�v���W�F�N�V�����s����擾�B
	const D3DXMATRIX& GetLightProjectionMatrix()
	{
		return lightProjMatrix;
	}

	//�X�V�B
	void Update();
	//�V���h�E�}�b�v�ɏ������݁B
	void Draw();
};

