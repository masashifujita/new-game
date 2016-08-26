#pragma once

class Unity;

/*!
*@brief	�J�����N���X�B
*/
class Camera {
public:
	Camera();
	~Camera();
	//�A�X�y�N�g���ݒ�B
	void SetAspect(float _aspect)
	{
		aspect = _aspect;
	}
	//�A�X�y�N�g����擾�B
	float GetAspect()
	{
		return aspect;
	}
	//�t�@�[��ݒ�B
	void SetFar(float _far)
	{
		Far = _far;
	}
	//�j�A�[�̎擾�B
	float GetNear()
	{
		return Near;
	}
	//�t�@�[�̎擾�B
	float GetFar()
	{
		return Far;
	}
	//�j�A�[��ݒ�B
	void SetNear(float _near)
	{
		Near = _near;
	}
	//���_�̃Z�b�^�[
	void SetEyePt(D3DXVECTOR3 pt)
	{
		vEyePt = pt;
	}
	//���_�̃Q�b�^�[
	D3DXVECTOR3 GetEyePt()
	{
		return vEyePt;
	}
	//�����_�̃Z�b�^�[
	void SetLookatPt(D3DXVECTOR3 pt)
	{
		vLookatPt = pt;
	}
	//�����_�̃Q�b�^�[�B
	D3DXVECTOR3 GetLookatPt()
	{
		return vLookatPt;
	}
	//������̃Z�b�^�[
	void SetUpVec(D3DXVECTOR3 up)
	{
		vUpVec = up;
	}
	//������̃Q�b�^�[
	D3DXVECTOR3 GetUpVec()
	{
		return vUpVec;
	}
	//�r���[�s��̃Z�b�^�[�B
	void SetViewMatrix(D3DXMATRIX mView)
	{
		viewMatrix = mView;
	}
	//�r���[�s��̃Q�b�^�[
	D3DXMATRIX GetViewMatrix()
	{
		return viewMatrix;
	}
	//�v���W�F�N�V�����s��̃Z�b�^�[
	void SetProjectionMatrix(D3DXMATRIX mProj)
	{
		projectionMatrix = mProj;
	}
	//�v���W�F�N�V�����s��̃Q�b�^�[
	D3DXMATRIX GetProjectionMatrix()
	{
		return projectionMatrix;
	}
	void Update();
	void Init(Unity* unity);
private:
	D3DXMATRIX				viewMatrix;			//!<�r���[�s��B�J�����s��Ƃ������B
	D3DXMATRIX				projectionMatrix;	//!<�v���W�F�N�V�����s��B�r���[��Ԃ���ˉe��Ԃɕϊ�����s��B
	D3DXVECTOR3				vEyePt;				//!<�J�����̎��_�B���̕ӂ�̓����o�ϐ��ɕύX����B
	D3DXVECTOR3				vLookatPt;			//!<�J�����̒����_�B�ǂ������Ă��邩�Ƃ������B���̕ӂ�̓����o�ϐ��ɕύX����B
	D3DXVECTOR3				vUpVec;				//!<�J�����̏�����B��{�I��Y�����ł��܂�Ȃ��B��{�I�ɂ́E�E�E�B���̕ӂ�̓����o�ϐ��ɕύX����B
	float					Far;				//!<�����ʁB
	float					Near;				//!<�ߕ��ʁB
	float					aspect;				//!<�A�X�y�N�g��
	Unity*					unity;
	D3DXMATRIX				mWorld;			//���[���h�s��B
};