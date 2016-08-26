#pragma once

class SkinModelData;
class RenderContext;
class Light;
/*!
*@brief	�X�L�����f��
*/
class SkinModel {
public:
	SkinModel(){ position = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	~SkinModel();
	/*!
	*@brief	������
	*/
	void Init(SkinModelData* modelData);
	/*!
	*@brief	�`��B
	*/
	void Draw(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix);

	/*!
	*@brief	���[���h�s����X�V�B
	*@details
	* ���CMotion�N���X�Ɉړ������܂��B
	*@param[in]		trans	���s�ړ��B
	*@param[in]		rot		��]�B
	*@param[in]		scale	�g��B
	*/
	void UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale);
	/*!
	*@brief	���C�g��ݒ�B
	*/
	void SetLight(Light* light)
	{
		this->light = light;
	}
private:
	D3DXMATRIX			worldMatrix;	//!<���[���h�s��B
	D3DXMATRIX			rotationMatrix;	//!<��]�s��B
	SkinModelData*		skinModelData;	//!<�X�L�����f���f�[�^�B
	ID3DXEffect*		pEffect;		//!<�G�t�F�N�g�B
	Light*				light;			//!<���C�g�B
	D3DXVECTOR3			position;
};
