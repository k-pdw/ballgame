#pragma once

#ifndef _MYUTIL
#define _MYUTIL

static int m_Patterns[5][5] = { {1,1,1,1,1}, {1,1,1,1,1}, {1,1,1,1,1},
								{1,1,1,1,1}, {1,1,1,1,1} };

POINT CenterPoint(RECT &r);
int CheckStrikeBar(RECT &r, RECT &bound);
int CheckBallBoundX(RECT &r, RECT &bound);
int CheckBallBoundY(RECT &r, RECT &bound);
void DrawObject(HDC hdc, RECT &r, COLORREF color, int type);
void DrawObject(HDC hdc, RECT &r, COLORREF penColor, COLORREF brushColor, int type);
int HitTest(RECT &r1, RECT &r2);
int HitTest(HDC hdc, RECT &ball, RECT blocks[][5]);
void InitBlocks(RECT blocks[][5], RECT &bound);
void DrawBlocks(HDC hdc, RECT blocks[][5]);

#endif
