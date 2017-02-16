#ifndef _TKPARTICLE_H_
#define _TKPARTICLE_H_

#include "primitive.h"
#include "stdafx.h"

struct SParicleEmitParameter;
/*!
* @brief	�p�[�e�B�N���B
*/
class CParticle{
	CPrimitive			primitive;			//!<�v���~�e�B�u�B
	LPDIRECT3DTEXTURE9	texture;			//!<�e�N�X�`���B
	ID3DXEffect*		shaderEffect;		//!<�V�F�[�_�[�G�t�F�N�g�B
	D3DXVECTOR3			moveSpeed;			//!<���x�B
	D3DXVECTOR3			position;			//!<���W�B
	float				life = 0.0f;		//!<����
public:
	CParticle();
	~CParticle();
	void Init(const SParicleEmitParameter& param);
	bool Update();
	void Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix);
};

#endif //_TKPARTICLE_H_