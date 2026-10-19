#pragma once

#include <windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


class KeyBoardInput
{

public:

	//初期化
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	//更新
	void  Update();

	

public:

//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


private:

	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard = nullptr;




};

