#include "framework.h"
#include "WinAPI_BallMove.h"
#include <wchar.h>
#include "MyUtil.h"

#define MAX_LOADSTRING 100

HWND				hWndMain;						// 현재 윈도우의 핸들
HINSTANCE			hInst;							// 현재 인스턴스
WCHAR				szTitle[MAX_LOADSTRING];		// 제목 표시줄 텍스트
WCHAR				szWindowClass[MAX_LOADSTRING];	// 기본 창 클래스 이름

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINAPIBALLMOVE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIBALLMOVE));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIBALLMOVE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPIBALLMOVE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;	// 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		100, 100, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static	HDC			hdc;
	static	RECT		ballR, clientR;
	static	POINT		p, q;
	static	int			ballToggleX, ballToggleY;

	switch (message)
	{
	case WM_CREATE:
		ballToggleX = 1;
		ballToggleY = 1;
		GetClientRect(hWnd, &clientR);	// 윈도우의 크기 계산
		p = CenterPoint(clientR);

		SetRect(&ballR, p.x - 30, p.y - 30, p.x + 30, p.y + 30);
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
			KillTimer(hWnd, 1);
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(0, 255, 0), 1);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);

		DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(0, 255, 0), 1);

		if (CheckBallBoundX(ballR, clientR))
			ballToggleX *= -1;					// 공이 좌/우 벽에 충돌하면 방향전환
		if (CheckBallBoundY(ballR, clientR))
			ballToggleY *= -1;					// 공이 상/하 벽에 충돌하면 방향전환

		OffsetRect(&ballR, 5 * ballToggleX, 5 * ballToggleY);	// 볼 자동 이동

		DrawObject(hdc, ballR, RGB(255, 255, 255), RGB(0, 255, 0), 1);

		ReleaseDC(hWnd, hdc);
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

