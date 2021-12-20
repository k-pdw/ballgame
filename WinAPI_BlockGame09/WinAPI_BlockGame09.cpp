#include "framework.h"
#include "WinAPI_BlockGame09.h"
#include <wchar.h>
#include "MyUtil.h"

#define MAX_LOADSTRING 100

HWND		hWndMain;						// 현재 윈도우의 핸들
HINSTANCE	hInst;							// 현재 인스턴스
WCHAR		szTitle[MAX_LOADSTRING];		// 제목 표시줄 텍스트
WCHAR		szWindowClass[MAX_LOADSTRING];	// 기본 창 클래스 이름

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINAPIBLOCKGAME09, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIBLOCKGAME09));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIBLOCKGAME09));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIBLOCKGAME09);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;	// 인스턴스 핸들을 전역 변수에 저장합니다.

	//HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		100, 100, 1000, 800, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC		hdc;
	static	int			alphaX, alphaY, KeyAlpha;	// 공의 이동방향(토글) : 상, 하, 좌, 우
	static	RECT		barR, ballR, clientR;
	static	POINT		mPoint;
	static	HPEN		hPen, hOldPen;
	static	HBRUSH		hBrush, hOldBrush;
	static	COLORREF	barColor, ballColor;
	static	int			vOffset;
	static	int			hitPosition;

	static  RECT		Blocks[5][5];
	static	int			hitNumCount, hitResult, hitResult_2;
	static  int			gameScore;

	switch (message)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		barColor = RGB(0, 0, 255);					// Bar 색 설정
		ballColor = RGB(255, 0, 0);					// Ball 색 설정
		alphaX = 1;	alphaY = 1;						// 공 위치 초기화
		KeyAlpha;

		GetClientRect(hWnd, &clientR);				// 윈도우의 크기 계산
		mPoint = CenterPoint(clientR);
		InitBlocks(Blocks, clientR);

		SetRect(&ballR, mPoint.x - 10, mPoint.y - 10, mPoint.x + 10, mPoint.y + 10);
		SetRect(&barR, mPoint.x - 50, mPoint.y - 15, mPoint.x + 50, mPoint.y + 15);
		OffsetRect(&barR, 0, 300);	// 공의 좌표를 x축으로 0, y축으로 200으로 이동
		SetTimer(hWnd, 1, 10, NULL);
		break;
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			switch (wmId)				// 메뉴 선택을 구문 분석합니다:
			{
			case IDM_ABOUT:
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);

		DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(255, 0, 255), 1);

		//hitNumCount = HitTest(ballR, barR);
		hitResult   = HitTest(ballR, barR);
		hitResult_2 = HitTest(hdc, ballR, Blocks);

		if (CheckStrikeX(ballR, clientR) || hitResult_2 == 1)
			alphaX *= -1;							// 공이 좌/우 벽에 충돌하면 방향전환
		if (CheckStrikeY(ballR, clientR) || hitResult || hitResult_2 == 2)
			alphaY *= -1;							// 공이 상/하 벽에 충돌하면 방향전환

		OffsetRect(&ballR, 3 * alphaX, 3 * alphaY);	// 볼 자동 이동

		if (hitResult_2) {
			hitNumCount++;
			//InvalidateRect(hWnd, &text_Rect, TRUE);
		}

		//gameScore++;
		//InvalidateRect(hWnd, NULL, TRUE);			// 화면 강제 복구

		DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(255, 0, 255), 1);

		ReleaseDC(hWnd, hdc);

		//hitPosition = HitTest(ballR, barR);

		//if (CheckStrikeX(ballR, clientR) || hitPosition == 1)
		//	alphaX *= -1;							// 공이 좌/우 벽에 충돌하면 방향전환
		//if (CheckStrikeY(ballR, clientR) || hitPosition == 2)
		//	alphaY *= -1;							// 공이 상/하 벽에 충돌하면 방향전환

		//OffsetRect(&ballR, 3 * alphaX, 3 * alphaY);	// 볼 자동 이동
		//InvalidateRect(hWnd, NULL, TRUE);			// 화면 강제 복구
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			DrawBlocks(hdc, Blocks);

			DrawObject(hdc, barR, barColor, 0);
			//DrawObject(hdc, ballR, ballColor, 1);
			DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(255, 0, 255), 1);

			DisplayCount(hdc, hitNumCount, clientR);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			KeyAlpha = -1;
			if (CheckStrikeBar(barR, clientR) == 1)
				break;
			else {		// 박스가 벽에 충돌하면 방향전환
				OffsetRect(&barR, KeyAlpha * 5, 0);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_RIGHT:
			KeyAlpha = 1;;
			if (CheckStrikeBar(barR, clientR) == 2)
				break;
			else {		// 박스가 벽에 충돌하면 방향전환
				OffsetRect(&barR, KeyAlpha * 5, 0);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
