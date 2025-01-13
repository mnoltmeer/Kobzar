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

#include <System.hpp>
#pragma hdrstop

#include "..\..\work-functions\Logs.h"
#include "CFThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TAMThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TCFThread::TCFThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------

void __fastcall TCFThread::Execute()
{
  try
	 {
       CreateHostWindow();
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::TCFThread: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void TCFThread::CreateHostWindow()
{
  if (!FHinst)
	return;

  MSG Msg;
  WNDCLASSEX WndClsEx = {0};

  WndClsEx.cbSize        = sizeof (WNDCLASSEX);
  WndClsEx.style         = CS_HREDRAW | CS_VREDRAW;
  WndClsEx.lpfnWndProc   = HostWindowProcedure;
  WndClsEx.hInstance     = FHinst;
  WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  WndClsEx.lpszClassName = L"ClassName";
  WndClsEx.hIconSm       = LoadIcon(FHinst, IDI_APPLICATION);

  RegisterClassEx(&WndClsEx);

  FHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
						   L"ClassName",
						   L"Window Title",
						   WS_OVERLAPPEDWINDOW,
						   100,
						   120,
						   800,
						   600,
						   NULL,
						   NULL,
						   FHinst,
                           NULL);

  ShowWindow(FHandle, SW_HIDE);
  UpdateWindow(FHandle);

  /* Run the message loop. It will run until GetMessage() returns 0 */
  while (GetMessage (&Msg, NULL, 0, 0))
    {
	  TranslateMessage(&Msg);
      DispatchMessage(&Msg);
	}
}
//---------------------------------------------------------------------------

void __fastcall TCFThread::OnTerminate(TObject *Sender)
{
  SendMessage(FHandle, WM_CLOSE, 0, 0);
}
//---------------------------------------------------------------------------

LRESULT CALLBACK HostWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
			{
			  PostQuitMessage(0);
			  break;
			}

        case WM_CLOSE:
			{
			  DestroyWindow(hwnd);
			  break;
			}

		case WM_QUIT: break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

  return 0;
}
//-------------------------------------------------------------------------


