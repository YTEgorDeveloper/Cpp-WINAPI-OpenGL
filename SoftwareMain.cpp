#include <Windows.h>
#include <gl/freeglut.h>

#include "SoftwareMain.h"

/*
  - Default WINAPI and OpenGL window example
  - Basic settings are done, simple triangle is rendered
  - By: EgorDeveloper
  - 
  - SoftwareMain.cpp:
  - Contains WINMAIN and WindowProcedure realisations
*/

//Main window procedure

LRESULT CALLBACK MainWndProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {

	case WM_CLOSE:
		ExitSoftware();
		break;

	case WM_CREATE:

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

	
	//Setup OpenGL render
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)GLWindowSizeX / (double)GLWindowSizeY, 0.1, 50);
	glMatrixMode(GL_MODELVIEW);
	glPointSize(5);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);			//for no cull use: glDisable(GL_CULL_FACE);


	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		
		/*				Frame draw begin			*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			5, 1, 5,
			0, 0, 0,
			0, 1, 0
		);

		
		glBegin(GL_TRIANGLES);		//Begin sending triangles to render
		glColor3ub(200, 10, 10);
		glVertex3f(1, 0, 0);
		glColor3ub(10, 200, 10);
		glVertex3f(0, 2, 1);
		glColor3ub(10, 10, 200);
		glVertex3f(-1, 0, 1);
		glEnd();					//End sending triangles to render

		glFlush();
		/*				Frame draw end				*/


	}

	return (int)msg.wParam;
}