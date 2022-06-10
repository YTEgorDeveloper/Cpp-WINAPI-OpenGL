#include <vector>
#include <Windows.h>
#include <gl/freeglut.h>

#include "SoftwareMain.h"

/*
  - Default WINAPI and OpenGL window example
  - Basic settings are done, created camera control menu buttons
  - Using EgorDeveloper's libraries Components.h
  - By: EgorDeveloper
  - 
  - SoftwareMain.cpp:
  - Contains WINMAIN and WindowProcedure realisations
*/

//Main window procedure

LRESULT CALLBACK MainWndProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	
	case WM_COMMAND:
		switch (wParam) {
		
		case CMDCameraOrtho:
			renderer.camera.SetOrtho();
			renderer.init();
			CheckMenuItem(CameraModeMenu, 0, MF_BYPOSITION | MF_CHECKED);
			CheckMenuItem(CameraModeMenu, 1, MF_BYPOSITION | MF_UNCHECKED);
			break;
		case CMDCameraPersp:
			renderer.camera.SetPerspective();
			renderer.init();
			CheckMenuItem(CameraModeMenu, 1, MF_BYPOSITION | MF_CHECKED);
			CheckMenuItem(CameraModeMenu, 0, MF_BYPOSITION | MF_UNCHECKED);
			break;
		case CMDCameraPos1:
			renderer.camera.SetCameraPosition(points[0]);
			break;
		case CMDCameraPos2:
			renderer.camera.SetCameraPosition(points[1]);
			break;
		case CMDCameraPos3:
			renderer.camera.SetCameraPosition(points[2]);
			break;
		case CMDCameraPos4:
			renderer.camera.SetCameraPosition(points[3]);
			break;
		
		default: return 0;
		}
		return 0;

	case WM_CLOSE:
		ExitSoftware();
		break;

	case WM_CREATE:
		MainWndAddMenus(hWnd);
		CheckMenuItem(CameraModeMenu, 1, MF_BYPOSITION | MF_CHECKED);
		break;

	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//Application entry point

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	if (!MainWndRegisterClass(hInstance, (HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_QUESTION))) { MessageBox(NULL, L"RegisterClass() failed", L"Error", MB_ICONERROR | MB_OK); return FALSE; }
	if (!CreateRenderContext(hInstance, L"OpenGL App")) { MessageBox(NULL, L"CreateRenderContext() failed", L"Error", MB_ICONERROR | MB_OK); return FALSE; }
	wglMakeCurrent(hDC, hRC);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	renderer.init();

	Quaternion q = Quaternion::EulerAngles(0, PI / 4, 0);
	Mesh m = Mesh::GenerateCuboid(Vector3(1, 1, 1));
	Material mat = Material(Material::diffuse, 0, 0.15f);
	
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		
		/*				Frame draw begin			*/
		renderer.BeginFrame();

		renderer.RenderGrid(-5, 5, 9, -5, 5, 9, 0, false, Color(50, 50, 50));
		renderer.RenderPoints(points, Color(220, 150, 10));
		renderer.RenderMesh(m, Vector3(0.1f), q, Color(150, 220, 10), mat);

		renderer.EndFrame();
		/*				Frame draw end				*/

	}
	return (int)msg.wParam;
}