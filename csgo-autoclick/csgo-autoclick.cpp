
#include <iostream>

#include <windows.h>
#include <stdio.h>

typedef COLORREF (WINAPI *pfGetPixel)(HDC, int, int);
pfGetPixel pGetPixel;

const LONG n = 990;
const LONG s = 867;
const LONG w = 579;
const LONG e = 586;

bool vaild_screen_color(HDC hdc, int _r, int _g, int _b) {
	POINT pt;
	COLORREF _color;
	int _red, _green, _blue;
	for (pt.x = s; pt.x <= n; pt.x++)
		for (pt.y = w; pt.y <= e; pt.y++) {
			_color = (*pGetPixel) (hdc, pt.x, pt.y);

			_red = GetRValue(_color);
			_green = GetGValue(_color);
			_blue = GetBValue(_color);
			printf("0x%02x 0x%02x 0x%02x\n", _red, _green, _blue);
			if (_red != _r || _green != _g || _blue != _b)
				return false;
		}

	/*printf("%d %d, Red: 0x%02x ", _cursor.x, _cursor.y, _red);
	printf("Green: 0x%02x ", _green);
	printf("Blue: 0x%02x\n", _blue);*/
	return true;
}

const int vaildr1 = 0x4c, vaildg1 = 0xb0, vaildb1 = 0x4e;
const int vaildr2 = 0x58, vaildg2 = 0xcc, vaildb2 = 0x5f;


int main(int argc, char** argv)
{
	POINT pt;
	// https://stackoverflow.com/questions/4839623/getting-pixel-color-in-c
	HINSTANCE _hGDI = LoadLibrary("gdi32.dll");
	if (_hGDI)
	{
		pGetPixel = (pfGetPixel)GetProcAddress(_hGDI, "GetPixel");
		while (true) {
			while (GetKeyState(VK_SCROLL)) {
				Sleep(100);
				HDC _hdc = GetDC(NULL);
				if (_hdc)
				{
					/*POINT _cursor;
					GetCursorPos(&_cursor);*/

					if (!vaild_screen_color(_hdc, vaildr1, vaildg1, vaildb1) &&
						!vaild_screen_color(_hdc, vaildr2, vaildg2, vaildb2)) {
						ReleaseDC(NULL, _hdc);
						continue;
					}

					//SetCursorPos(891, 573);
					printf("ALL MATCHED");
					mouse_event(MOUSEEVENTF_LEFTDOWN, 982, 635, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 982, 635, 0, 0);

					keybd_event(VK_SCROLL, 0, 0, 0);
					keybd_event(VK_SCROLL, 0, KEYEVENTF_KEYUP, 0);
				}
				ReleaseDC(NULL, _hdc);
			}
			Sleep(1000);
			GetCursorPos(&pt);
			printf("%d %d\n", pt.x, pt.y);
		}
		FreeLibrary(_hGDI);
	}
	return 0;
}
