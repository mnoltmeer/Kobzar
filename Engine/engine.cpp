/*!
Copyright 2023-2024 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#pragma hdrstop
#include <stdio.h>
#include <Vcl.Forms.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.Win.msxmldom.hpp>

#include "..\..\work-functions\Logs.h"
#include "..\..\work-functions\Data.h"
#include "..\..\work-functions\FilesDirs.h"
#include "engine.h"
//---------------------------------------------------------------------------

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  LogPath = GetEnvironmentVariable("USERPROFILE") + "\\Documents\\Kobzar";

  if (!DirectoryExists(LogPath))
	CreateDir(LogPath);

  GetModuleFileName(hinst, path, sizeof(path));

  return 1;
}
//--------------------------------------------------------------------------

KE_API int __stdcall GetKEInterface(KE_INTERFACE **eInterface)
{
  if (*eInterface)
	return 0;

  int res = 1;

  try
	 {
	   *eInterface = new KobzarEngine();
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "GetKEInterface:" + e.ToString());
	 }

  return res;
}
//-------------------------------------------------------------------------

KE_API int __stdcall FreeKEInterface(KE_INTERFACE **eInterface)
{
  if (*eInterface)
	{
	  delete *eInterface;
	  *eInterface = NULL;

	  return 1;
	}

  return 0;
}
//-------------------------------------------------------------------------

KobzarEngine::KobzarEngine()
{
  ActiveItem = nullptr;
  FCurrentFile = "";
  FLastError = "no error";
};
//-------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetVersion()
{
  return GetVersionInString(path).c_str();
}
//-------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetLastError()
{
  return FLastError.c_str();
}
//-------------------------------------------------------------------------

int __stdcall KobzarEngine::CreateStory(const wchar_t *story_file)
{
  int res = 0;

  try
	 {
	   String file = ParseString(String(story_file), ".\\", GetDirPathFromFilePath(String(path)) + "\\");
	   std::unique_ptr<TStringStream> ms(new TStringStream("", TEncoding::UTF8, true));

       ms->Position = 0;
	   ms->SaveToFile(file);

	   FCurrentFile = String(file);

       res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::CreateStory", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::LoadStory(const wchar_t *story_file)
{
  int res = 0;

  try
	 {
	   String file = ParseString(String(story_file), ".\\", GetDirPathFromFilePath(String(path)) + "\\");

	   if (!FileExists(file))
		 throw Exception("Story file " + file + " not found!");

	   if (UpperCase(GetFileExtensionFromFileName(file)) == "SCS")
		 LoadDlgSchema(file);
	   else
		 {
		   try
			  {
				CoInitializeEx(NULL, 0);
				XMLImport(file);
			  }
		   __finally {CoUninitialize();}
		 }

	   FCurrentFile = String(file);

	   res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::LoadStory", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SaveStory()
{
  int res = 0;

  try
	 {
	   if (UpperCase(GetFileExtensionFromFileName(FCurrentFile)) == "SCS")
		 res = SaveDlgSchema(FCurrentFile);
	   else
		 res = XMLExport(FCurrentFile);
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::SaveStory", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::CloseStory()
{
  int res = 0;

  try
	 {
	   ClearStory();
	   FCurrentFile = "";

	   res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::CloseStory", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::ClearStory()
{
  int res = 0;

  try
	 {
	   ClearItems();
	   ActiveItem = nullptr;

	   res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::ClearStory", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::AddScene()
{
  int res = 0;

  try
	 {
	   TDlgScreenText *tmp = new TDlgScreenText(GenElementID(), GenDialogID());
	   FItems.push_back(tmp);

	   res = tmp->ID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::AddScene", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::AddAnswer()
{
  int res = 0;

  try
	 {
	   TDlgAnswer *tmp = new TDlgAnswer(GenElementID());
	   FItems.push_back(tmp);

	   res = tmp->ID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::AddAnswer", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::AddScript()
{
  int res = 0;

  try
	 {
	   TDlgScript *tmp = new TDlgScript(GenElementID());
	   FItems.push_back(tmp);

       res = tmp->ID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::AddScript", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::Select(int id)
{
  int res = 0;

  try
	 {
	   ActiveItem = FindElement(id);

       if (!ActiveItem)
		 throw Exception("No active element!");

	   res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::Select", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t *__stdcall KobzarEngine::RunScript(const wchar_t *text)
{
  const wchar_t *res = nullptr;

  try
	 {
       std::unique_ptr<ELIScript> script(new ELIScript(GetDirPathFromFilePath(String(path)) + "\\ELI.dll"));

	   if (!script->Initialised)
		 throw Exception("Script object not initialised!");

	   wchar_t buffer[256];
	   swprintf(buffer, L"%d\r\n", reinterpret_cast<int>(this));

	   script->Interpreter->SetParam(L"pHandle", buffer);

	   script->Text = text;

	   if (!script->Run())
		 throw Exception(script->Log);
	   else
		 res = script->Result.c_str();
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::RunScript", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::Remove(int id)
{
  int res = 0;

  try
	 {
	   TDlgBaseText *itm = FindElement(id);

	   if (!itm)
		 throw Exception("Element with ID = " + IntToStr(id) + " not found");

       RemoveFromItems(itm);

	   res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::Remove", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::Link(int id, int to_id)
{
  int res = 0;

  try
	 {
	   TDlgBaseText *from = FindElement(id);
	   TDlgBaseText *to = FindElement(to_id);

	   if (!from)
		 throw Exception("Element with ID = " + IntToStr(id) + " not found");

	   if (!to)
		 throw Exception("Element with ID = " + IntToStr(to_id) + " not found");

	   if ((from->Type == DlgAnsw) &&
		  (to->Type == DlgText)) //перший елемент - Відповідь, другий - Сцена
		 {
		   if (to->PrevID > 0) //якщо Сцена вже привязана до іншої Відповіді
			 {
			   TDlgBaseText *old_answ = FindElement(to->PrevID);

			   if (old_answ) //видалимо прив'язки у старої Відповіді
				 {
				   old_answ->NextDialog = 0;
				   old_answ->NextID = 0;
				 }
			 }

		  from->NextDialog = to->Dialog;
		  from->NextID = to->ID;
		 }
	   else if ((from->Type == DlgText) &&
			   ((to->Type == DlgAnsw) || (to->Type == DlgScript))) //перший елемент - Сцена, другий - Відповідь
		 {
		   to->Dialog = from->Dialog;
		   to->PrevID = from->ID;
		 }

	   res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::Link", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::Unlink(int id, int to_id)
{
  int res = 0;

  try
	 {
	   TDlgBaseText *from = FindElement(id);
	   TDlgBaseText *to = FindElement(to_id);

	   if (!from)
		 throw Exception("Element with ID = " + IntToStr(id) + " not found");

	   if (!to)
		 throw Exception("Element with ID = " + IntToStr(to_id) + " not found");

	   if (from->Type == DlgAnsw)
		 from->NextDialog = 0;

	   if ((to->Type == DlgAnsw) || (to->Type == DlgScript))
		 to->Dialog = 0;

	   from->NextID = 0;
	   to->PrevID = 0;

	   res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("Story::Unlink", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::GetID()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       res = ActiveItem->ID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetID", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetID(int val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   UpdatePrevID(ActiveItem->ID, val);
       ActiveItem->ID = val;

       res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetID", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::GetPrevID()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   res = ActiveItem->PrevID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetPrevID", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetPrevID(int val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type == DlgText)
		 CreateLog("KobzarEngine::SetPrevID", "Can't change PrevID property of TEXT element");
	   else
		 {
		   TDlgBaseText *lnk = FindElement(val);

		   if (lnk && (lnk->Type == DlgText))
			 {
			   ActiveItem->PrevID = val;
			   ActiveItem->Dialog = lnk->Dialog;
			 }
		   else if (lnk && (lnk->Type != DlgText))
			 throw Exception("Element with ID = PrevID (" + IntToStr(val) + ") is not TEXT element");
		   else
			 throw Exception("No TEXT_LIKE element with ID = PrevID (" + IntToStr(val) + ")");
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetPrevID", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::GetNextID()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       res = ActiveItem->NextID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetNextID", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetNextID(int val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       if (ActiveItem->Type == DlgText)
         throw Exception("Can't change NextID property of TEXT element");

       TDlgBaseText *lnk = FindElement(val);

       if (lnk && (lnk->Type == DlgText))
         {
           ActiveItem->NextID = val;
           ActiveItem->NextDialog = lnk->Dialog;
         }
       else if (lnk && (lnk->Type != DlgText))
         throw Exception("Element with ID = NextID (" + IntToStr(val) + ") is not a TEXT");
       else
         throw Exception("No TEXT element with ID = NextID (" + IntToStr(val) + ")");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetNextID", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::GetDialog()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       res = ActiveItem->Dialog;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetDialog", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetDialog(int val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       int old = ActiveItem->Dialog;

       if (ActiveItem->Type != DlgText)
         {
           ActiveItem->Dialog = val;
           ActiveItem->PrevID = FindTextElementID(val);
         }
       else
         {
           ActiveItem->Dialog = val;
           std::vector<TDlgAnswer*> links;

           if (FindAnswersByDialog(ActiveItem->Dialog, &links) > 0)
             {
               CreateLog("KobzarEngine::SetDialog", "Founded elements with Dialog. Creating links");

               for (auto itm : links)
                  itm->PrevID = ActiveItem->ID;
             }

           if ((old != ActiveItem->Dialog) && (SearchDependeciesDialog(old) > 0))
             {
               CreateLog("KobzarEngine::SetDialog", "Founded links of old elements. Rebuilding links");
               UpdateDialog(old, ActiveItem->Dialog);
             }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetDialog", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::GetNextDialog()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       res = ActiveItem->NextDialog;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetNextDialog", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetNextDialog(int val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type != DlgText)
         {
           ActiveItem->NextDialog = val;
           int new_dlg_id = FindTextElementID(ActiveItem->NextDialog);

           if (new_dlg_id > 0)
             {
               TDlgBaseText *ndlg = FindElement(new_dlg_id);
               ndlg->PrevID = ActiveItem->ID;
               ActiveItem->NextID = ndlg->ID;

               res = 1;
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetNextDialog", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::GetType()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       res = ActiveItem->Type;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetType", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetText()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       res = ActiveItem->Text.c_str();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetText", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetText(const wchar_t *val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

       ActiveItem->Text = String(val);

       res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetText", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::IsEndDialog()
{
  int res = -1;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type == DlgAnsw)
		 res = reinterpret_cast<TDlgAnswer*>(ActiveItem)->EndDialog;
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not an Answer");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::IsEndDialog", e.ToString());

       res = -1;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetEndDialog(bool val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type == DlgAnsw)
		 {
		   TDlgAnswer *itm = reinterpret_cast<TDlgAnswer*>(ActiveItem);

		   itm->EndDialog = val;

		   if (val)
			 {
			   itm->PrevNextID = itm->NextID;
			   itm->NextID = 0;
			   itm->NextDialog = 0;
			 }
		   else if (itm->PrevNextID > 0)
			 {
			   SetNextID(itm->PrevNextID);
			   itm->PrevNextID = 0;
			 }
		 }
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not an Answer");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetEndDialog", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetParams()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type == DlgScript)
		 res = reinterpret_cast<TDlgScript*>(ActiveItem)->Params.c_str();
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not a Script");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetParams", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SetParams(const wchar_t *val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type == DlgScript)
		 reinterpret_cast<TDlgScript*>(ActiveItem)->Params = String(val);
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not a Script");

       res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetParams", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::Execute()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type != DlgScript)
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not a Script");
	   else
		 res = TranslateScript(reinterpret_cast<TDlgScript*>(ActiveItem));
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::Execute", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetResult()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type == DlgScript)
		 res = reinterpret_cast<TDlgScript*>(ActiveItem)->Result.c_str();
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not a Script");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetResult", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::TellStory(const wchar_t *story_file)
{
  int res = 0;

  try
	 {
	   if (!LoadStory(story_file))
		 throw Exception("Can't load story");

	   res = LoadDialog(1);
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::TellStory", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::LoadDialog(int dlg_id)
{
  int res = 0;

  try
	 {
	   if (Select(FindTextElementID(dlg_id)))
		 {
		   std::vector<TDlgScript*> scripts;

		   FindScriptsByDialog(dlg_id, &scripts);

		   for (auto itm : scripts)
			  TranslateScript(itm);

		   res = 1;
		 }
	   else
         throw Exception("No Scene in dialog with ID = " + IntToStr(dlg_id));
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::LoadDialog", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetScene()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type != DlgText)
		 throw Exception("Active element isn't a Scene");

	   res = ActiveItem->Text.c_str();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetScene", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::GetAnswerCount()
{
  int res = -1;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   if (ActiveItem->Type != DlgText)
		 throw Exception("Active element isn't a Scene");

	   std::vector<TDlgAnswer*> answers;

       res = FindAnswersByDialog(ActiveItem->Dialog, &answers);
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetAnswerCount", e.ToString());

       res = -1;
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetAnswer(int index)
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");

	   std::vector<TDlgAnswer*> answers;

       FindAnswersByDialog(ActiveItem->Dialog, &answers);

       res = answers[index]->Text.c_str();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetAnswer", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int __stdcall KobzarEngine::SelectAnswer(int index)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active Scene!");

       if (ActiveItem->Type != DlgText)
		 throw Exception("Active element isn't a Scene");

       std::vector<TDlgAnswer*> answers;

       FindAnswersByDialog(ActiveItem->Dialog, &answers);

       TDlgAnswer *itm = answers[index];

       if (itm->NextDialog > 0)
         LoadDialog(itm->NextDialog);
       else
         CloseStory();

       res = 1;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SelectAnswer", e.ToString());

       res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------
//------------------INTERNAL FUNCTIONS-------------------------------------
//-------------------------------------------------------------------------
void KobzarEngine::CreateLog(const String &method_name, const String &error)
{
  FLastError = error;
  SaveLogToUserFolder("Engine.log", "Kobzar", method_name + ": " + error);
}
//---------------------------------------------------------------------------

int KobzarEngine::GenDialogID()
{
  int max = 0;

  try
	 {
	   for (auto itm : FItems)
		 {
		   if (itm->Dialog > max)
			 max = itm->Dialog;
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GenDialogID", e.ToString());
	   max = -1;
	 }

  return max + 1;
}
//---------------------------------------------------------------------------

int KobzarEngine::GenElementID()
{
  int max = 0;

  try
	 {
	   for (auto itm : FItems)
		 {
		   if (itm->ID > max)
			 max = itm->ID;
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GenElementID", e.ToString());
	   max = -1;
	 }

  return max + 1;
}
//---------------------------------------------------------------------------

TDlgBaseText *KobzarEngine::FindElement(int id)
{
  TDlgBaseText *res = nullptr;

  try
	 {
	   if (id == 0)
	     throw Exception("Element ID can't be 0");

	   for (int i = 0; i < FItems.size(); i++)
		 {
		   if (FItems[i]->ID == id)
			 res = FItems[i];
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::FindElement", e.ToString());
	   res = nullptr;
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::FindLinkedElements(int id, std::vector<TDlgBaseText*> *el_list)
{
  int cnt = 0;

  if (id == 0)
	return 0;

  try
	 {
	   for (int i = 0; i < FItems.size(); i++)
		 {
		   if (FItems[i]->PrevID == id)
			 {
			   cnt++;
			   el_list->push_back(FItems[i]);
			 }
         }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::FindLinkedElements", e.ToString());
	   cnt = -1;
	 }

  return cnt;
}
//---------------------------------------------------------------------------

int KobzarEngine::FindAnswersByDialog(int dlg_id, std::vector<TDlgAnswer*> *el_list)
{
  int cnt = 0;

  if (dlg_id == 0)
	return 0;

  try
	 {
       el_list->clear();

	   for (int i = 0; i < FItems.size(); i++)
		 {
		   if ((FItems[i]->Dialog == dlg_id) && (FItems[i]->Type == DlgAnsw))
			 {
			   cnt++;
			   el_list->push_back(reinterpret_cast<TDlgAnswer*>(FItems[i]));
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::FindAnswersByDialog", e.ToString());
	   cnt = -1;
	 }

  return cnt;
}
//---------------------------------------------------------------------------

int KobzarEngine::FindScriptsByDialog(int dlg_id, std::vector<TDlgScript*> *el_list)
{
  int cnt = 0;

  if (dlg_id == 0)
	return 0;

  try
	 {
	   for (int i = 0; i < FItems.size(); i++)
		 {
		   if ((FItems[i]->Dialog == dlg_id) && (FItems[i]->Type == DlgScript))
			 {
			   cnt++;
			   el_list->push_back(reinterpret_cast<TDlgScript*>(FItems[i]));
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::FindScriptsByDialog", e.ToString());
	   cnt = -1;
	 }

  return cnt;
}
//---------------------------------------------------------------------------

int KobzarEngine::FindTextElementID(int dlg_id)
{
  int res = 0;

  try
	 {
	   for (int i = 0; i < FItems.size(); i++)
		 {
		   if ((FItems[i]->Dialog == dlg_id) &&
			   (FItems[i]->Type == DlgText))
			 {
			   res = FItems[i]->ID;
             }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::FindTextElementID", e.ToString());
	   res = 0;
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::RemoveFromItems(TDlgBaseText *element)
{
  try
	 {
	   for (int i = 0; i < FItems.size(); i++)
		 {
		   if (FItems[i] == element)
			 {
			   delete FItems[i];
			   FItems.erase(FItems.begin() + i);
			   break;
			 }
		 }

	   RemoveLimboLinks();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::RemoveFromFItems", e.ToString());
	 }
}
//---------------------------------------------------------------------------

bool KobzarEngine::SaveDlgSchema(String file)
{
  bool res = false;

  try
	 {
	   std::unique_ptr<TFileStream> fs(new TFileStream(file.c_str(), fmOpenWrite|fmCreate));

	   fs->Position = 0;

	   int val = 0, left = 0, top = 0;

	   for (int i = 0; i < FItems.size(); i++)
		  {
			val = 0;

			val = FItems[i]->ID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = FItems[i]->Type;
			fs->Position += fs->Write(&val, sizeof(int));

			val = FItems[i]->PrevID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = FItems[i]->NextID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = FItems[i]->Dialog;
			fs->Position += fs->Write(&val, sizeof(int));

			val = FItems[i]->NextDialog;
			fs->Position += fs->Write(&val, sizeof(int));

//тут має бути запис згенерованих координат для візуального редактора
			val = 100;
			fs->Position += fs->Write(&val, sizeof(int));
			fs->Position += fs->Write(&val, sizeof(int));

			WriteStringIntoBinaryStream(fs.get(), FItems[i]->Text);

			if (FItems[i]->Type == DlgScript)
			  {
				TDlgScript *d = dynamic_cast<TDlgScript*>(FItems[i]);

				WriteStringIntoBinaryStream(fs.get(), d->Params);
			  }

			if (FItems[i]->Type == DlgAnsw)
			  {
				TDlgAnswer *d = dynamic_cast<TDlgAnswer*>(FItems[i]);
				bool chk = d->EndDialog;
				fs->Position += fs->Write(&chk, sizeof(bool));
			  }
		  }

	   res = true;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SaveDlgSchema", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

bool KobzarEngine::LoadDlgSchema(String file)
{
  bool res = false;

  try
	 {
	   std::unique_ptr<TFileStream> fs(new TFileStream(file.c_str(), fmOpenRead));

	   ClearItems();

	   fs->Position = 0;

	   int id, linked_id, linked_from_id, left, top, card_of_dialog,
		   dlg_type, next_card_of_dialog;
	   bool end_dlg;
	   TDlgBaseText *lnk;

	   while (fs->Position < fs->Size)
		 {
		   fs->Position += fs->Read(&id, sizeof(int));
		   fs->Position += fs->Read(&dlg_type, sizeof(int));
		   fs->Position += fs->Read(&linked_id, sizeof(int));
		   fs->Position += fs->Read(&linked_from_id, sizeof(int));
		   fs->Position += fs->Read(&card_of_dialog, sizeof(int));
		   fs->Position += fs->Read(&next_card_of_dialog, sizeof(int));
		   fs->Position += fs->Read(&left, sizeof(int));
		   fs->Position += fs->Read(&top, sizeof(int));

		   if (id == 0)
			 throw Exception("Element ID can't be 0");

		   if (card_of_dialog == 0)
			 throw Exception("Dialog ID can't be 0");

		   switch (dlg_type)
			 {
			   case DlgText:
				 {
				   TDlgScreenText *dl = new TDlgScreenText(id, card_of_dialog);

				   dl->Text = ReadStringFromBinaryStream(fs.get());

				   lnk = dl;

				   break;
				 }
			   case DlgAnsw:
				 {
				   TDlgAnswer *dl = new TDlgAnswer(id,
												   card_of_dialog,
												   next_card_of_dialog,
												   linked_id,
												   linked_from_id);

				   dl->Text = ReadStringFromBinaryStream(fs.get());

				   fs->Position += fs->Read(&end_dlg, sizeof(bool));
				   dl->EndDialog = end_dlg;

				   lnk = dl;

				   break;
				 }
			   case DlgScript:
				 {
				   TDlgScript *dl = new TDlgScript(id,
												   card_of_dialog,
												   next_card_of_dialog,
												   linked_id,
												   linked_from_id);

				   dl->Text = ReadStringFromBinaryStream(fs.get());
				   dl->Params = ReadStringFromBinaryStream(fs.get());

				   lnk = dl;

				   break;
				 }
			 }

		   FItems.push_back(lnk);
		 }

	   res = true;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::LoadDlgSchema", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::SearchDependeciesID(int id)
{
  int dpnd = 0;

  if (id == 0)
    return 0;

  try
	 {
       for (auto itm : FItems)
		  {
			if ((itm->PrevID == id) || (itm->NextID == id))
			  dpnd++;
          }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SearchDependeciesID", e.ToString());
       dpnd = -1;
	 }

  return dpnd;
}
//---------------------------------------------------------------------------

int KobzarEngine::SearchDependeciesDialog(int id)
{
  int dpnd = 0;

  if (id == -1)
    return 0;

  try
	 {
	   for (auto itm : FItems)
		  {
			if (itm->Dialog == id)
			  dpnd++;
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SearchDependeciesDialog", e.ToString());
       dpnd = -1;
	 }

  return dpnd;
}
//---------------------------------------------------------------------------

void KobzarEngine::UpdatePrevID(int old_id, int new_id)
{
  try
	 {
	   for (int i = 0; i < FItems.size(); i++)
		  {
			if (FItems[i]->PrevID == old_id)
			  FItems[i]->PrevID = new_id;

			if (FItems[i]->NextID == old_id)
			  FItems[i]->NextID = new_id;
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::UpdatePrevID", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::UpdateDialog(int old_val, int new_val)
{
  try
	 {
	   for (auto itm : FItems)
		  {
			if (itm->Dialog == old_val)
			  itm->Dialog = new_val;
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::UpdateDialog", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::TranslateScript(TDlgScript *el)
{
  int res = 1;

  try
	 {
	   if (!el)
		 throw Exception("Element not found");
	   else if (el->Type != DlgScript)
		 throw Exception("Element with ID = " + IntToStr(el->ID) + ", is not a Script");
	   else
		 {
		   String dir = GetDirPathFromFilePath(String(path));

		   if (!FileExists(dir + "\\scripts\\InnerScriptHeader.eh"))
             throw Exception("Can't load script header!");

		   std::unique_ptr<ELIScript> script(new ELIScript(dir + "\\ELI.dll"));

		   if (!script->Initialised)
			 throw Exception("Script object not initialised!");

		   String header = "#begin Script" + IntToStr(el->ID) + ";\r\n",
				  footer = "\r\n#end;";

		   header += LoadTextFile(dir + "\\scripts\\InnerScriptHeader.eh");
		   header += "\r\n";

		   wchar_t buffer[256];

		   swprintf(buffer, L"%d\r\n", reinterpret_cast<int>(this));
		   script->Interpreter->SetParam(L"pKobzarEngineHandle", buffer);

		   script->Text = header + el->Text + footer;
		   script->Params = el->Params;

		   if (!script->Run())
			 throw Exception(script->Log);
		   else
			 el->Result = script->Result;

		   CreateLog("KobzarEngine::TranslateScript", script->Log);
		 }
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("KobzarEngine::TranslateScript", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

bool KobzarEngine::XMLImport(String xml_file)
{
  bool res = false;

  try
	 {
	   std::unique_ptr<TXMLDocument> ixml(new TXMLDocument(Application));

	   ClearItems();

	   ixml->DOMVendor = GetDOMVendor(SMSXML);
	   ixml->FileName = xml_file;
	   ixml->Active = true;
	   ixml->Encoding = "UTF-8";
	   ixml->Options = ixml->Options << doNodeAutoIndent;

	   _di_IXMLNode DialogFile = ixml->DocumentElement;
	   _di_IXMLNode Dialog;
	   _di_IXMLNode ScreenText;
	   _di_IXMLNode AnswerMassive;
	   _di_IXMLNode Answer;
	   _di_IXMLNode TextOfAnswer;
	   _di_IXMLNode EndDialog;

	   int curr_card = 0;

	   for (int i = 0; i < DialogFile->ChildNodes->Count; i++)
		  {
			Dialog = DialogFile->ChildNodes->Nodes[i];

			for (int j = 0; j < Dialog->ChildNodes->Count; j++)
			   {
				 if (Dialog->ChildNodes->Nodes[j]->GetNodeName() == "ScreenText")
				   {
					 ScreenText = Dialog->ChildNodes->Nodes[j];
					 curr_card = StrToInt(Dialog->GetAttribute("id"));

					 TDlgScreenText *lnk = new TDlgScreenText(GenElementID());

					 lnk->Dialog = curr_card;
					 lnk->Text = ScreenText->Text;
					 FItems.push_back(lnk);
				   }
				 else
				   {
					 AnswerMassive = Dialog->ChildNodes->Nodes[j];

					 for (int k = 0; k < AnswerMassive->ChildNodes->Count; k++)
						{
						  Answer = AnswerMassive->ChildNodes->Nodes[k];
						  int ind = 0;
						  int ncd;
						  bool end;
						  String text;

						  if (Answer->HasAttribute("NextDialog"))
							ncd = StrToInt(Answer->GetAttribute("NextDialog"));

						  ind = Answer->ChildNodes->IndexOf("Text");

						  if (ind >= 0)
							text = Answer->ChildNodes->Nodes[ind]->Text;

						  ind = Answer->ChildNodes->IndexOf("EndDialog");

						  if (ind < 0)
							end = false;
						  else if (Answer->ChildNodes->Nodes[ind]->Text == "True")
							end = true;
						  else
							end = false;

						  if (Answer->HasAttribute("Params")) //Script
							{
							  TDlgScript *lnk = new TDlgScript(GenElementID());

							  lnk->Dialog = curr_card;
							  lnk->NextDialog = 0;
							  lnk->Params = Answer->GetAttribute("Params");
							  lnk->Text = text;
							  FItems.push_back(lnk);
							}
						  else //Answer
							{
							  TDlgAnswer *answ = new TDlgAnswer(GenElementID());

							  answ->Dialog = curr_card;
							  answ->NextDialog = ncd;
							  answ->Text = text;
							  answ->EndDialog = end;
							  FItems.push_back(answ);
							}
						}
				   }
			   }
		  }

	   BuildLinksAfterXMLImport();

	   res = true;
	 }
  catch (Exception &e)
	 {
       CoUninitialize();
	   CreateLog("KobzarEngine::XMLImport", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

bool KobzarEngine::XMLExport(String xml_file)
{
  bool res = false;

  try
	 {
	   std::unique_ptr<TStringList> list(new TStringList());

	   String xml_exp = "<StoryFile>\r\n";

	   for (int i = 0; i < FItems.size(); i++)
		  {
			if (FItems[i]->Type == DlgText)
			  {
				xml_exp += "\t<Dialog id = '" +
						   IntToStr(FItems[i]->Dialog) +
						   "'>\r\n";
				xml_exp += FItems[i]->CreateXML();
				xml_exp += "\t\t<Answers>\r\n";

				for (int j = 0; j < FItems.size(); j++)
				   {
					 if (FItems[j]->PrevID == FItems[i]->ID)
					   {
						 xml_exp += FItems[j]->CreateXML();
						 xml_exp += "\r\n";
					   }
				   }

				xml_exp += "\t\t</Answers>\r\n";
				xml_exp += "\t</Dialog>\r\n";
			  }
		  }

	   xml_exp += "</StoryFile>\r\n";

	   list->Text = xml_exp;
	   list->SaveToFile(xml_file, TEncoding::UTF8);

	   res = true;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::XMLExport", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::ClearItems()
{
  try
	 {
	   for (auto itm : FItems)
		  delete itm;

	   FItems.clear();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::ClearFItems", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::BuildLinksAfterXMLImport()
{
  try
	 {
	   for (auto itm : FItems)
		  {
			if (itm->Type != DlgText)
			  {
				itm->PrevID = FindTextElementID(itm->Dialog);
				itm->NextID = FindTextElementID(itm->NextDialog);
			  }
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::BuildLinksAfterXMLImport", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::RemoveLimboLinks()
{
  try
	 {
	   for (auto itm : FItems)
		  {
			if ((itm->PrevID > -1) && (!FindElement(itm->PrevID)))
			  itm->PrevID = 0;

			if ((itm->NextID > -1) && (!FindElement(itm->NextID)))
			  itm->NextID = 0;
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::RemoveLimboLinks", e.ToString());
	 }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
