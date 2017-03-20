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

	bool GetMap(int, int);

	bool SetMap(int, int);

	bool SetMap(int, int, bool);

	void SetMapNull(int,int);

	int tansaku(int x, int y, int num);

	void SetGameOverDec(bool flg){
		gameoverflg = flg;
	}

	int* GetNum()
	{
		return &Number[0][0];
	}

	void SetPuyo(int x,int y,Puyo* puyo);

	void SetNullNum(int, int);

	void sakujo();

	void mapdown(int x,int y);

	bool A();


private:
	D3DXVECTOR3				position;		//���W�B
	D3DXVECTOR3				scale;
	D3DXMATRIX				mWorld;						//���[���h�s��B
	D3DXMATRIX				mRotation;					//��]�s��B
	Model                   model;
	LPD3DXMESH				mesh;						//���b�V���B
	LPDIRECT3DTEXTURE9*		textures;					//�e�N�X�`���B
	DWORD					numMaterial;				//�}�e���A���̐��B
	ID3DXEffect*			effect;						//�G�t�F�N�g�B
	bool					map[TATE][YOKO];			//�Ղ悪���邩�ǂ����m�F����z��B����Ƃ�:true / �Ȃ��Ƃ�:false
	bool					gameoverflg;
	int						Number[TATE][YOKO];			//�ǂ̎�ނ̂Ղ悪����̂��m�F����z��B
	bool					cmb[TATE][YOKO];			//�T���ς݂��ǂ������ׂ�z��B�ρFtrue / ���Ffalse
	bool					deadflg[TATE][YOKO];		//���S�t���O�B���S�Ftrue / �����Ffalse
	Puyo*					PuyoMap[TATE][YOKO];
};