#pragma once
#include "stdafx.h"
#include "system.h"

/*!
*@brief	�����_�����O�^�[�Q�b�g�B
*/
class CRenderTarget {
public:
	/*!
	*@brief	�R���X�g���N�^�B
	*/
	CRenderTarget()
	{
		m_depthSurface = NULL;
		m_texture = NULL;
		m_surface = NULL;
		m_texture = NULL;
	}
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~CRenderTarget()
	{
		Release();
	}
	/*!
	*@brief	����B
	*/
	void Release()
	{
		if (m_texture != nullptr) {
			m_texture->Release();
			m_texture = NULL;
		}
		if (m_depthSurface != nullptr) {
			m_depthSurface->Release();
			m_depthSurface = NULL;
		}
		if (m_surface != NULL) {
			m_surface->Release();
			m_surface = NULL;
		}
	}
	/*!
	*@brief	�����_�����O�^�[�Q�b�g�̍쐬�B
	*@param[in]	w					�����_�����O�^�[�Q�b�g�̕��B
	*@param[in]	h					�����_�����O�^�[�Q�b�g�̍����B
	*@param[in]	mipLevel			�~�b�v�}�b�v���x���B
	*@param[in]	colorFormat			�J���[�o�b�t�@�̃t�H�[�}�b�g�B
	*@param[in]	depthStencilFormat	�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B
	*@param[in]	multiSampleType		�}���`�T���v�����O�̎�ށB
	*@param[in]	multiSampleQuality	�}���`�T���v�����O�̕i���B
	*/
	void Create(
		int w,
		int h,
		int mipLevel,
		D3DFORMAT colorFormat,
		D3DFORMAT depthStencilFormat,
		D3DMULTISAMPLE_TYPE multiSampleType,
		int multiSampleQuality)
	{
		//�f�v�X�X�e���V���o�b�t�@�̍쐬�B
		HRESULT hr = g_pd3dDevice->CreateDepthStencilSurface(
			w,
			h,
			static_cast<D3DFORMAT>(depthStencilFormat),
			static_cast<D3DMULTISAMPLE_TYPE>(multiSampleType),
			multiSampleQuality,
			TRUE,
			&m_depthSurface,
			NULL
			);
		//�J���[�o�b�t�@���쐬�B
		hr = g_pd3dDevice->CreateTexture(
			w,
			h,
			mipLevel,
			D3DUSAGE_RENDERTARGET,
			static_cast<D3DFORMAT>(colorFormat),
			D3DPOOL_DEFAULT,
			&m_texture,
			NULL
			);
		m_texture->GetSurfaceLevel(0, &m_surface);
		m_width = w;
		m_height = h;
	}
	/*!
	*@brief	�����_�����O�^�[�Q�b�g���擾�B
	*/
	LPDIRECT3DSURFACE9 GetRenderTarget()
	{
		return m_surface;
	}
	/*!
	*@brief	�f�v�X�X�e���V���o�b�t�@���擾�B
	*/
	LPDIRECT3DSURFACE9 GetDepthStencilBuffer()
	{
		return m_depthSurface;
	}
	/*!
	*@brief	�e�N�X�`�����擾�B
	*/
	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return m_texture;
	}
	int GetWidth() const
	{
		return m_width;
	}
	int GetHeight() const
	{
		return m_height;
	}
private:
	LPDIRECT3DSURFACE9		m_depthSurface;		//!<�[�x�o�b�t�@�p�̃T�[�t�F�C�X
	LPDIRECT3DTEXTURE9		m_texture;			//!<�������ݐ�̃e�N�X�`���B
	LPDIRECT3DSURFACE9		m_surface;			//!<�T�[�t�F�C�X
	int						m_width = 0;		//!<��
	int						m_height = 0;		//!<����
};