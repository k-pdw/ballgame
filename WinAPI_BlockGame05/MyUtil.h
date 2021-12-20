#pragma once

#ifndef _MYUTIL
#define _MYUTIL

POINT CenterPoint(RECT &r);
int CheckStrikeX(RECT &r, RECT &bound);
void DrawObject(HDC hdc, RECT &r, COLORREF color, int type);
void DrawObject(HDC hdc, RECT &r, COLORREF penColor, COLORREF brushColor, int type);

#endif
