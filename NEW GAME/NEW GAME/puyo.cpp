#include "stdafx.h"
#include "puyo.h"

extern Feild g_feild;

//�R���X�g���N�^
Puyo::Puyo()
{
	//�������B
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rotation);
	movespeed = D3DXVECTOR3(0.0f, 0.03f, 0.0f);
	lefttop = D3DXVECTOR2(0.0, TATE * GRIDSIZE);
	ipos_X = 0;
	ipos_Y = 0;
	ipos_Z = 0;
	rot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	checkdownflg = false;
	downmoveflg = true;
}

//�f�X�g���N�^
Puyo::~Puyo()
{
	Release();
}

void Puyo::UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
	D3DXMATRIX Trans, Scale;
	D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&Trans, trans.x, trans.y, trans.z);
	D3DXMatrixRotationQuaternion(&rotation, &rot);
	D3DXMATRIX Rot;
	D3DXMatrixRotationY(&Rot, targetAngleY);
	rotation = rotation*Rot;
	world = Scale*rotation*Trans;
}

//���Ԃ��Ǘ�����֐�
bool Puyo::Time()
{
	time += 2.0f / 60.0f;
	if (time >= 1.0f)
	{
		time = 0.0f;			//���Ԃ̃��Z�b�g
		return true;
	}
	return false;
}

//���Ɉړ�������֐�
void Puyo::DownMove()
{
	int movespeed_y = 1;
	if (Time() == true)
	{
		//���t���[�����������炱���̏������s���B
		ipos_Y += movespeed_y;

		// �����蔻��
		if (feild->GetMap(ipos_X, ipos_Y,ipos_Z) == MapState::PUYO)
		{
			//���݂�ipos_Y�̏ꏊ��puyo������ꍇ�̏���
			downmoveflg = false;

			//feild->SetMap(ipos_X,ipos_Y - 1):���̏�����Puyo������ipos_Y�̈ꂩ�����puyo���Z�b�g����B
			feild->SetGameOverDec(feild->SetMap(ipos_X, ipos_Y - 1,ipos_Z));

			//updata��position�����߂�v�Z������̂�ipos_Y���ꂩ����Ɉړ�������K�v������B
			ipos_Y--;
		}
		else if (ipos_Y == TATE - 1)
		{
			//���݂�ipos_Y�̏ꏊ����ԉ��̒i�������ꍇ�̏���
			downmoveflg = false;

			feild->SetGameOverDec(feild->SetMap(ipos_X, ipos_Y,ipos_Z));
		}
	}
}

//�E�Ɉړ�������֐�
void Puyo::Add_X(short num)
{
	if (ipos_X + num < YOKO / 2)													//�E���ɍs�������̔���
	{
		if (feild->GetMap(ipos_X + num + 1, ipos_Y,ipos_Z) != MapState::PUYO)		//�E��puyo���Ȃ����̔���
		{
			//�E����puyo���Ȃ���puyopuyo���E�Ɉړ�������B
			ipos_X++;
		}
	}
}

//���Ɉړ�������֐�
void Puyo::Sub_X(short num)													//num :�@���݂�puyo�̗v�f�ԍ��H
{
	if (ipos_X - num > (YOKO / -2) + 1)													//���[�ɍs�������̔���							
	{
		if (feild->GetMap(ipos_X - num - 1, ipos_Y,ipos_Z) != MapState::PUYO)		//����puyo���Ȃ����̔���
		{
			//������puyo���Ȃ���puyopuyo�����Ɉړ�������B
			ipos_X--;
		}	
	}
}

//����1�}�X�ړ�����֐�
void Puyo::Add_Z()
{
	if (ipos_Z < NANAME / 2)
	{
		if (feild->GetMap(ipos_X, ipos_Y, ipos_Z + 1) != MapState::PUYO)
		{
			ipos_Z++;
		}
	}
}

//�O��1�}�X�ړ�����֐�
void Puyo::Sub_Z()
{
	if (ipos_Z > (NANAME / -2) + 1){
		if (feild->GetMap(ipos_X, ipos_Y, ipos_Z - 1) != MapState::PUYO)		//����puyo���Ȃ����̔���
		{
			ipos_Z--;
		}
	}
}

void Puyo::Init(LPDIRECT3DDEVICE9 pd3dDevice,LPCSTR name)
{
	feild = &g_feild;

	model.Init(pd3dDevice,name);
}

void Puyo::Update()
{
	//DownMove()�֐��Ăяo��m	
	DownMove();


	//������ipos��position�ɕϊ�����B
	position.x = ipos_X * GRIDSIZE + GRIDSIZE * 0.5;
	position.y = lefttop.y - (ipos_Y * GRIDSIZE) - GRIDSIZE * 0.5;
	position.z = ipos_Z;

	//���[���h�s��̍X�V�B
	UpdateWorldMatrix(position, rot, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

//�`��B
void Puyo::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight
	)
{
	model.Render(pd3dDevice,
		viewMatrix,
		projMatrix,
		diffuseLightDirection,
		diffuseLightColor,
		ambientLight,
		numDiffuseLight,
		world,
		rotation
		);
}

//�J���B
void Puyo::Release()
{
	model.Release();
}