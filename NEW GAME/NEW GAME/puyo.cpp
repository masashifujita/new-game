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
	//position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	lefttop = D3DXVECTOR2(0.0, TATE * GRIDSIZE);
	ipos_X = 0;
	ipos_Y = 0;
	rot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	downmoveflg = true;
	checkdownflg = false;
	pos_X = 0;
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
	time += 1.0f / 60.0f;
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
		if (feild->GetMap(ipos_X, ipos_Y) == MapState::PUYO)
		{
			//���݂�ipos_Y�̏ꏊ��puyo������ꍇ�̏���
			downmoveflg = false;
			
			//feild->SetMap(ipos_X,ipos_Y - 1):���̏�����Puyo������ipos_Y�̈ꂩ�����puyo���Z�b�g����B
			feild->SetGameOverDec(feild->SetMap(ipos_X,ipos_Y - 1));
			
			//updata��position�����߂�v�Z������̂�ipos_Y���ꂩ����Ɉړ�������K�v������B
			ipos_Y--;
		}
		else if (ipos_Y == TATE - 1)
		{
			//���݂�ipos_Y�̏ꏊ����ԉ��̒i�������ꍇ�̏���
			downmoveflg = false;

			feild->SetGameOverDec(feild->SetMap(ipos_X,ipos_Y));
		}
	}
}

void Puyo::SideMove()
{
	if (GetAsyncKeyState(VK_UP))
	{
		position.z += 0.1f;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		position.z -= 0.1f;
	}
	if (GetAsyncKeyState(VK_LEFT) && ipos_X > -4)
	{
		ipos_X -= 1;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && ipos_X < 5)
	{
		ipos_X += 1;
	}
}


void Puyo::Init(LPDIRECT3DDEVICE9 pd3dDevice,LPCSTR name)
{
	feild = &g_feild;

	model.Init(pd3dDevice,name);
}

void Puyo::Update()
{
	//DownMove()�֐��Ăяo��
	DownMove();

	if (downmoveflg == true)
	{
		SideMove();
	}

	//������ipos��position�ɕϊ�����B
	position.x = ipos_X * GRIDSIZE + GRIDSIZE * 0.5;
	position.y = lefttop.y - (ipos_Y * GRIDSIZE) - GRIDSIZE * 0.5;

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