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
#include "..\..\work-functions\Logs.h"

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
	   FDllHandle = LoadLibrary(L"KobzarEngine.dll");

	   FGetKE = (GETKEINTERFACE) GetProcAddress(FDllHandle, "GetKEInterface");
	   FFreeKE = (FREEKEINTERFACE) GetProcAddress(FDllHandle, "FreeKEInterface");

	   FGetKE(&kIface);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::ConnectEngine: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetEngineHandle: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

__declspec(dllexport) void __stdcall eCreateEngineHandle(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);	   

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), IntToStr(ConnectEngine()).c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eCreateEngineHandle: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eFreeEngineHandle: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetLastError: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eCreateStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->CreateStory(eIface->GetParamToStr(L"pFile")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eCreateStory: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eLoadStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->LoadStory(eIface->GetParamToStr(L"pFile")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eLoadStory: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSaveStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->SaveStory());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSaveStory: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eCloseStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->CloseStory();

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eCloseStory: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eClearStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->ClearStory();

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eClearStory: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eAddScene(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->AddScene());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eAddScene: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eAddAnswer(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);
	   
	   String res = IntToStr(kIface->AddAnswer());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eAddAnswer: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eAddScript(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->AddScript());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eAddScript: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSelect(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Select(eIface->GetParamToInt(L"pID")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSelect: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eRemove(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Remove(eIface->GetParamToInt(L"pID")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eRemove: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eLink(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Link(eIface->GetParamToInt(L"pFrID"), eIface->GetParamToInt(L"pToID")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eLink: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eUnlink(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Unlink(eIface->GetParamToInt(L"pFrID"), eIface->GetParamToInt(L"pToID")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eUnlink: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetID());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetID: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetID(eIface->GetParamToInt(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetID: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetDialog());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetDialog(eIface->GetParamToInt(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetNextDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetNextDialog());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetNextDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetNextDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetNextDialog(eIface->GetParamToInt(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetNextDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetPrevID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetPrevID());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetPrevID: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetPrevID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetPrevID(eIface->GetParamToInt(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetPrevID: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetNextID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetNextID());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetNextID: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetNextID(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetNextID(eIface->GetParamToInt(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eNextPrevID: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetType(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetType());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetType: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetText(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = kIface->GetText();

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetText: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetText(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetText(eIface->GetParamToStr(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetText: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eIsEndDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->IsEndDialog());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eIsEndDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetEndDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetEndDialog(eIface->GetParamToInt(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetEndDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetParams(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), kIface->GetParams());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetParams: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSetParams(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SetParams(eIface->GetParamToStr(L"pValue"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetParams: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eExecute(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->Execute());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eExecute: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetResult(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), kIface->GetResult());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetResult: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eTellStory(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->TellStory(eIface->GetParamToStr(L"pFile")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eTellStory: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eLoadDialog(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->LoadDialog(eIface->GetParamToInt(L"pID")));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eLoadDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetScene(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), kIface->GetScene());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetScene: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetAnswerCount(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   String res = IntToStr(kIface->GetAnswerCount());

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), res.c_str());
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eGetAnswerCount: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eGetAnswer(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), kIface->GetAnswer(eIface->GetParamToInt(L"pIndex")));
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSetEndDialog: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------

void __stdcall eSelectAnswer(void *p)
{
  try
	 {
	   eIface = static_cast<ELI_INTERFACE*>(p);

	   kIface->SelectAnswer(eIface->GetParamToInt(L"pIndex"));

	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "ELIConnector::eSelectAnswer: " + e.ToString());
	   eIface->SetFunctionResult(eIface->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------
}

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  if (reason == DLL_PROCESS_ATTACH)
	CreatedInstance = false;
  if ((reason == DLL_PROCESS_DETACH) && (!CreatedInstance))
	FreeEngine();
	
  return 1;
}

