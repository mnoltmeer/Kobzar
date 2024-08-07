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
#include <Xml.adomxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.Win.msxmldom.hpp>

#include "..\..\work-functions\MyFunc.h"
#include "engine.h"
//---------------------------------------------------------------------------

extern String UsedAppLogDir;

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  LogPath = GetEnvironmentVariable("USERPROFILE") + "\\Documents\\Kobzar";

  if (!DirectoryExists(LogPath))
	CreateDir(LogPath);

  UsedAppLogDir = "Kobzar";

  GetModuleFileName(hinst, path, sizeof(path));

  return 1;
}
//--------------------------------------------------------------------------

DLL_EXPORT int __stdcall GetKEInterface(KE_INTERFACE **eInterface)
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "*eInterface = new KobzarEngine() :" + e.ToString());
	 }

  return res;
}
//-------------------------------------------------------------------------

DLL_EXPORT int __stdcall FreeKEInterface(KE_INTERFACE **eInterface)
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
  CurrentFile = "";
  LastError = "";
};
//-------------------------------------------------------------------------

void KobzarEngine::LoadFunctionsToELI(ELI_INTERFACE *FEIface)
{
  FEIface->AddFunction(L"_Foo", L"num pID", &eFoo);
}
//---------------------------------------------------------------------------

