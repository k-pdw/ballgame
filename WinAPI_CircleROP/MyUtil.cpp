#include <Windows.h>
#include <tchar.h>
#include "MyUtil.h"

POINT CenterPoint(RECT &r)
{
	POINT p;
	p.x = (r.left + r.right) / 2;
	p.y = (r.top + r.bottom) / 2;

	return p;
}

int CheckStrikeBar(RECT &r, RECT &bound)
{
	if (r.left <= bound.left)
		return 1;	// 왼쪽 벽에 충돌함
	else if (r.right >= bound.right)
		return 2;	// 오른쪽 벽에 충돌함
	else
		return 0;	// 벽에 출돌하지 않음
}

int CheckBallBoundX(RECT &r, RECT &bound)
{
	if (r.left <= bound.left || r.right >= bound.right)
		return 1;	// 좌/우 벽에 충돌함
	else
		return 0;	// 벽에 출돌하지 않음
}

int CheckBallBoundY(RECT &r, RECT &bound)
{
	if (r.top <= bound.top || r.bottom >= bound.bottom)
		return 1;	// 상/하 벽에 충돌함
	else
		return 0;	// 벽에 출돌하지 않음
}

void DrawObject(HDC hdc, RECT &r, COLORREF color, int type)
{
	DrawObject(hdc, r, color, color, type);
}

void DrawObject(HDC hdc, RECT &r, COLORREF penColor, COLORREF brushColor, int type)
{
	HPEN	hPen, hOldPen;
	HBRUSH	hBrush, hOldBrush;

	SetROP2(hdc, R2_XORPEN);
	
	hPen = CreatePen(PS_SOLID, 1, penColor);	// 빨강색 선 생성
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(brushColor);		// 빨강색 브러시 생성
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	switch (type)
	{
	case 0:
		Rectangle(hdc, r.left, r.top, r.right, r.bottom);
		break;
	case 1:
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		break;
	}
	
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

int HitTest(RECT &r1, RECT &r2)
{
	RECT tmp;
	int  w, h;
	if (IntersectRect(&tmp, &r1, &r2)) {
		w = tmp.right - tmp.left;
		h = tmp.bottom - tmp.top;
		if (w < h)		// 공이 바의 옆면에 맞은 경우
			return 1;
		else
			return 2;	// 공이 바의 윗면에 맞은 경우
	}
	else
		return 0;		// 공이 바에 맞지 않은 경우
}

int HitTest(HDC hdc, RECT &ball, RECT blocks[][5])
{
	int result;
	COLORREF	color = 255;

	for (int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++)
			if (m_Patterns[i][j]) {
				result = HitTest(ball, blocks[i][j]);

				if (result) {
					m_Patterns[i][j] = 0;
					DrawObject(hdc, blocks[i][j], RGB(0, 0, 0), RGB(0, color, color), 0);
					return result;
				}
			}
		color -= 25;
	}

	return 0;
}

void InitBlocks(RECT blocks[][5], RECT &bound)
{
	RECT r, t;
	int  w, h;

	r = bound;		r.right /= 5;
	r.top = 20;
	r.bottom = r.top + 20;

	w = r.right - r.left;		// 블럭의 가로 사이즈
	h = r.bottom - r.top;		// 블럭의 세로 사이즈

	for (int i = 0; i < 5; i++) {
		t = r;
		OffsetRect(&r, 0, h);

		for (int j = 0; j < 5; j++) {
			blocks[i][j] = t;
			OffsetRect(&t, w, 0);	// 블럭의 좌표값 기록
		}
	}
}

void DrawBlocks(HDC hdc, RECT blocks[][5])
{
	COLORREF color = 255;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (m_Patterns[i][j])
				DrawObject(hdc, blocks[i][j], RGB(0, 0, 0), RGB(0, color, color), 0);
		}

		color -= 25;
	}
}