#include "stdafx.h"
#include "feild.h"
#include "puyo.h"
#include "puyopuyo.h"

extern Puyo puyo;
extern PuyoPuyo* nowPuyoPuyo;


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

//�������B
void Feild::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for (short i = 0; i < TATE; i++)
	{
		for (short j = 0; j < YOKO; j++)
		{
			map[i][j] = MapState::None;
			Number[i][j] = MapState::None;
		}
	}
	model.Init(g_pd3dDevice, "court.x");
	gameoverflg = false;
}

//�X�V�B
void Feild::Update(PuyoPuyo* puyopuyo)
{
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

int Feild::Conbine(int x,int y,int num)
{
	if (Number[y][x] != num)
		return 0;
	int cnt = 0;
	if (y > 0)
	{
		cnt += Conbine(x + 1, y, num);
		cnt += Conbine(x - 1, y, num);
		cnt += Conbine(x, y + 1, num);
		cnt += Conbine(x, y - 1, num);
		return cnt;
	}
}

bool Feild::Sakujo()
{
	//for (int i = TATE - 1; i >= 0; i--)
	//{
	//	for (int j = 0; j < YOKO; j++)
	//	{
	//		int ret = Conbine(j, i, Number[i][j]);
	//		if (ret >= 4)
	//		{
	//			return true;
	//		}
	//		else
	//		{
	//			return false;
	//		}
	//	}
	//}
	int cnt = 0;
	for (short i = TATE - 1; i <= 0; i--)
	{
		for (short j = 0; j < YOKO; j++)
		{
			if (Number[i][j] = Number[i][j + 1])
			{
				cnt++;
				if (cnt <= 4)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
}
