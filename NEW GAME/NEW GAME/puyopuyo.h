#pragma once

class Puyo;

#define NUM 2
#define NAMELIST_MAX 5	// Puyo�N���X�̃��f���ǂݍ��ݎ��Ɏg�p����t�@�C�����̃��X�g�̗v�f��

class PuyoPuyo
{
public:
	//�R���X�g���N�^
	PuyoPuyo();
	//�f�X�g���N�^
	~PuyoPuyo();
	//�������B
	void Init(LPDIRECT3DDEVICE9);
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
		int numDiffuseLight
		);
	//�J���B
	void Release();

	D3DXVECTOR3 GetPos()
	{
		return position;
	}

	bool GetDownFlg()
	{
		return downflg;
	}
	
	//puyo�̐擪�A�h���X���擾����֐�
	Puyo** GetPuyoArray()
	{
		return puyo;
	}

private:
	Puyo*					puyo[NUM];
	D3DXVECTOR3				position;		//���W�B
	D3DXVECTOR3				movespeed;
	float					targetAngleY;
	D3DXQUATERNION			rot;
	bool					downflg;
	
public:
	static const char nameList[NAMELIST_MAX][FILENAME_MAX];
};