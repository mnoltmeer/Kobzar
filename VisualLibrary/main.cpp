/*!
Copyright 2024 Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of Kobzar Engine.

	Kobzar Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Kobzar Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Kobzar Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <process.h>
#include <gdiplus.h>

#pragma hdrstop
#pragma argsused

#include "eli_interface.h"
#include "..\..\work-functions\MyFunc.h"
#include "..\..\work-functions\Logs.h"

#pragma comment(lib, "gdiplus.lib")

ELI_INTERFACE *eIface;

//Власне повідомлення для оновлення зображення
#define WM_KVL_DRAW_IMAGE (WM_USER + 1)

HINSTANCE DllHinst;
HANDLE WndThread;
HWND WHandle;
// Віртуальне вікно (буфер)
HBITMAP hBitmap = NULL;
HDC hMemDC = NULL;
int WndWidth = 800, WndHeight = 600;
bool FullScreen = 0;
CRITICAL_SECTION cs;  //Для безпечного доступу до пам’яті

// Функція створення віртуального вікна (буфера)
void CreateVirtualWindow(HWND hWnd, int width, int height)
{
  try
	 {
	   HDC hdc = GetDC(hWnd);
	   hMemDC = CreateCompatibleDC(hdc);
	   hBitmap = CreateCompatibleBitmap(hdc, width, height);
	   SelectObject(hMemDC, hBitmap);
	   ReleaseDC(hWnd, hdc);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

// Функція малювання у віртуальному вікні
void DrawToVirtualWindow(const wchar_t* file, int x, int y)
{
  try
	 {
	   if (!hMemDC)
		 throw ("Virtual buffer not exists!");

//Очистка фону
	   RECT rect = {0, 0, WndWidth, WndHeight};
	   FillRect(hMemDC, &rect, (HBRUSH)(COLOR_WINDOW+1));

//Завантаження зображення через GDI+
	   Gdiplus::Graphics graphics(hMemDC);
	   Gdiplus::Image image(file);

	   if (image.GetLastStatus() == Gdiplus::Ok)
		 graphics.DrawImage(&image, x, y);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawToVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

// Функція копіювання вмісту віртуального вікна в основне
void CopyVirtualToMain(HDC hdc)
{
  try
	 {
	   BitBlt(hdc, 0, 0, WndWidth, WndHeight, hMemDC, 0, 0, SRCCOPY);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CopyVirtualToMain: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

LRESULT CALLBACK HostWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int sx, sy; //розміри вікна
  static POINT mouse; //координати курсора миші

  switch (message)
	{
	  case WM_CREATE:
		{
		  break;
		}

	  case WM_SIZE:
		{
		  sx = LOWORD(lParam);
		  sy = HIWORD(lParam);
		  break;
		}

	  case WM_KVL_DRAW_IMAGE:
		{
		  EnterCriticalSection(&cs);
		  int x = LOWORD(lParam);
		  int y = HIWORD(lParam);

		  DrawToVirtualWindow((LPWSTR)wParam, x, y);  // Оновлення буфера
		  LeaveCriticalSection(&cs);
		  InvalidateRect(WHandle, NULL, FALSE);  // Перемальовуємо вікно
		  break;
		}

	  case WM_DESTROY:
		{
		  PostQuitMessage(0);
		  break;
		}

	  case WM_PAINT:
		{
		  PAINTSTRUCT ps;
		  HDC hdc = BeginPaint(WHandle, &ps);
		  EnterCriticalSection(&cs);
		  CopyVirtualToMain(hdc);
		  LeaveCriticalSection(&cs);
		  EndPaint(WHandle, &ps);
		  break;
		}

	  case WM_LBUTTONUP:
		{
		  mouse.x = LOWORD(lParam);
		  mouse.y = HIWORD(lParam);

		  break;
		}

	  case WM_CLOSE:
		{
		  DestroyWindow(hwnd);
		  break;
		}

	  case WM_QUIT: break;

	  default:  /* for messages that we don't deal with */
		return DefWindowProc (hwnd, message, wParam, lParam);
	}

  return 0;
}
//-------------------------------------------------------------------------

