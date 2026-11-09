#include <windows.h>
#include <cstdint>

#pragma once
class WindAPI
{
public:		//静的メンバ関数

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//getter
	HINSTANCE GetHInstance() const { return wc.hInstance; }

	//メッセージの処理
	bool ProcessMessage();

public:		//メンバ関数

	void Initialize();

	void UpDate();

	void Finalize(); //終了

public:		//定数

	//クライアント領域のサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	//getter
	HWND GetHwnd() const { return hwnd; }

private:

	//ウィンドハンドル
	HWND hwnd = nullptr;

	//ウィンドウクラスの設定
	WNDCLASS wc{};

};

