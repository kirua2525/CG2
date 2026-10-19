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

	///<summary>
	///キーの押下をチェック
	///</summary>
    ///<param name= "keyNumber">キー番号( DIK_0 等)</param>
	///<returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);


private:

	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> keyboard = nullptr;

	//全キーの状態
	BYTE key[256] = {};

};