unsigned __stdcall CreateHostWindow(void*)
{
  try
	 {
	   MSG Msg;

//Ініціалізація GDI+
	   Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	   ULONG_PTR gdiplusToken;
	   Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	   WNDCLASSEX wcex = {sizeof(WNDCLASSEX),
						  CS_HREDRAW|CS_VREDRAW,
						  HostWindowProcedure,
						  0, 0,
						  GetModuleHandle(NULL),
						  NULL,
						  LoadCursor(NULL, IDC_ARROW),
						  (HBRUSH)(COLOR_WINDOW+1),
						  NULL,
						  L"KobzarVisualisation",
						  NULL};

	   RegisterClassEx(&wcex);

//Створюємо вікно
	  WHandle = CreateWindow(L"KobzarVisualisation",
							 L"Kobzar Visual Library Window", WS_OVERLAPPEDWINDOW,
							 CW_USEDEFAULT, CW_USEDEFAULT,
							 WndWidth, WndHeight,
							 NULL, NULL,
							 GetModuleHandle(NULL),
							 NULL);

		CreateVirtualWindow(WHandle, WndWidth, WndHeight);

        ShowWindow(WHandle, SW_HIDE);
		UpdateWindow(WHandle);

		/* Run the message loop. It will run until GetMessage() returns 0 */
		while (GetMessage (&Msg, NULL, 0, 0))
		  {
			TranslateMessage(&Msg);
      		DispatchMessage(&Msg);
		  }

//Очищення пам’яті
		DeleteObject(hBitmap);
	    DeleteDC(hMemDC);

//Завершення роботи з GDI+
		Gdiplus::GdiplusShutdown(gdiplusToken);

		UnregisterClass(L"KobzarVisualisation", DllHinst);

	   _endthreadex(0);
	 }
  catch (...)
	 {
       _endthreadex(0);
	   throw std::runtime_error("CreateTestWindow: error!");
	 }

  return 0;
}
//---------------------------------------------------------------------------

void StartWork()
{
  try
	 {
	   InitializeCriticalSection(&cs);  //Ініціалізуємо синхронізацію
	   unsigned int thID;
	   WndThread = (HANDLE)_beginthreadex(NULL, 4096, CreateHostWindow, NULL, NULL, &thID);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::StartWork: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void StopWork()
{
  try
	 {
	   WaitForSingleObject(WndThread, -1);
	   DeleteCriticalSection(&cs);
	   CloseHandle(WndThread);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::StopWork: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
extern "C"
{
__declspec(dllexport) void __stdcall eLoadFont(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String file = eIface->GetParamToStr(L"pFile");

	   String res = IntToStr(AddRuntimeFont(file));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eLoadFont: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eRemoveFont(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String file = eIface->GetParamToStr(L"pFile");

	   String res = IntToStr(RemoveRuntimeFont(file));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eRemoveFont: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateForm(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   WndWidth = eIface->GetParamToInt(L"pWidth"),
	   WndHeight = eIface->GetParamToInt(L"pHeight");

	   bool full = eIface->GetParamToInt(L"pFullscreen");

	   if (!WHandle)
		  StartWork();
	   else
		 {
		   StopWork();
		   StartWork();
		 }

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eCreateForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDestroyForm(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   if (!WHandle)
		  throw("Window doesn't exists!");
	   else
		 SendMessage(WHandle, WM_CLOSE, 0, 0);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDestroyForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eOpenForm(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   ShowWindow(WHandle, SW_SHOWNORMAL);

	   String res = IntToStr(reinterpret_cast<int>(WHandle));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eOpenForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCloseForm(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   ShowWindow(WHandle, SW_HIDE);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eCloseForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

_declspec(dllexport) void __stdcall eClearForm(void *p)
{
  try
	 {
       if (!WHandle)
		 throw("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);


	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eClearForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawImage(void *p)
{
  try
	 {
       if (!WHandle)
		 throw("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   int x = eIface->GetParamToInt(L"pX"),
		   y = eIface->GetParamToInt(L"pY");

	   String file = eIface->GetParamToStr(L"pFile");

	   //малюємо картинку у вікні
       SendMessage(WHandle, WM_KVL_DRAW_IMAGE, (WPARAM)file.c_str(), MAKELPARAM(x, y));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawImage: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  DllHinst = hinst;

  //if (reason == DLL_PROCESS_ATTACH)

  if (reason == DLL_PROCESS_DETACH)
	StopWork();

  return 1;
}
//---------------------------------------------------------------------------
