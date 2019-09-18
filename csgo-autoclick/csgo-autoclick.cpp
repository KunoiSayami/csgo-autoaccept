
#include <iostream>

#include <windows.h>
#include <stdio.h>

typedef COLORREF (WINAPI *pfGetPixel)(HDC, int, int);

const LONG n = 891;
const LONG s = 867;
const LONG w = 573;
const LONG e = 584;

bool vaild_screen_color(int r, int g, int b, int _r, int _g, int _b) {

	for (int x = s; x <= n; x++)
		for (int y = w; y <= e; y++)
			if (r != _r || g != _g || b != _b)
				return false;

	return true;
}

const int vaildr1 = 0x4c, vaildg1 = 0xb0, vaildb1 = 0x50;
const int vaildr2 = 0x5b, vaildg2 = 0xcb, vaildb2 = 0x5f;


int main(int argc, char** argv)
{
	pfGetPixel pGetPixel;

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
					POINT _cursor;
					GetCursorPos(&_cursor);
					COLORREF _color = (*pGetPixel) (_hdc, _cursor.x, _cursor.y);
					int _red = GetRValue(_color);
					int _green = GetGValue(_color);
					int _blue = GetBValue(_color);

					
					if (!vaild_screen_color(_red, _green, _blue, vaildr1, vaildg1, vaildb1) || 
						!vaild_screen_color(_red, _green, _blue, vaildr2, vaildg2, vaildb2)) {
						ReleaseDC(NULL, _hdc);
						continue;
					}
					printf("%d %d, Red: 0x%02x ", _cursor.x, _cursor.y, _red);
					printf("Green: 0x%02x ", _green);
					printf("Blue: 0x%02x\n", _blue);
					
					keybd_event(VK_SCROLL, 0, 0, 0);
					keybd_event(VK_SCROLL, 0, KEYEVENTF_KEYUP, 0);
				}
				ReleaseDC(NULL, _hdc);
			}
			Sleep(1000);
		}
		FreeLibrary(_hGDI);
	}
	return 0;
}
