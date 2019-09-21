
#include <iostream>

#include <windows.h>
#include <stdio.h>

typedef COLORREF (WINAPI *pfGetPixel)(HDC, int, int);
pfGetPixel pGetPixel;

const LONG n = 873;
const LONG s = 912;
const LONG w = 575;
const LONG e = 583;


const int vaildr1 = 0x4c, vaildg1 = 0xaf, vaildb1 = 0x4e;
const int vaildr2 = 0x5a, vaildg2 = 0xcc, vaildb2 = 0x5f;

bool vaild_screen_color(HDC hdc) {
	POINT pt = {n, w};
	COLORREF _color;
	int _red, _green, _blue;
	int _r, _g, _b;

	_color = (*pGetPixel) (hdc, pt.x, pt.y);

	_red = GetRValue(_color);
	_green = GetGValue(_color);
	_blue = GetBValue(_color);
	printf("0x%02x 0x%02x 0x%02x\n", _red, _green, _blue);

	if (_red <= vaildr2 && _red >= vaildr1 && _green <= vaildg2 && _green >= vaildg1 && _blue <= vaildb2 && _blue >= vaildb1)
		_r = _red, _g = _green, _b = _blue;
	else
		return false;
	printf("Enter matching...");
	for (pt.x = n; pt.x <= s; pt.x++)
		for (pt.y = w; pt.y <= e; pt.y++) {
			_color = (*pGetPixel) (hdc, pt.x, pt.y);

			_red = GetRValue(_color);
			_green = GetGValue(_color);
			_blue = GetBValue(_color);
			printf("0x%02x 0x%02x 0x%02x ", _red, _green, _blue);
			if (_red != _r || _green != _g || _blue != _b)
				return false;
		}

	/*printf("%d %d, Red: 0x%02x ", _cursor.x, _cursor.y, _red);
	printf("Green: 0x%02x ", _green);
	printf("Blue: 0x%02x\n", _blue);*/
	return true;
}


int main(int argc, char** argv)
{
	POINT pt;
	// https://stackoverflow.com/questions/4839623/getting-pixel-color-in-c
	HINSTANCE _hGDI = LoadLibrary("gdi32.dll");
	COLORREF _color;
	HDC _hdc;
	int _red, _green, _blue;
	if (_hGDI)
	{
		pGetPixel = (pfGetPixel)GetProcAddress(_hGDI, "GetPixel");
		_hdc = GetDC(NULL);
		while (true) {
			while (GetKeyState(VK_SCROLL)) {
				Sleep(100);
				if (_hdc)
				{
					/*POINT _cursor;
					GetCursorPos(&_cursor);*/

					if (!vaild_screen_color(_hdc)) {
						//ReleaseDC(NULL, _hdc);
						continue;
					}

					//SetCursorPos(891, 573);
					printf("ALL MATCHED\n");
					SetCursorPos(912, 575);
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

					keybd_event(VK_SCROLL, 0, 0, 0);
					keybd_event(VK_SCROLL, 0, KEYEVENTF_KEYUP, 0);
				}
			}
			Sleep(500);
			GetCursorPos(&pt);
			_color = (*pGetPixel) (_hdc, pt.x, pt.y);

			_red = GetRValue(_color);
			_green = GetGValue(_color);
			_blue = GetBValue(_color);
			printf("%d %d 0x%02x 0x%02x 0x%02x\n", pt.x, pt.y, _red, _green, _blue);
		}
		ReleaseDC(NULL, _hdc);
		FreeLibrary(_hGDI);
	}
	return 0;
}
