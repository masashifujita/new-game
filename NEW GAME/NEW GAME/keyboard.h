#pragma once
#include "stdafx.h"
class KeyBoard
{
public:
	KeyBoard();
	~KeyBoard();
	void Update();
	BYTE GetKeyPress(int);
	BYTE GetKeyTrigger(int);
private:
	BYTE NowKeyState[256];	// ���݂̃L�[�̏��(�S�ẴL�[)�B
	BYTE PrevKeyState[256];	// 1�t���[���O�̃L�[�̏��(�S�ẴL�[)�B
};
