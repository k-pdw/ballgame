#include <Windows.h>
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
		return 1;	// ���� ���� �浹��
	else if (r.right >= bound.right)
		return 2;	// ������ ���� �浹��
	else
		return 0;	// ���� �⵹���� ����
}

int CheckStrikeX(RECT &r, RECT &bound)
{
	if (r.left <= bound.left || r.right >= bound.right)
		return 1;	// ��/�� ���� �浹��
	else
		return 0;	// ���� �⵹���� ����
}

int CheckStrikeY(RECT &r, RECT &bound)
{
	if (r.top <= bound.top || r.bottom >= bound.bottom)
		return 1;	// ��/�� ���� �浹��
	else
		return 0;	// ���� �⵹���� ����
}

void DrawObject(HDC hdc, RECT &r, COLORREF color, int type)
{
	DrawObject(hdc, r, color, color, type);
}

void DrawObject(HDC hdc, RECT &r, COLORREF penColor, COLORREF brushColor, int type)
{
	HPEN	hPen, hOldPen;
	HBRUSH	hBrush, hOldBrush;
	
	hPen = CreatePen(PS_SOLID, 1, penColor);	// ������ �� ����
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(brushColor);		// ������ �귯�� ����
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
		if (w < h)		// ���� ���� ���鿡 ���� ���
			return 1;
		else
			return 2;	// ���� ���� ���鿡 ���� ���
	}
	else
		return 0;		// ���� �ٿ� ���� ���� ���

}