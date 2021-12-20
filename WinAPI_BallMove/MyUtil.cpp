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
	
	hPen = CreatePen(PS_SOLID, 2, penColor);	// 펜 생성
	hBrush = CreateSolidBrush(brushColor);		// 브러시 생성
	hOldPen = (HPEN)SelectObject(hdc, hPen);
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

