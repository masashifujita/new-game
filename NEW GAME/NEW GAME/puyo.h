#pragma once
#include "model.h"
#include "feild.h"

class Puyo
{
public:
	//�R���X�g���N�^
	Puyo();
	//�f�X�g���N�^
	~Puyo();
	//�������B
	void Init(LPDIRECT3DDEVICE9,LPCSTR);
	//�X�V�B
	void Update();
	//�`��B
	void Render(
		LPDIRECT3DDEVICE9 pd3dDevice,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix,
		D3DXVECTOR4* diffuseLightDirection,
		D3DXVECTOR4* diffuseLightColor,
		D3DXVECTOR4	 ambientLight,
		int numDiffuseLight,
		bool isDrawShadowMap,
		bool isRecieveShadow
		);
	//�J���B
	void Release();

	void UpdateWorldMatrix(const D3DXVECTOR3& trans,const D3DXQUATERNION& rot,const D3DXVECTOR3& scale);

	void DownMove();

	int GetiPos_X()
	{
		return ipos_X;
	}

	int GetiPos_Y()
	{
		return ipos_Y;
	}

	int GetiPos_Z()
	{
		return ipos_Z;
	}

	void SetiPos_X(int x)
	{
		ipos_X = x;
	}

	void SetiPos_Y(int y){
		ipos_Y = y;
	}

	void SetiPos_Z(int z)
	{
		ipos_Z = z;
	}

	const D3DXVECTOR3& GetPos()
	{
		return position;
	}

	//downmoveflg�̃Q�b�^�[
	bool GetMoveFlg()
	{
		return downmoveflg;
	}

	//���Ԃ��Ǘ�����֐�
	bool Time();

	// �E��1�}�X�ړ��B
	void Add_X(short);

	// ����1�}�X�ړ��B
	void Sub_X(short);

	//����1�}�X�ړ��B
	void Add_Z();

	//�O��1�}�X�ړ��B
	void Sub_Z();

	void Search();

private:
	D3DXVECTOR3				position;		//���W�B
	D3DXVECTOR3				movespeed;
	float					targetAngleY;
	D3DXQUATERNION			rot;
	Model                   model;
	LPD3DXMESH				mesh;			//���b�V���B
	LPDIRECT3DTEXTURE9*		textures;		//�e�N�X�`���B
	DWORD					numMaterial;	//�}�e���A���̐��B
	ID3DXEffect*			effect;			//�G�t�F�N�g�B
	const float				cTurnSpeed = D3DXToRadian(20.0f);
	float					turnSpeed = 0.0f;
	D3DXMATRIX				worldMatrix;										
	D3DXMATRIX				world;
	D3DXMATRIX				rotation;
	bool					downmoveflg;	//�Ղ悪�������Ă��邩�B
	int						ipos_X;
	int						ipos_Y;
	int						ipos_Z;
	D3DXVECTOR2				lefttop;
	float					time;
	Feild*					feild;
	bool					checkdownflg;
	int						namber;
};