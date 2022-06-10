#pragma once

/*
  - Default WINAPI and OpenGL window example
  - Basic settings are done, simple triangle is rendered
  - By: EgorDeveloper
  -
  - SoftwareMain.h:
  - Contains main app definitions, function prototypes, realisations
*/

#include "Components.h"

#define MainWindowSizeX		1600
#define MainWindowSizeY		958
#define GLWindowSizeX		1600
#define GLWindowSizeY		900

#define GLWindowPosX		0
#define GLWindowPosY		0

#define CMDCameraPos1		10
#define CMDCameraPos2		11
#define CMDCameraPos3		12
#define CMDCameraPos4		13
#define CMDCameraOrtho		14
#define CMDCameraPersp		15


HDC     hDC;					/* device context */
HGLRC   hRC;					/* render context (opengl context) */
HWND    hWnd, GLWnd;			/* window */

HMENU	CameraPosMenu, CameraModeMenu;

Renderer renderer = Renderer(Camera(Vector3(3, 4, 3), Vector3(0, 1, 0)));
std::vector<Vector3> points = Vector3::CirclePoints(4, 6, Vector3(0, 3.5f, 0), Quaternion::EulerAngles(0.07f, 0.5f, 0.059f));

LRESULT CALLBACK MainWndProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

void ExitSoftware() {			/* Application close function */
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);
	PostQuitMessage(0);
}

ATOM MainWndRegisterClass(HINSTANCE hInstance, HBRUSH BGColor, HCURSOR Cursor, HICON Icon) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)MainWndProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = Icon;
	wcex.hCursor = Cursor;
	wcex.hbrBackground = BGColor;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"SoftwareMain";
	wcex.hIconSm = Icon;

	return RegisterClassExW(&wcex);
}

BOOL CreateRenderContext(HINSTANCE hInstance, LPCWSTR mainWndName) {
	hWnd = CreateWindowW(L"SoftwareMain", mainWndName, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 100, 100, MainWindowSizeX, MainWindowSizeY, nullptr, nullptr, hInstance, nullptr);
	GLWnd = CreateWindowA("static", NULL, WS_VISIBLE | WS_CHILD, GLWindowPosX, GLWindowPosY, GLWindowSizeX, GLWindowSizeY, hWnd, NULL, NULL, NULL);
	if (!hWnd) { return FALSE; }
	
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };
	int                   pixelFormat;
	hDC = GetDC(GLWnd);

	pixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 32;
	pixelFormat = ChoosePixelFormat(hDC, &pixelFormatDescriptor);

	SetPixelFormat(hDC, pixelFormat, &pixelFormatDescriptor);
	DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor);
	hRC = wglCreateContext(hDC);

	return (hRC != NULL);
}

void MainWndAddMenus(HWND hWndMain) {
	HMENU RootMenu = CreateMenu();
	
	CameraPosMenu = CreateMenu();
	CameraModeMenu = CreateMenu();
	
	AppendMenu(CameraPosMenu, MF_STRING, CMDCameraPos1, L"Position 1");
	AppendMenu(CameraPosMenu, MF_STRING, CMDCameraPos2, L"Position 2");
	AppendMenu(CameraPosMenu, MF_STRING, CMDCameraPos3, L"Position 3");
	AppendMenu(CameraPosMenu, MF_STRING, CMDCameraPos4, L"Position 4");

	AppendMenu(CameraModeMenu, MF_STRING, CMDCameraOrtho, L"Ortho");
	AppendMenu(CameraModeMenu, MF_STRING, CMDCameraPersp, L"Perspective");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)CameraPosMenu, L"Position");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)CameraModeMenu, L"View mode");

	SetMenu(hWndMain, RootMenu);
}