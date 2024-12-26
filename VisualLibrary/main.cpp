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

#pragma hdrstop
#pragma argsused

#include "eli_interface.h"
#include "..\..\work-functions\Logs.h"

ELI_INTERFACE *eIface;
HWND HostWindowHandle;
TForm *WorkWindow;

void CreateHostWindow()
{

}
//---------------------------------------------------------------------------

TForm *CreateWorkWindow()
{
  TForm *res = new TForm(HostWindowHandle);

  return res;
}
//---------------------------------------------------------------------------

extern "C"
{
__declspec(dllexport) void __stdcall eCreateForm(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   WorkWindow = CreateWorkWindow();

	   if (!WorkWindow)
		 throw("Error creating window");

	   String res = IntToStr(reinterpret_cast<int>(WorkWindow));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eCreateForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eDeleteForm(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   if (!WorkWindow)
		 throw("Window doesn't exist");
	   else
		 delete WorkWindow;

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "VisualLibrary::eDeleteForm: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  if (reason == DLL_PROCESS_ATTACH)
	CreateHostWindow();
  else if (reason == DLL_PROCESS_DETACH)
	{
	  if (WorkWindow)
		delete WorkWindow;

	  if (HostWindowHandle)
		DestroyWindow(HostWindowHandle);
    }

  return 1;
}

