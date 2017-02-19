#pragma once
#include "model.h"
#include "puyopuyo.h"


enum MapState{None = 0,PUYO};

class Feild {
public:
	//�R���X�g���N�^
	Feild();
	//�f�X�g���N�^
	~Feild();
	//�������B
	void Init(LPDIRECT3DDEVICE9 pd3dDevice);
	//�X�V�B
	void Update(PuyoPuyo*);
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
	//���W��ݒ�B
	void SetPosition(D3DXVECTOR3 pos);

	int GetMap(int, int);

	bool SetMap(int, int);

	void SetMapNull(int,int);

	void Con();

	void SetGameOverDec(bool flg){
		gameoverflg = flg;
	}

	bool Sakujo();

	void SetNumMap(int, int,int);

	int Conbine(int,int,int);

	int* GetNum()
	{
		return &Number[0][0];
	}

private:
	D3DXVECTOR3				position;		//���W�B
	D3DXVECTOR3				scale;
	D3DXMATRIX				mWorld;			//���[���h�s��B
	D3DXMATRIX				mRotation;		//��]�s��B
	Model                   model;
	LPD3DXMESH				mesh;			//���b�V���B
	LPDIRECT3DTEXTURE9*		textures;		//�e�N�X�`���B
	DWORD					numMaterial;	//�}�e���A���̐��B
	ID3DXEffect*			effect;			//�G�t�F�N�g�B
	int						map[TATE][YOKO];
	bool					gameoverflg;
	int						Number[TATE][YOKO];
	bool					cmb[TATE][YOKO];
};