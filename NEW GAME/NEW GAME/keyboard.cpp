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
	GetKeyboardState(NowKeyState);		//256 個の仮想キーの状態を、指定されたバッファへコピーします。
}

BYTE KeyBoard::GetKeyPress(int key)
{
	return NowKeyState[key] & 0x80;
}

BYTE KeyBoard::GetKeyTrigger(int key)
{
	//現在のキーと1フレーム前のキーの状態を比較する。
	if (NowKeyState[key] != PrevKeyState[key])
	{
		return NowKeyState[key] & 0x80;
	}
	else{
		return false;
	}
}