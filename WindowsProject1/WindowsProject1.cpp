#include <windows.h> 
#include <vector>
#include "time.h"
#include <string>

HINSTANCE hInst;
LPCTSTR szWindowClass = "Korotkyi Anton";
LPCTSTR szTitle = "Korotkyi Anton";
int first_numb, second_numb, sub_numb;
BOOL first = false;
BOOL second = false;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW, CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_HAND);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 182, 153));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hWnd;
	hInst = hInstance;
	hWnd = CreateWindow(szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_BORDER,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd) {
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

DWORD WINAPI FirstNumbPaint(LPVOID lpParam) {

	HWND hWnd = (HWND)lpParam;
	HDC hdc = GetDC(hWnd);
	RECT rt;
	srand(time(0));
	
	while (true) {
		if (!second) {
			first_numb = (rand() % 10000) + 2;
			TextOutA(hdc, 10, 10, std::to_string(first_numb).c_str(), 5);
			first = true;
			second = false;
		}
	}
	ReleaseDC(hWnd, hdc);
	return 0;
}

DWORD WINAPI SecondNumbPaint(LPVOID lpParam) {

	HWND hWnd = (HWND)lpParam;
	HDC hdc = GetDC(hWnd);
	RECT rt;
	while (true) {
		if (first) {
			second_numb = (rand() % 10000) + 1;
			TextOutA(hdc, 10, 30, std::to_string(second_numb).c_str(), 5);
			second = true;
			first = false;
		}
	}
	ReleaseDC(hWnd, hdc);
	return 0;

}

DWORD WINAPI SubNumbPaint(LPVOID lpParam) {

	HWND hWnd = (HWND)lpParam;
	HDC hdc = GetDC(hWnd);
	RECT rt;

	while (true) {
		if (!first&&second) {
			sub_numb = first_numb - second_numb;
			TextOutA(hdc, 10, 50, std::to_string(sub_numb).c_str(), 5);
			second = false;
		}
	}
	ReleaseDC(hWnd, hdc);
	return 0;

}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HANDLE hFirstNumb;
	static HANDLE hSecondNumb;
	static HANDLE hSubNumb;

	switch (message) {
	case WM_CREATE:
		hFirstNumb = CreateThread(NULL, 0, *FirstNumbPaint, (LPVOID)hWnd, 0, NULL);
		hSecondNumb = CreateThread(NULL, 0, *SecondNumbPaint, (LPVOID)hWnd, 0, NULL);
		hSubNumb = CreateThread(NULL, 0, *SubNumbPaint, (LPVOID)hWnd, 0, NULL);
		break;

	case WM_SIZE:
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_CLOSE:
		CloseHandle(hFirstNumb);
		CloseHandle(hSecondNumb);
		CloseHandle(hSubNumb);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}