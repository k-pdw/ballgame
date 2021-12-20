#include "framework.h"
#include "Dialog_TextOut.h"

INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);
HWND hDlgMain;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), HWND_DESKTOP, DlgProc);
	//CreateDialog(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), HWND_DESKTOP, DlgProc);
    return 0;
}

int nBut;

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
		hDlgMain = hDlg;
		//nBut = 0;
        return (INT_PTR)TRUE;

    case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			nBut = 1;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_BUTTON2:
			nBut = 2;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);
			// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			if(nBut == 1)
				TextOut(hdc, 100, 100, TEXT("버튼1번 Test"), 10);
			else
				TextOut(hdc, 100, 120, TEXT("버튼2번 Test"), 10);

			EndPaint(hDlg, &ps);
		}
		break;
    }

    return (INT_PTR)FALSE;
}
