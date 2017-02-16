#include "keyboard.h"


KeyBoard::KeyBoard()
{
}


KeyBoard::~KeyBoard()
{
}

void KeyBoard::Update()
{
	memcpy(PrevKeyState, NowKeyState, sizeof(BYTE)* 256);
	GetKeyboardState(NowKeyState);		//256 �̉��z�L�[�̏�Ԃ��A�w�肳�ꂽ�o�b�t�@�փR�s�[���܂��B
}

BYTE KeyBoard::GetKeyPress(int key)
{
	return NowKeyState[key] & 0x80;
}

BYTE KeyBoard::GetKeyTrigger(int key)
{
	//���݂̃L�[��1�t���[���O�̃L�[�̏�Ԃ��r����B
	if (NowKeyState[key] != PrevKeyState[key])
	{
		return NowKeyState[key] & 0x80;
	}
	else{
		return false;
	}
}