#include "stdafx.h"
#include "feild.h"
#include "puyo.h"
#include "puyopuyo.h"
#include "keyboard.h"

extern Puyo* puyo;
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

void Feild::sakujo()
{
	for (short y = TATE - 1; y >= 0; y--)
	{
		for (short x = 0; x < YOKO; x++)
		{
			if (deadflg[y][x] == true)
			{
				bool flg = true;
				PuyoMap[y][x]->SetDead(flg);
				PuyoMap[y][x] = nullptr;
				map[y][x] = false;
				//mapdown(x, y);
				Number[y][x] = NULL;
			}
		}
	}
}


void Feild::mapdown(int x, int y)
{
	if (map[y][x] == false && map[y - 1][x] == true)
	{
		map[y][x] = map[y - 1][x];
		map[y - 1][x] = map[y - 2][x];
	}
}


int Feild::tansaku(int x,int y,int num)
{
	if (Number[y][x] != num || cmb[y][x] != 0 || num == 0)
	{
		return 0;
	}

	cmb[y][x] = 1;
	deadflg[y][x] = true;
	int ret = 1;

	if (y > 0)
	{
		ret += tansaku(x + 1, y, num);
		ret += tansaku(x - 1, y, num);
		ret += tansaku(x, y - 1, num);
		ret += tansaku(x, y + 1, num);
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
			map[i][j] = false;
			Number[i][j] = MapState::None;
			cmb[i][j] = false;
			PuyoMap[i][j] = nullptr;
			deadflg[i][j] = false;
		}
	}
	model.Init(g_pd3dDevice, "court.x");
	gameoverflg = false;
}


//�X�V
void Feild::Update(PuyoPuyo* puyopuyo)
{
	//bool f = true;
	//for (short x = 0; x < YOKO; x++)
	//{
	//	if (!map[9][x])
	//	{
	//		f = false;
	//		break;
	//	}
	//}
	//if (f)
	//{
	//	int y = 9;
	//	for (short x = 0; x < YOKO; x++)
	//	{
	//		sakujo(x, y);
	//	}
	//}

	if (puyopuyo->GetIsDownEnd() == false)
	{
		for (short y = TATE - 1; y >= 0; y--)
		{
			for (short x = 0; x < YOKO; x++)
			{
				int num = 0, ret = 0;
				num = Number[y][x];
				ret = tansaku(x, y, num);
				if (ret >= 4)
				{
					sakujo();
				}
				//���S�t���O�̃��Z�b�g
				deadflg[y][x] = false;
			}
		}

		for (short i = 0; i < TATE; i++)
		{
			for (short j = 0; j < YOKO; j++)
			{
				//�T���������m�F����z��̃��Z�b�g�B
				cmb[i][j] = false;
			}
		}

	}

	
	if (gameoverflg==true)
	{
		Puyo** puyo;
		puyo = puyopuyo->GetPuyoArray();
		for (short i = 0; i < 2; i++){
			if (GetMap(puyo[i]->GetiPos_X(), puyo[i]->GetiPos_Y()) == true)
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

bool Feild::GetMap(int x, int y)
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

bool Feild::SetMap(int x, int y, bool isDead)
{
	if (isDead == false)
	{
		isDead = true;
	}
	map[y][x] = isDead;
	if (y <= 0)
	{
		return true;
	}
	return false;
}

void Feild::SetPuyo(int x, int y, Puyo* puyo)
{
	PuyoMap[y][x] = puyo;
	Number[y][x] = puyo->GetNum();
}

void Feild::SetMapNull(int x, int y)
{
	map[y][x] = MapState::None;
}

void Feild::SetNullNum(int x, int y)
{
	Number[y][x] = MapState::None;
}

