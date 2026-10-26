#include <windows.h>
#include <cstdint>

#pragma once
class WindAPI
{
public:		//静的メンバ関数

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);





public:		//メンバ関数

	void Initialize();

	void UpDate();



private:





};

