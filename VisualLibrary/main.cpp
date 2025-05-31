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

//Власне повідомлення для роботи з вікном
#define WM_KVL_CLEAR_WINDOW (WM_USER + 1)
#define WM_KVL_DRAW_IMAGE (WM_USER + 2)
#define WM_KVL_DRAW_FRAME (WM_USER + 3)
#define WM_KVL_DRAW_TEXT (WM_USER + 4)

struct COLOR
{
  int Red = 0;
  int Green = 0;
  int Blue = 0;
  int Alpha = 0;
};

struct FRAME
{
  RECT Rect;
  COLOR Color;
  int Border = 0;
  COLOR BorderColor;
  int CornerRadius = 0;
};

struct TEXT
{
  RECT Rect;
  wchar_t FontName[32];
  int FontSize = 10;
  int FontStyle = 0;
  COLOR FontColor;
  wchar_t Alignment[8];
  int CenterVerticaly = 0;
  int WordWrap = 0;
};

HINSTANCE DllHinst;
HANDLE WndThread;
HWND WHandle;
unsigned int thID;

//Віртуальне вікно (буфер)
HBITMAP hBitmap = NULL;
HDC hMemDC = NULL;

int WndWidth = 800, WndHeight = 600;
bool FullScreen = 0;
CRITICAL_SECTION cs;  //Для безпечного доступу до пам’яті
FRAME CurrentFrame; //для визначення параметрів поточного об'єкту типу Frame
TEXT CurrentText; //для визначення параметрів поточного об'єкту типу Text

