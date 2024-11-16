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
#include "..\Engine\ke_interface.h"

ELI_INTERFACE *eIface;
KE_INTERFACE *kIface;
HINSTANCE FDllHandle;
GETKEINTERFACE FGetKE;
FREEKEINTERFACE FFreeKE;
bool CreatedInstance;

int ConnectEngine()
{
  int res = 1;

  try
	 {
	   FDllHandle = LoadLibrary("KobzarEngine.dll");

	   FGetKE = (GETKEINTERFACE) GetProcAddress(FDllHandle, "GetKEInterface");
	   FFreeKE = (FREEKEINTERFACE) GetProcAddress(FDllHandle, "FreeKEInterface");

	   FGetKE(&kIface);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ConnectEngine: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void FreeEngine()
{
  try
	 {
	   FFreeKE(&kIface);
	   FreeLibrary(FDllHandle);
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "FreeEngine: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------


extern "C"
{
__declspec(dllexport) void __stdcall eGetEngineHandle(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);
	   kIface = reinterpret_cast<KE_INTERFACE*>(eIface->GetParamToInt(L"pHandle"));

	   if (kIface)
		 eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	   else
	     throw Exception("Error getting engine instance");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetEngineHandle: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateEngineHandle(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);	   

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), IntToStr(ConnectEngine().c_str()));
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eCreateEngineHandle: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eFreeEngineHandle(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   FreeEngine();

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eFreeEngineHandle: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eGetLastError(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), kIface->GetLastError());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetLastError: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eCreateStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->CreateStory(ep->GetParamToStr(L"pFile")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eCreateStory: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eLoadStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->LoadStory(ep->GetParamToStr(L"pFile")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eLoadStory: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSaveStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->SaveStory());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSaveStory: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eCloseStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->CloseStory();

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eCloseStory: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eClearStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->ClearStory();

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eClearStory: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eAddScene(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->AddScene());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eAddScene: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eAddAnswer(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);
	   
	   String res = IntToStr(kIface->AddAnswer());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eAddAnswer: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eAddScript(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->AddScript());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eAddScript: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSelect(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Select(ep->GetParamToInt(L"pID")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSelect: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eRemove(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Remove(ep->GetParamToInt(L"pID")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eRemove: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eLink(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Link(ep->GetParamToInt(L"pFrID"), ep->GetParamToInt(L"pToID")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eLink: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eUnlink(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Unlink(ep->GetParamToInt(L"pFrID"), ep->GetParamToInt(L"pToID")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eUnlink: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetID(ep->GetParamToInt(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetID: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetID(ep->GetParamToInt(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetID: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetDialog());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetDialog(ep->GetParamToInt(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetNextDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetNextDialog());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetNextDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetNextDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetNextDialog(ep->GetParamToInt(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetNextDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetPrevID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetPrevID());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetPrevID: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetPrevID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetPrevID(ep->GetParamToInt(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetPrevID: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetNextID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetNextID());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetNextID: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetNextID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetNextID(ep->GetParamToInt(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eNextPrevID: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetType(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetType());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetType: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetText(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = kIface->GetText();

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetText: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetText(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetText(ep->GetParamToStr(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetText: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eIsEndDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->IsEndDialog());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eIsEndDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetEndDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetEndDialog(ep->GetParamToInt(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetEndDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetParams(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), kIface->GetParams());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetParams: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetParams(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetParams(ep->GetParamToStr(L"pValue"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetParams: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eRun(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Run());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eRun: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetResult(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), kIface->GetResult());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetResult: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eTellStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->TellStory(ep->GetParamToStr(L"pFile")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eTellStory: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eLoadDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->LoadDialog(ep->GetParamToInt(L"pID")));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eLoadDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetScene(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), kIface->GetScene());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetScene: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetAnswerCount(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetAnswerCount());

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eGetAnswerCount: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetAnswer(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), kIface->GetAnswer(ep->GetParamToInt(L"pIndex")));
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSetEndDialog: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSelectAnswer(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SelectAnswer(ep->GetParamToInt(L"pIndex"));

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KE_INTERFACE::eSelectAnswer: " + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  if (reason == DLL_PROCESS_ATTACH)
	CreatedInstance = false;
  if (reason == DLL_PROCESS_DETACH) && (!CreatedInstance))
	FreeEngine();
	
  return 1;
}

