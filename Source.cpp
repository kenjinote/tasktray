#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "resource.h"

#define MYWM_NOTIFYICON (WM_APP+100)
#define TRAY_ID 1

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT point;
	NOTIFYICONDATA tnd;

	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_CREATE:
		tnd.cbSize = sizeof(NOTIFYICONDATA);
		tnd.hWnd = hWnd;
		tnd.uID = TRAY_ID;
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = MYWM_NOTIFYICON;
		tnd.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
		lstrcpyn(tnd.szTip, L"ここ", sizeof(tnd.szTip));
		Shell_NotifyIcon(NIM_ADD, &tnd);
		break;
	case WM_ENDSESSION:
		DestroyWindow(hWnd);
		return 0;
	case WM_CLOSE:
		tnd.cbSize = sizeof(NOTIFYICONDATA);
		tnd.hWnd = hWnd;
		tnd.uID = TRAY_ID;
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = MYWM_NOTIFYICON;
		tnd.hIcon = NULL;
		*tnd.szTip = '\0';
		Shell_NotifyIcon(NIM_DELETE, &tnd);
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case MYWM_NOTIFYICON:
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
			ShowWindow(hWnd, SW_SHOW);
			if (IsIconic(hWnd) != 0) {
				ShowWindow(hWnd, SW_RESTORE);
			}
			SetForegroundWindow(hWnd);
			break;
		case WM_RBUTTONDOWN:
			SetForegroundWindow(hWnd);
			GetCursorPos((LPPOINT)&point);
			TrackPopupMenu(GetSubMenu(LoadMenu(GetModuleHandle(0), (LPCTSTR)IDR_MENU1), 0), TPM_TOPALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON,
				point.x, point.y, 0, hWnd, NULL);
			PostMessage(hWnd, WM_NULL, 0, 0);
			break;
		}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
