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
	BYTE NowKeyState[256];	// 現在のキーの状態(全てのキー)。
	BYTE PrevKeyState[256];	// 1フレーム前のキーの状態(全てのキー)。
};