int KobzarEngine::CreateStory(const wchar_t *story_file)
{
  int res = 1;

  try
	 {
	   String file = ParseString(String(story_file), "\\\\", GetDirPathFromFilePath(String(path)));
	   SaveToFile(file, "");
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::CreateStory", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::LoadStory(const wchar_t *story_file)
{
  int res = 1;

  try
	 {
	   String file = ParseString(String(story_file), "\\\\", GetDirPathFromFilePath(String(path)));

	   if (UpperCase(GetFileExtensionFromFileName(file)) == "SCS")
		 LoadDlgSchema(file);
	   else
		 XMLImport(file);

       CurrentFile = String(story_file);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::LoadStory", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::SaveStory()
{
  int res = 1;

  try
	 {
	   if (UpperCase(GetFileExtensionFromFileName(CurrentFile)) == "SCS")
		 SaveDlgSchema(CurrentFile);
	   else
		 XMLExport(CurrentFile);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::SaveStory", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::CloseStory()
{
  int res = 1;

  try
	 {
	   ClearStory();
       CurrentFile = "";
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::CloseStory", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::ClearStory()
{
  int res = 1;

  try
	 {
	   ClearItems();
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::ClearStory", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::AddScene()
{
  int res = 1;

  try
	 {
	   TDlgScreenText *tmp = new TDlgScreenText(GenElementID(), GenDialogID());
	   items.push_back(tmp);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::AddScene", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::AddAnswer()
{
  int res = 1;

  try
	 {
	   TDlgAnswer *tmp = new TDlgAnswer(GenElementID());
	   items.push_back(tmp);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::AddAnswer", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::AddScript()
{
  int res = 1;

  try
	 {
	   TDlgScript *tmp = new TDlgScript(GenElementID());
	   items.push_back(tmp);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::AddScript", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Activate(int id)
{
  int res = 1;

  try
	 {
	   ActiveItem = FindElement(id);

       if (!ActiveItem)
		 throw Exception("No active element!");
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::Activate", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::RunScript(int id)
{
  int res = 1;

  try
	 {
	   TDlgScript *itm = reinterpret_cast<TDlgScript*>(FindElement(id));

	   if (!itm)
		 throw Exception("Element with ID = " + IntToStr(id) + " not found");
	   else
		 res = RunScript(itm);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::RunScript", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Remove(int id)
{
  int res = 1;

  try
	 {
	   TDlgBaseText *itm = FindElement(id);

	   if (!itm)
		 throw Exception("Element with ID = " + IntToStr(id) + " not found");
	   else
         RemoveFromItems(itm);
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::Remove", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Link(int id, int to_id)
{
  int res = 1;

  try
	 {
	   TDlgBaseText *from = FindElement(id);
	   TDlgBaseText *to = FindElement(to_id);

	   if (!from)
		 throw Exception("Element with ID = " + IntToStr(id) + " not found");

	   if (!to)
		 throw Exception("Element with ID = " + IntToStr(to_id) + " not found");

	   if ((from->Type == DlgAnsw) &&
		  (to->Type == DlgText)) //������ ������� - ³������, ������ - �����
		{
		  if (to->LinkedID > -1) //���� ����� ��� ��������� �� ���� ³�����
			{
			  TDlgBaseText *old_answ = FindElement(to->LinkedID);

			  if (old_answ) //�������� ����'���� � ����� ³�����
				{
				  old_answ->NextCardOfDialog = -1;
				  old_answ->LinkedFromID = -1;
				}
			}

		  from->NextCardOfDialog = to->CardOfDialog;
		  from->LinkedFromID = to->ID;
		}
	  else if ((from->Type == DlgText) &&
			   ((to->Type == DlgAnsw) || (to->Type == DlgScript))) //������ ������� - �����, ������ - ³������
		{
		  to->CardOfDialog = from->CardOfDialog;
		  to->LinkedID = from->ID;
		}
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::Link", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Unlink(int id, int to_id)
{
  int res = 1;

  try
	 {
	   TDlgBaseText *from = FindElement(id);
	   TDlgBaseText *to = FindElement(to_id);

	   if (!from)
		 throw Exception("Element with ID = " + IntToStr(id) + " not found");

	   if (!to)
		 throw Exception("Element with ID = " + IntToStr(to_id) + " not found");

	   if (from->Type == DlgAnsw)
		 from->NextCardOfDialog = -1;

	   if ((to->Type == DlgAnsw) || (to->Type == DlgScript))
		 to->CardOfDialog = -1;

	   from->LinkedFromID = -1;
	   to->LinkedID = -1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::Unlink", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetVersion()
{
  return GetVersionInString(path).c_str();
}
//-------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetLastError()
{
  return LastError.c_str();
}
//-------------------------------------------------------------------------
//------------------INTERNAL FUNCTIONS-------------------------------------
//-------------------------------------------------------------------------
void KobzarEngine::CreateLog(const String &method_name, const String &error)
{
  LastError = error;
  SaveLogToUserFolder("Engine.log", "Kobzar", method_name + ": " + error);
}
//---------------------------------------------------------------------------

int KobzarEngine::GenDialogID()
{
  int max = -1;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if (items[i]->CardOfDialog > max)
			 max = items[i]->CardOfDialog;
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GenDialogID", e.ToString());
	   max = -2;
	 }

  return max + 1;
}
//---------------------------------------------------------------------------

int KobzarEngine::GenElementID()
{
  int max = -1;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if (items[i]->ID > max)
			 max = items[i]->ID;
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GenElementID", e.ToString());
	   max = -2;
	 }

  return max + 1;
}
//---------------------------------------------------------------------------

TDlgBaseText *KobzarEngine::FindElement(int id)
{
  TDlgBaseText *res = NULL;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if (items[i]->ID == id)
			 res = items[i];
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::FindElement", e.ToString());
	   res = NULL;
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::FindLinkedElements(int id, std::vector<TDlgBaseText*> *el_list)
{
  int cnt = 0;

  if (id == -1)
	return 0;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if (items[i]->LinkedID == id)
			 {
			   cnt++;
			   el_list->push_back(items[i]);
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

  if (dlg_id == -1)
	return 0;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if ((items[i]->CardOfDialog == dlg_id) && (items[i]->Type == DlgAnsw))
			 {
			   cnt++;
			   el_list->push_back(reinterpret_cast<TDlgAnswer*>(items[i]));
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

  if (dlg_id == -1)
	return 0;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if ((items[i]->CardOfDialog == dlg_id) && (items[i]->Type == DlgScript))
			 {
			   cnt++;
			   el_list->push_back(reinterpret_cast<TDlgScript*>(items[i]));
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
  int res = -1;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if ((items[i]->CardOfDialog == dlg_id) &&
			   (items[i]->Type == DlgText))
			 {
			   res = items[i]->ID;
             }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::FindTextElementID", e.ToString());
	   res = -1;
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::RemoveFromItems(TDlgBaseText *element)
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if (items[i] == element)
			 {
			   delete items[i];
			   items.erase(items.begin() + i);
			   break;
			 }
		 }

	   RemoveLimboLinks();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::RemoveFromItems", e.ToString());
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
	   int val = 0;

	   for (int i = 0; i < items.size(); i++)
		  {
			val = 0;

			val = items[i]->ID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->Type;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->LinkedID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->LinkedFromID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->CardOfDialog;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->NextCardOfDialog;
			fs->Position += fs->Write(&val, sizeof(int));

			val = 0;
			fs->Position += fs->Write(0, sizeof(int));

			val = 0;
			fs->Position += fs->Write(0, sizeof(int));

			WriteStringIntoBinaryStream(fs.get(), items[i]->Text);

			if (items[i]->Type == DlgScript)
			  {
				TDlgScript *d = dynamic_cast<TDlgScript*>(items[i]);

				WriteStringIntoBinaryStream(fs.get(), d->Params);
				//WriteStringIntoBinaryStream(fs.get(), d->Result);
			  }

			if (items[i]->Type == DlgAnsw)
			  {
				TDlgAnswer *d = dynamic_cast<TDlgAnswer*>(items[i]);
				bool chk = d->EndDialog;
				fs->Position += fs->Write(&chk, sizeof(bool));
			  }
		  }

	   res = true;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SaveDlgSchema", e.ToString());
	   res = false;
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

		   items.push_back(lnk);
		 }

	   res = true;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::LoadDlgSchema", e.ToString());
	   res = false;
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::SearchDependeciesID(int id)
{
  int dpnd = 0;

  if (id == -1)
    return 0;

  try
	 {
       for (int i = 0; i < items.size(); i++)
		  {
			if ((items[i]->LinkedID == id) || (items[i]->LinkedFromID == id))
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
	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->CardOfDialog == id)
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

void KobzarEngine::UpdateLinkedID(int old_id, int new_id)
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->LinkedID == old_id)
			  items[i]->LinkedID = new_id;

			if (items[i]->LinkedFromID == old_id)
			  items[i]->LinkedFromID = new_id;
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::UpdateLinkedID", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::UpdateCardOfDialog(int old_val, int new_val)
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->CardOfDialog == old_val)
			  items[i]->CardOfDialog = new_val;
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::UpdateCardOfDialog", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::RunScript(TDlgScript *el)
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
		   std::unique_ptr<ELIScript> script(new ELIScript(GetDirPathFromFilePath(String(path)) + "\\ELI.dll"));

		   if (!script->Initialised)
			 throw Exception("Script object not initialised!");

		   String header = "#begin Script_" + IntToStr(el->ID) + ";\r\n#protect\r\n{\r\n";
		   String footer = "\r\n}\r\n#end;";

		   LoadFunctionsToELI(script->Interpreter);
		   script->Text = header + el->Text + footer;
		   script->Params = el->Params;

		   if (!script->Run())
			 throw Exception(script->Log);
		   else
			 el->Result = script->Result;
		 }
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   CreateLog("Story::RunScript", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::XMLImport(String xml_file)
{
  try
	 {
	   std::unique_ptr<TXMLDocument> ixml(new TXMLDocument(Application));

	   ClearItems();

	   ixml->DOMVendor = GetDOMVendor("MSXML");
	   ixml->FileName = xml_file;
	   ixml->Active = true;
	   ixml->Encoding = "UTF-8";
	   ixml->Options = ixml->Options << doNodeAutoIndent;

	   _di_IXMLNode DialogFile = ixml->DocumentElement;
	   _di_IXMLNode CardOfDialog;
	   _di_IXMLNode ScreenText;
	   _di_IXMLNode AnswerMassive;
	   _di_IXMLNode Answer;
	   _di_IXMLNode TextOfAnswer;
	   _di_IXMLNode EndDialog;

	   int curr_card = -1;

	   for (int i = 0; i < DialogFile->ChildNodes->Count; i++)
		  {
			CardOfDialog = DialogFile->ChildNodes->Nodes[i];

			for (int j = 0; j < CardOfDialog->ChildNodes->Count; j++)
			   {
				 if (CardOfDialog->ChildNodes->Nodes[j]->GetNodeName() == "ScreenText")
				   {
					 ScreenText = CardOfDialog->ChildNodes->Nodes[j];
					 curr_card = StrToInt(CardOfDialog->GetAttribute("id"));

					 TDlgScreenText *lnk = new TDlgScreenText(GenElementID());

					 lnk->CardOfDialog = curr_card;
					 lnk->Text = ScreenText->Text;
					 items.push_back(lnk);
				   }
				 else
				   {
					 AnswerMassive = CardOfDialog->ChildNodes->Nodes[j];

					 for (int k = 0; k < AnswerMassive->ChildNodes->Count; k++)
						{
						  Answer = AnswerMassive->ChildNodes->Nodes[k];
						  int ind = 0;
						  int ncd;
						  bool end;
						  String text;

						  if (Answer->HasAttribute("NextCardOfDialog"))
							ncd = StrToInt(Answer->GetAttribute("NextCardOfDialog"));

						  ind = Answer->ChildNodes->IndexOf("TextOfAnswer");

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

							  lnk->CardOfDialog = curr_card;
							  lnk->NextCardOfDialog = -1;
							  lnk->Params = Answer->GetAttribute("Params");
							  lnk->Text = text;
							  items.push_back(lnk);
							}
						  else //Answer
							{
							  TDlgAnswer *answ = new TDlgAnswer(GenElementID());

							  answ->CardOfDialog = curr_card;
							  answ->NextCardOfDialog = ncd;
							  answ->Text = text;
							  answ->EndDialog = end;
							  items.push_back(answ);
							}
						}
				  }
			   }
		  }

		BuildLinksAfterXMLImport();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::XMLImport", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::XMLExport(String xml_file)
{
  try
	 {
	   std::unique_ptr<TStringList> list(new TStringList());

	   String xml_exp = "<StoryFile>\r\n";

	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->Type == DlgText)
			  {
				xml_exp += "\t<CardOfDialog id = '" +
						   IntToStr(items[i]->CardOfDialog) +
						   "'>\r\n";
				xml_exp += items[i]->CreateXML();
				xml_exp += "\t\t<AnswersMassive>\r\n";

				for (int j = 0; j < items.size(); j++)
				   {
					 if (items[j]->LinkedID == items[i]->ID)
					   {
						 xml_exp += items[j]->CreateXML();
						 xml_exp += "\r\n";
					   }
				   }

				xml_exp += "\t\t</AnswersMassive>\r\n";
				xml_exp += "\t</CardOfDialog>\r\n";
			  }
		  }

	   xml_exp += "</StoryFile>\r\n";

	   list->Text = xml_exp;
	   list->SaveToFile(xml_file, TEncoding::UTF8);
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::XMLExport", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::ClearItems()
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  delete items[i];

	   items.clear();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::ClearItems", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::BuildLinksAfterXMLImport()
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->Type != DlgText)
			  {
				items[i]->LinkedID = FindTextElementID(items[i]->CardOfDialog);
				items[i]->LinkedFromID = FindTextElementID(items[i]->NextCardOfDialog);
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
	   for (int i = 0; i < items.size(); i++)
		  {
			if ((items[i]->LinkedID > -1) && (!FindElement(items[i]->LinkedID)))
			  items[i]->LinkedID = -1;

			if ((items[i]->LinkedFromID > -1) && (!FindElement(items[i]->LinkedFromID)))
			  items[i]->LinkedFromID = -1;
		  }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::RemoveLimboLinks", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::GetID()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 res = ActiveItem->ID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetID", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetID(int val)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 UpdateLinkedID(ActiveItem->ID, val);
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetID", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::GetLinkedID()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 res = ActiveItem->LinkedID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetLinkedID", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetLinkedID(int val)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type == DlgText)
		 CreateLog("KobzarEngine::SetLinkedID", "Can't change LinkedID property of TEXT element");
	   else
		 {
		   TDlgBaseText *lnk = FindElement(val);

		   if (lnk && (lnk->Type == DlgText))
			 {
			   ActiveItem->LinkedID = val;
			   ActiveItem->CardOfDialog = lnk->CardOfDialog;
			 }
		   else if (lnk && (lnk->Type != DlgText))
			 throw Exception("Element with ID = LinkedID (" + IntToStr(val) + ") is not TEXT element");
		   else
			 throw Exception("No TEXT_LIKE element with ID = LinkedID (" + IntToStr(val) + ")");
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetLinkedID", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::GetLinkedFromID()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 res = ActiveItem->LinkedFromID;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetLinkedFromID", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetLinkedFromID(int val)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 {
		   if (ActiveItem->Type == DlgText)
			 throw Exception("Can't change LinkedFromID property of TEXT element");

		   TDlgBaseText *lnk = FindElement(val);

		   if (lnk && (lnk->Type == DlgText))
			 {
			   ActiveItem->LinkedFromID = val;
			   ActiveItem->NextCardOfDialog = lnk->CardOfDialog;
			 }
		   else if (lnk && (lnk->Type != DlgText))
			 throw Exception("Element with ID = LinkedFromID (" + IntToStr(val) + ") is not a TEXT");
		   else
			 throw Exception("No TEXT element with ID = LinkedFromID (" + IntToStr(val) + ")");
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetLinkedFromID", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::GetDialog()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 res = ActiveItem->CardOfDialog;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetDialog", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetDialog(int val)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 {
		   int old = ActiveItem->CardOfDialog;

		   if (ActiveItem->Type != DlgText)
			 {
			   ActiveItem->CardOfDialog = val;
			   ActiveItem->LinkedID = FindTextElementID(val);
			 }
		   else
			 {
			   ActiveItem->CardOfDialog = val;
			   std::vector<TDlgAnswer*> lnks;

			   if (FindAnswersByDialog(ActiveItem->CardOfDialog, &lnks) > 0)
				 {
				   CreateLog("KobzarEngine::SetDialog", "Founded elements with CardOfDialog. Creating links");

				   for (int i = 0; i < lnks.size(); i++)
					 lnks[i]->LinkedID = ActiveItem->ID;
				 }

			   if ((old != ActiveItem->CardOfDialog) && (SearchDependeciesDialog(old) > 0))
				 {
				   CreateLog("KobzarEngine::SetDialog", "Founded links of old elements. Rebuilding links");
				   UpdateCardOfDialog(old, ActiveItem->CardOfDialog);
				 }
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetDialog", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::GetNextDialog()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 res = ActiveItem->NextCardOfDialog;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetNextDialog", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetNextDialog(int val)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 {
		   if (ActiveItem->Type != DlgText)
			 {
			   ActiveItem->NextCardOfDialog = val;
			   int new_dlg_id = FindTextElementID(ActiveItem->NextCardOfDialog);

			   if (new_dlg_id > -1)
				 {
				   TDlgBaseText *ndlg = FindElement(new_dlg_id);
				   ndlg->LinkedID = ActiveItem->ID;
				   ActiveItem->LinkedFromID = ndlg->ID;
				 }
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetNextDialog", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::GetType()
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 res = ActiveItem->Type;
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetType", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t *KobzarEngine::GetText()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 res = ActiveItem->Text.c_str();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetText", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetText(const wchar_t *val)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 ActiveItem->Text = String(val);
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetText", e.ToString());
	 }
}
//---------------------------------------------------------------------------

int KobzarEngine::IsEndDialog()
{
  int res = -1;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type == DlgAnsw)
		 res = reinterpret_cast<TDlgAnswer*>(ActiveItem)->EndDialog;
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not an Answer");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::IsEndDialog", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetEndDialog(bool val)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type == DlgAnsw)
		 {
		   TDlgAnswer *itm = reinterpret_cast<TDlgAnswer*>(ActiveItem);

		   itm->EndDialog = val;

		   if (val)
			 {
			   itm->PrevLinkedFromID = itm->LinkedFromID;
			   itm->LinkedFromID = -1;
			   itm->NextCardOfDialog = -1;
			 }
		   else if (itm->PrevLinkedFromID > 0)
			 {
			   SetLinkedFromID(itm->PrevLinkedFromID);
			   itm->PrevLinkedFromID = -1;
			 }
		 }
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not an Answer");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetEndDialog", e.ToString());
	 }
}
//---------------------------------------------------------------------------

const wchar_t *KobzarEngine::GetParams()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type == DlgScript)
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

void KobzarEngine::SetParams(const wchar_t *val)
{
  int res = 0;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type == DlgScript)
		 reinterpret_cast<TDlgScript*>(ActiveItem)->Params = String(val);
	   else
		 throw Exception("Element with ID = " + IntToStr(ActiveItem->ID) + ", is not a Script");
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SetParams", e.ToString());
	 }
}
//---------------------------------------------------------------------------

const wchar_t *KobzarEngine::GetResult()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type == DlgScript)
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

int KobzarEngine::TellStory(const wchar_t *story_file)
{
  int res = 0;

  try
	 {
	   if (!LoadStory(story_file))
		 throw Exception("Can't load story");
	   else
		 res = LoadDialog(0);
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::TellStory", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::LoadDialog(int dlg_id)
{
  int res = 0;

  try
	 {
	   if (Activate(FindTextElementID(dlg_id)))
		 {
		   std::vector<TDlgScript*> scripts;

		   FindScriptsByDialog(dlg_id, &scripts);

		   for (int i = 0; i < scripts.size(); i++)
			  RunScript(scripts[i]);

           res = 1;
		 }
	   else
         throw Exception("No Scene in dialog with ID = " + IntToStr(dlg_id));
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::LoadScene", e.ToString());
	 }
}
//---------------------------------------------------------------------------

const wchar_t *KobzarEngine::GetScene()
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type != DlgText)
		 throw Exception("Active element isn't a Scene");
	   else
		 res = ActiveItem->Text.c_str();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetScene", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::GetAnswerCount()
{
  int res = -1;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else if (ActiveItem->Type != DlgText)
		 throw Exception("Active element isn't a Scene");
	   else
		 {
		   std::vector<TDlgAnswer*> answers;

		   res = FindAnswersByDialog(ActiveItem->CardOfDialog, &answers);
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetAnswerCount", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

const wchar_t *KobzarEngine::GetAnswer(int index)
{
  const wchar_t *res = nullptr;

  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active element!");
	   else
		 {
		   std::vector<TDlgAnswer*> answers;

		   FindAnswersByDialog(ActiveItem->ID, &answers);

		   res = answers[index]->Text.c_str();
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::GetAnswer", e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::SelectAnswer(int index)
{
  try
	 {
	   if (!ActiveItem)
		 throw Exception("No active Scene!");
       else if (ActiveItem->Type != DlgText)
		 throw Exception("Active element isn't a Scene");
       else
		 {
		   std::vector<TDlgAnswer*> answers;

		   FindAnswersByDialog(ActiveItem->CardOfDialog, &answers);

		   LoadDialog(answers[index]->NextCardOfDialog);
		 }
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::SelectAnswer", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __stdcall KobzarEngine::eFoo(void *p)
{
  ELI_INTERFACE *ep;

  try
	 {
	   ep = static_cast<ELI_INTERFACE*>(p);

	   String id = ep->GetParamToStr(L"pID");

	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"1");
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "*eInterface = new KobzarEngine() :" + e.ToString());
	   ep->SetFunctionResult(ep->GetCurrentFuncName(), L"0");
	 }
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
