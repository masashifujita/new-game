#include "stdafx.h"
#include "feild.h"
#include "puyo.h"
#include "puyopuyo.h"
#include "keyboard.h"

extern Puyo puyo;
extern PuyoPuyo* nowPuyoPuyo;
extern KeyBoard g_keyboard;


//�R���X�g���N�^
Feild::Feild()
{
	//�������B
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRotation);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//�f�X�g���N�^
Feild::~Feild()
{
	Release();
}

//void Feild::Con()
//{
//	if (g_keyboard.GetKeyTrigger('A'))
//	{
//		for (short y = TATE - 1; y > 0; y--)
//		{
//			for (short x = 0; x < YOKO; x++)
//			{
//				map[y][x] = NULL;
//				Number[y][x] = NULL;
//				puyo.isDead = true;
//			}
//		}
//	}
//}


//�Ղ悪���Ȃ����Ă��邩�T������֐�
int Feild::Conbine(int x,int y,int num)
{
	//���ׂ����ʒu��puyo�̎�ނ��Ⴄ���͂��łɃ`�F�b�N�ς݂̏ꍇ0��Ԃ��B
	if (Number[y][x] != num || cmb[y][x] != 0)
	{
		return 0;
	}


	cmb[y][x] = 1;
	int ret = 1;

	if (y > 0)
	{
		ret += Conbine(x + 1, y, num);
		ret += Conbine(x - 1, y, num);
	}
	return ret;
}

//�������B
void Feild::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for (short i = 0; i < TATE; i++)
	{
		for (short j = 0; j < YOKO; j++)
		{
			map[i][j] = MapState::None;
			Number[i][j] = MapState::None;
			cmb[i][j] = false;
		}
	}
	model.Init(g_pd3dDevice, "court.x");
	gameoverflg = false;
}

//�X�V�B
void Feild::Update(PuyoPuyo* puyopuyo)
{
	//for (short y = TATE - 1; y > 0; y--)
	//{
	//	for (short x = 0; x < YOKO; x++)
	//	{
	//		int ret = 0, num = 0;
	//		num = Number[y][x];
	//		ret = Conbine(x, y,num);
	//	}
	//}

	
	for (short y = TATE - 1; y > 0; y--)
	{
		for (short x = 0; x < YOKO; x++)
		{
			if (Number[y][x] != 0)
			{
				int num;
				num = Number[y][x];
				int cnt = Conbine(x, y, num);
				if (cnt >= 0)
				{
					puyo.isDead = true;
				}
			}
		}
	}

	if (gameoverflg==true)
	{
		Puyo** puyo;
		puyo = puyopuyo->GetPuyoArray();
		for (short i = 0; i < 2; i++){
			if (GetMap(puyo[i]->GetiPos_X(), puyo[i]->GetiPos_Y()) == MapState::PUYO)
			{
				// �Q�[���I�[�o�[����
				MessageBox(nullptr, ("���[�ނ��[�΁["), ("Message"), MB_OK);
				abort();
				break;
			}
		}
	}

	//���[���h�s��̍X�V�B
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);
}

//�`��B

void Feild::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight,
	bool isDrawShadowMap,
	bool isRecieveShadow
	)
{
	model.Render(pd3dDevice,
		viewMatrix,
		projMatrix,
		diffuseLightDirection,
		diffuseLightColor,
		ambientLight,
		numDiffuseLight,
		mWorld,
		mRotation,
		isDrawShadowMap,
		isRecieveShadow
		);
}

//�J���B
void Feild::Release()
{
	model.Release();
}

int Feild::GetMap(int x, int y)
{
	return map[y][x];
}

// �}�b�v�̎w�肵���ʒu�ɂՂ�𖄂ߍ��ފ֐�
bool Feild::SetMap(int x, int y)
{
	map[y][x] = MapState::PUYO;
	if (y <= 0){
		return true;
	}
	return false;
}


void Feild::SetMapNull(int x, int y)
{
	map[y][x] = MapState::None;
}

void Feild::SetNumMap(int x, int y,int num)
{
	Number[y][x] = num;
}

