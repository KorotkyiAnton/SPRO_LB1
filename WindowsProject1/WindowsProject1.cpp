#include <windows.h> 
#include <vector>
#include "time.h"
#include <string>

HINSTANCE hInst;
LPCTSTR szWindowClass = "LB1";
LPCTSTR szTitle = "LB1";
int first_numb, second_numb, sub_numb;
BOOL first = false;
BOOL sub = true;
int Start1, Res1, Start2, Res2, Start3, Res3;



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

	Start1 = GetTickCount64();
	HWND hWnd = (HWND)lpParam;
	HDC hdc = GetDC(hWnd);
	RECT rt;
	srand(time(0));
	
	while (true) {
		if (sub) {
			first_numb = (rand() % 10000) + 1;
			TextOutA(hdc, 10, 10, "First number:", 13);
			TextOutA(hdc, 10, 30, std::to_string(first_numb).c_str(), 7);
			first = true;
			sub = false;
			Res1 = GetTickCount64();
			Res1 -= Start1;
			Sleep(1000);
		}
	}
	ReleaseDC(hWnd, hdc);
	return 0;
}

DWORD WINAPI SecondNumbPaint(LPVOID lpParam) {

	Start2 = GetTickCount64();
	HWND hWnd = (HWND)lpParam;
	HDC hdc = GetDC(hWnd);
	RECT rt;
	while (true) {
		if (first) {
			second_numb = (rand() % 10000) + 1;
			TextOutA(hdc, 10, 50, "Second number:", 14);
			TextOutA(hdc, 10, 70, std::to_string(second_numb).c_str(), 7);
			first = false;
			Res2 = GetTickCount64();
			Res2 -= Start2;
		}
	}
	ReleaseDC(hWnd, hdc);
	return 0;

}

DWORD WINAPI SubNumbPaint(LPVOID lpParam) {

	Start3 = GetTickCount64();
	HWND hWnd = (HWND)lpParam;
	HDC hdc = GetDC(hWnd);
	RECT rt;

	while (true) {
		if (first) {
			sub_numb = first_numb - second_numb;
			TextOutA(hdc, 10, 90, "Result:", 7);
			TextOutA(hdc, 10, 110, std::to_string(sub_numb).c_str(), 7);
			sub = true;
			Res3 = GetTickCount64();
			Res3 -= Start3;
		}
	}
	ReleaseDC(hWnd, hdc);
	return 0;

}

DWORD WINAPI Timer(LPVOID lpParam) {

	HWND hWnd = (HWND)lpParam;
	HDC hdc = GetDC(hWnd);
	RECT rt;

	while (true) {
		Sleep(2000);
		TextOutA(hdc, 10, 130, "Timer:", 6);
		TextOutA(hdc, 10, 150, std::to_string(Res1).c_str(), std::to_string(Res1).size());
		TextOutA(hdc, 10, 170, std::to_string(Res2).c_str(), std::to_string(Res2).size());
		TextOutA(hdc, 10, 190, std::to_string(Res3).c_str(), std::to_string(Res3).size());
	}
	ReleaseDC(hWnd, hdc);
	return 0;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HANDLE hFirstNumb;
	static HANDLE hSecondNumb;
	static HANDLE hSubNumb;
	static HANDLE hTimer;
	HDC hdc;

	switch (message) {
	case WM_CREATE:
		hFirstNumb = CreateThread(NULL, 0, *FirstNumbPaint, (LPVOID)hWnd, 0, NULL);
		hSecondNumb = CreateThread(NULL, 0, *SecondNumbPaint, (LPVOID)hWnd, 0, NULL);
		hSubNumb = CreateThread(NULL, 0, *SubNumbPaint, (LPVOID)hWnd, 0, NULL);
		hTimer = CreateThread(NULL, 0, *Timer, (LPVOID)hWnd, 0, NULL);
		break;

	case WM_SIZE:
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_CLOSE:
		CloseHandle(hFirstNumb);
		CloseHandle(hSecondNumb);
		CloseHandle(hSubNumb);
		CloseHandle(hTimer);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}