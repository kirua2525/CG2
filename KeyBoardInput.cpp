#include "KeyBoardInput.h"
#include <cassert>


#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


void KeyBoardInput::Initialize(HINSTANCE hInstance, HWND hwnd) {

	HRESULT hr;

	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	hr = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));

	//キーボードデバイスの生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));

	//入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	//排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

}

void KeyBoardInput::Update() {

	HRESULT result;

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));


	//キーボード情報の取得開始
	result = keyboard->Acquire();

	//全キーの入力状態を取得する
	result = keyboard->GetDeviceState(sizeof(key), key);








}

bool KeyBoardInput::PushKey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
	
		return true;
	
	}

	//そうでなければfalseを返す
	return false;

}

bool KeyBoardInput::TriggerKey(BYTE keyNumber)
{

	//指定キーを押していればtrueを返す
	if (keyPre[keyNumber]) {

		return true;

	}

	return false;

}