//Функція створення віртуального вікна (буфера)
void CreateVirtualWindow(HWND hWnd, int width, int height)
{
  try
	 {
	   HDC hdc = GetDC(hWnd);
	   hMemDC = CreateCompatibleDC(hdc);
	   hBitmap = CreateCompatibleBitmap(hdc, width, height);
	   SelectObject(hMemDC, hBitmap);
	   ReleaseDC(hWnd, hdc);
       SetBkMode(hMemDC, TRANSPARENT); //встановлюємо прозорий фон для тексту
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//Функція малювання у віртуальному вікні
void DrawToVirtualWindow(const wchar_t* file, int x, int y)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

//Завантаження зображення через GDI+
	   Gdiplus::Graphics graphics(hMemDC);
	   Gdiplus::Image image(file);

	   if (image.GetLastStatus() == Gdiplus::Ok)
		 graphics.DrawImage(&image, x, y);
	   else
		 throw Exception("GDI+ not initialised!");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawToVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DrawTextGDIPlus(const std::wstring& text, const RECT& rect,
					 const std::wstring& font_name = L"Segoe UI",
					 float font_size = 20.0f,
					 Gdiplus::FontStyle font_style = Gdiplus::FontStyleRegular,
					 Gdiplus::Color color = Gdiplus::Color(255, 0, 0, 0), // чорний
                     const std::wstring& align = L"left", // "left", "center", "right"
					 bool center_vertically = false,
                     bool word_wrap = true)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	   Gdiplus::FontFamily font_family(font_name.c_str());
	   Gdiplus::Font font(&font_family, font_size, font_style, Gdiplus::UnitPixel);
	   Gdiplus::SolidBrush brush(color);

	   Gdiplus::RectF layout_rect((float)rect.left, (float)rect.top,
								 (float)(rect.right - rect.left),
								 (float)(rect.bottom - rect.top));

	   Gdiplus::StringFormat format;

//Горизонтальне вирівнювання
	   if (align == L"center")
		 format.SetAlignment(Gdiplus::StringAlignmentCenter);
	   else if (align == L"right")
		 format.SetAlignment(Gdiplus::StringAlignmentFar);
	   else if (align == L"justify")
		 {
		   format.SetAlignment(Gdiplus::StringAlignmentNear);
		   format.SetFormatFlags(format.GetFormatFlags() |
								 Gdiplus::StringFormatFlagsLineLimit |
								 Gdiplus::StringFormatFlagsNoFitBlackBox);
		   format.SetTrimming(Gdiplus::StringTrimmingNone);
		 }
	   else
		 format.SetAlignment(Gdiplus::StringAlignmentNear);//left

//Вертикальне вирівнювання
	   format.SetLineAlignment(center_vertically ? Gdiplus::StringAlignmentCenter : Gdiplus::StringAlignmentNear);

//Перенос рядків
	   if (!word_wrap)
		 format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

	   graphics.DrawString(text.c_str(), -1, &font, layout_rect, &format, &brush);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawTextGDIPlus: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//Функція копіювання вмісту віртуального вікна в основне
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

//очистка віртуального буферу
void ClearVirtualWindow()
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   RECT rect = {0, 0, WndWidth, WndHeight};
	   FillRect(hMemDC, &rect, (HBRUSH)(COLOR_WINDOW+1));
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::ClearVirtualWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

//Допоміжна функція для побудови прямокутника із закругленими кутами
Gdiplus::GraphicsPath* CreateRoundedRectPath(Gdiplus::Rect rect, int radius)
{
  Gdiplus::GraphicsPath* path = new Gdiplus::GraphicsPath();

  try
	 {
	   int x = rect.X, y = rect.Y, w = rect.Width, h = rect.Height;
	   int r = radius * 2;

	   path->AddArc(x, y, r, r, 180, 90);                 // верхній лівий кут
	   path->AddArc(x + w - r, y, r, r, 270, 90);         // верхній правий кут
	   path->AddArc(x + w - r, y + h - r, r, r, 0,   90); // нижній правий кут
	   path->AddArc(x, y + h - r, r, r, 90, 90);          // нижній лівий кут
	   path->CloseFigure();
	 }
  catch (Exception &e)
	 {
	   if (path)
		 delete path;

	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateRoundedRectPath: " + e.ToString());
	 }

  return path;
}
//---------------------------------------------------------------------------

void DrawRectangleGDIPlus(int x, int y, int width, int height,
						  Gdiplus::Color fillColor = Gdiplus::Color(255, 255, 255, 255),
                          Gdiplus::Color borderColor = Gdiplus::Color(255, 0, 0, 0),
                          float borderWidth = 1.0f,
                          int cornerRadius = 0)
{
  try
	 {
	   if (!hMemDC)
		 throw Exception("Virtual buffer doesn't exists!");

	   Gdiplus::Graphics graphics(hMemDC);
	   graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	   Gdiplus::Rect rect(x, y, width, height);
	   Gdiplus::SolidBrush brush(fillColor);
	   Gdiplus::Pen pen(borderColor, borderWidth);

	   if (cornerRadius > 0)
		 {
		   Gdiplus::GraphicsPath* path = CreateRoundedRectPath(rect, cornerRadius);

		   graphics.FillPath(&brush, path);
		   graphics.DrawPath(&pen, path);

		   delete path;
		 }
	   else
		 {
		   graphics.FillRectangle(&brush, rect);
		   graphics.DrawRectangle(&pen, rect);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DrawRectangleGdiPlus: " + e.ToString());
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

	  case WM_KVL_CLEAR_WINDOW:
		{
		  EnterCriticalSection(&cs);

		  ClearVirtualWindow();

		  LeaveCriticalSection(&cs);

          InvalidateRect(WHandle, NULL, FALSE);  //Перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_IMAGE:
		{
		  EnterCriticalSection(&cs);

		  int x = LOWORD(lParam);
		  int y = HIWORD(lParam);

		  DrawToVirtualWindow((LPWSTR)wParam, x, y);  //Оновлення буфера

		  LeaveCriticalSection(&cs);

		  InvalidateRect(WHandle, NULL, FALSE);  //Перемальовуємо вікно
		  break;
		}

      case WM_KVL_DRAW_FRAME:
		{
		  EnterCriticalSection(&cs);

		  DrawRectangleGDIPlus(CurrentFrame.Rect.left,
							   CurrentFrame.Rect.top,
							   CurrentFrame.Rect.right,
							   CurrentFrame.Rect.bottom,
							   Gdiplus::Color(CurrentFrame.Color.Alpha,
											  CurrentFrame.Color.Red,
											  CurrentFrame.Color.Green,
											  CurrentFrame.Color.Blue),
							   Gdiplus::Color(CurrentFrame.BorderColor.Alpha,
											  CurrentFrame.BorderColor.Red,
											  CurrentFrame.BorderColor.Green,
											  CurrentFrame.BorderColor.Blue),
							   CurrentFrame.Border,
							   CurrentFrame.CornerRadius);

		  LeaveCriticalSection(&cs);
		  InvalidateRect(WHandle, NULL, FALSE);  // Перемальовуємо вікно
		  break;
		}

	  case WM_KVL_DRAW_TEXT:
		{
		  EnterCriticalSection(&cs);

		  DrawTextGDIPlus((LPWSTR)wParam,
						  CurrentText.Rect,
						  CurrentText.FontName,
						  CurrentText.FontSize,
						  (Gdiplus::FontStyle)CurrentText.FontStyle,
						  Gdiplus::Color(CurrentText.FontColor.Alpha,
										 CurrentText.FontColor.Red,
										 CurrentText.FontColor.Green,
										 CurrentText.FontColor.Blue),
						  CurrentText.Alignment,
						  CurrentText.CenterVerticaly,
						  CurrentText.WordWrap);

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

		ShowWindow(WHandle, SW_SHOWNORMAL);
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
		WHandle = NULL;
	   _endthreadex(0);
	 }
  catch (...)
	 {
	   _endthreadex(0);
	   throw std::runtime_error("CreateHostWindow: error!");
	 }

  return 0;
}
//---------------------------------------------------------------------------

void StartWork()
{
  try
	 {
	   InitializeCriticalSection(&cs);  //Ініціалізуємо синхронізацію
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
	   if (WHandle)
		 PostMessage(WHandle, WM_CLOSE, 0, 0);

	   WaitForSingleObject(WndThread, 500);
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

	   if (!FileExists(file))
		 throw Exception("Error loading file: " + file);

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

       if (!FileExists(file))
		 throw Exception("Error loading file: " + file);

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

       Sleep(500);

	   PostMessage(WHandle, WM_KVL_CLEAR_WINDOW, 0, 0);

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
		  throw Exception("Window doesn't exists!");
	   else
		 PostMessage(WHandle, WM_CLOSE, 0, 0);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDestroyForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

_declspec(dllexport) void __stdcall eClearForm(void *p)
{
  try
	 {
       if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   PostMessage(WHandle, WM_KVL_CLEAR_WINDOW, 0, 0);

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
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   int x = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left")),
		   y = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));

	   String file = eIface->GetObjectProperty(obj.c_str(), L"Source");

	   file = ParseString(file, ".\\", String(eIface->GetInitDir()) + "\\");

	   if (!FileExists(file))
		 throw Exception("Error loading image file: " + file);

	   PostMessage(WHandle, WM_KVL_DRAW_IMAGE, (WPARAM)file.c_str(), MAKELPARAM(x, y));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawImage: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawFrame(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw Exception("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_color = eIface->GetObjectProperty(obj.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   String obj_bord = eIface->GetObjectProperty(obj.c_str(), L"Border");

	   if (obj_color == "")
		 throw Exception("Can't get Border object name!");

	   CurrentFrame.Rect.left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentFrame.Rect.top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentFrame.Rect.right = CurrentFrame.Rect.left +
								_wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentFrame.Rect.bottom = CurrentFrame.Rect.top +
								 _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));

	   CurrentFrame.CornerRadius = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Corner"));

	   CurrentFrame.Color.Red = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red"));
	   CurrentFrame.Color.Green = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green"));
	   CurrentFrame.Color.Blue = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue"));
	   CurrentFrame.Color.Alpha = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha"));

       CurrentFrame.Border = _wtoi(eIface->GetObjectProperty(obj_bord.c_str(), L"Size"));

	   obj_color = eIface->GetObjectProperty(obj_bord.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Border Color object name!");

	   CurrentFrame.BorderColor.Red = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red"));
	   CurrentFrame.BorderColor.Green = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green"));
	   CurrentFrame.BorderColor.Blue = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue"));
	   CurrentFrame.BorderColor.Alpha = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha"));

	   PostMessage(WHandle, WM_KVL_DRAW_FRAME, NULL, NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawImage: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDrawText(void *p)
{
  try
	 {
	   if (!WHandle)
		 throw("Window doesn't exists!");

	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String obj = eIface->GetParamToStr(L"pObjectName");

	   if (obj == "")
		 throw Exception("Can't get main object name!");

	   obj = "&" + obj;

	   String obj_font = eIface->GetObjectProperty(obj.c_str(), L"Font");

	   if (obj_font == "")
		 throw Exception("Can't get Font object name!");

	   String obj_color = eIface->GetObjectProperty(obj_font.c_str(), L"Color");

	   if (obj_color == "")
		 throw Exception("Can't get Color object name!");

	   CurrentText.Rect.left = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Left"));
	   CurrentText.Rect.top = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Top"));
	   CurrentText.Rect.right = CurrentText.Rect.left +
								_wtoi(eIface->GetObjectProperty(obj.c_str(), L"Width"));
	   CurrentText.Rect.bottom = CurrentText.Rect.top +
								 _wtoi(eIface->GetObjectProperty(obj.c_str(), L"Height"));
	   wcscpy(CurrentText.Alignment, eIface->GetObjectProperty(obj.c_str(), L"Align"));
	   CurrentText.CenterVerticaly = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"CenterVerticaly"));
	   CurrentText.WordWrap = _wtoi(eIface->GetObjectProperty(obj.c_str(), L"WordWrap"));
	   String text = eIface->GetObjectProperty(obj.c_str(), L"Data");

	   wcscpy(CurrentText.FontName, eIface->GetObjectProperty(obj_font.c_str(), L"Name"));
	   CurrentText.FontSize = _wtoi(eIface->GetObjectProperty(obj_font.c_str(), L"Size"));

	   String style = eIface->GetObjectProperty(obj_font.c_str(), L"Style");

	   if (style == "b")
		 CurrentText.FontStyle = 1;
	   else if (style == "i")
		 CurrentText.FontStyle = 2;
	   else if (style == "bi")
		 CurrentText.FontStyle = 3;
	   else if (style == "u")
		 CurrentText.FontStyle = 4;
	   else if (style == "s")
		 CurrentText.FontStyle = 8;
	   else
		 CurrentText.FontStyle = 0;

	   CurrentText.FontColor.Red = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Red"));
	   CurrentText.FontColor.Green = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Green"));
	   CurrentText.FontColor.Blue = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Blue"));
	   CurrentText.FontColor.Alpha = _wtoi(eIface->GetObjectProperty(obj_color.c_str(), L"Alpha"));

	   PostMessage(WHandle, WM_KVL_DRAW_TEXT, (WPARAM)text.c_str(), NULL);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDrawText: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  if (reason == DLL_PROCESS_ATTACH)
	DllHinst = hinst;
  else if (reason == DLL_PROCESS_DETACH)
	StopWork();

  return 1;
}
//---------------------------------------------------------------------------
