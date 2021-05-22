#include <windows.h>
#include <windowsx.h>

#include "DXRR.h"
#include "GamePadRR.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

tagPOINT pt; //variable para mouse//
DXRR* dxrr;
GamePadRR* gamePad;
bool mientras = true;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"DXRR_E1";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 1020, 750 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"DXRR_E1",
		L"Proyecto Graficas II",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	dxrr = new DXRR(hWnd, 800, 600);
	dxrr->vel = 0;
	gamePad = new GamePadRR(1);

	//ocultar y posicionar el cursor a mitad de la pantalla//
	ShowCursor(FALSE);
	ClientToScreen(hWnd, &pt);
	SetCursorPos((pt.x + 1200 / 2), (pt.y + 750 / 2));


	SetTimer(hWnd, 100, 33, NULL);
	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		dxrr->Render();
	}

	return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	float x, y;

	switch (message)
	{
	case WM_DESTROY:
	{
		KillTimer(hWnd, 100);
		PostQuitMessage(0);
		return 0;
	} break;

	case WM_TIMER: //Aquí pon el gamePad para que funcione con el teclado y el mouse.
	{
		dxrr->izqder = 0;
		dxrr->arriaba = 0;
		if (gamePad->IsConnected())
		{
			dxrr->izqder = 0;
			dxrr->arriaba = 0;

			if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
			{
				dxrr->izqder = 0.01;
			}


			if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
			{
				dxrr->izqder = -0.01;
			}

			if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			{
				dxrr->arriaba = 0.01;
			}


			if (gamePad->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
			{
				dxrr->arriaba = -0.01;
			}

			dxrr->vel = gamePad->GetState().Gamepad.bLeftTrigger / 255.0;

		}
	} break;

		case WM_KEYDOWN:{
			switch (wParam)
			{
			//ARRIBA
			case VK_UP:
				if(dxrr->camara->firstperson)
					dxrr->arriaba = 0.01;
				else
					dxrr->arriaba = -0.01;

				break;

			//ABAJO
			case VK_DOWN:
				if (dxrr->camara->firstperson)
					dxrr->arriaba = -0.01;
				else
					dxrr->arriaba = +0.01;

				break;

			//IZQUIERDA
			case 0x41:
					dxrr->izqder = -0.05;
				break;

			//DERECHA
			case 0x44:
					dxrr->izqder = 0.05;
				break;
	
			//AVANZA
			case 0x57:
				if (dxrr->camara->firstperson)
					dxrr->vel = 5.0;
				else
					dxrr->vel = -5.0;
				break;

			//AVANZA
			case 0x53:
				if (dxrr->camara->firstperson)
					dxrr->vel = -5.0;
				else
					dxrr->vel = +5.0;
			break;

			//AVANZA
			case 0x49:
			dxrr->lerp += 0.05;
			break;

			//AVANZA
			case 0x4E:
			dxrr->lerp -= 0.05;
			break;
			//salir del programa con escape//
			case 0x1B: {
				PostQuitMessage(0);
			}break;
					
			case 32: {
				if (dxrr->camara->firstperson)
					dxrr->camara->firstperson = 0;
				else
					dxrr->camara->firstperson = 1;
				}break;
			}
		}break;
		case WM_KEYUP: {
			switch (wParam)
			{
				//ARRIBA
			case VK_UP:
				dxrr->arriaba = 0.0;
				break;

				//ABAJO
			case VK_DOWN:
				dxrr->arriaba = 0.0;
				break;

				//IZQUIERDA
			case 0x41:
				dxrr->izqder = 0.0;
				break;

				//DERECHA
			case 0x44:
				dxrr->izqder = 0.0;
				break;

				//AVANZA HACIA ADELANTE
			case 0x57:
				dxrr->vel = 0.0;
				break;

				//AVANZA HACIA ATRAS
			case 0x53:
				dxrr->vel = 0.0;
				break;

			
			}
		}break;

		case WM_MOUSEMOVE: {

			POINT mouse;
			GetCursorPos(&mouse);
			x = (mouse.x - (pt.x + 1200 / 2)) * 0.0004f; // el *2 es la sensibilidad del mouse
			y = (mouse.y - (pt.y + 750 / 2)) * 0.0004;


			dxrr->izqder = x;
			dxrr->arriaba = -y;
			SetCursorPos((pt.x + 1200 / 2), (pt.y + 750 / 2)); //centra el cursor

		}

						 break;

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
