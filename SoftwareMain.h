#pragma once

/*
  - Default WINAPI and OpenGL window example
  - Basic settings are done, simple triangle is rendered
  - By: EgorDeveloper
  -
  - SoftwareMain.h:
  - Contains main app definitions, function prototypes, realisations
*/

#define MainWindowSizeX 1600
#define MainWindowSizeY 900
#define GLWindowSizeX 1600
#define GLWindowSizeY 900

#define GLWindowPosX 0
#define GLWindowPosY 0

HDC     hDC;					/* device context */
HGLRC   hRC;					/* render context (opengl context) */
HWND    hWnd, GLWnd;			/* window */

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