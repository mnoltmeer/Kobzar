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

#pragma hdrstop
#pragma argsused

#include "eli_interface.h"
#include "..\..\work-functions\Logs.h"
#include "CFThread.h"

ELI_INTERFACE *eIface;
TForm *WorkWindow;
TCFThread *Thread;

void CreateWorkWindow(HINSTANCE hinst)
{
  try
	 {
	   Thread = new TCFThread(true);

	   Thread->FreeOnTerminate = true;
	   Thread->Instance = hinst;
	   Thread->Resume();
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::CreateWorkWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void DestroyWorkWindow()
{
  try
	 {
	   if (Thread->Started)
		 Thread->Terminate();
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::DestroyWorkWindow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

extern "C"
{
__declspec(dllexport) void __stdcall eOpenForm(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   if (!Thread->Started || !Thread->WindowHandle)
		 throw("Window doesn't exists!");

	   ShowWindow(Thread->WindowHandle, SW_SHOWNORMAL);

	   String res = IntToStr(reinterpret_cast<int>(Thread->WindowHandle));

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
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   if (!Thread->Started || !Thread->WindowHandle)
		 throw("Window doesn't exists!");

	   ShowWindow(Thread->WindowHandle, SW_HIDE);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eCloseForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  if (reason == DLL_PROCESS_ATTACH)
	CreateWorkWindow(hinst);

  if (reason == DLL_PROCESS_DETACH)
	DestroyWorkWindow();

  return 1;
}

