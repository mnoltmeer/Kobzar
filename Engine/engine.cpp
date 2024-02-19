/*!
Copyright 2023 Maxim Noltmeer (m.noltmeer@gmail.com)

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

int KobzarEngine::CreateStory(const wchar_t *story_file)
{
  int res = 1;

  try
	 {
	   res = 1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::CreateStory: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::LoadStory(const wchar_t *story_file)
{
  int res = 1;

  try
	 {
	   res = 1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::LoadStory: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::SaveStory()
{
  int res = 1;

  try
	 {
	   res = 1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::SaveStory: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void KobzarEngine::CloseStory()
{
  int res = 1;

  try
	 {
	   res =1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::CloseStory: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::ClearStory()
{
  int res = 1;

  try
	 {
	   res =1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::ClearStory: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::AddScene: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::AddAnswer: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::AddScript: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Activate(int id)
{
  int res = 1;

  try
	 {
	   res = 1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::Activate: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Remove(int id)
{
  int res = 1;

  try
	 {
	   res = 1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::Remove: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Link(int id, int to_id)
{
  int res = 1;

  try
	 {
	   res = 1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::Link: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::Unlink(int id, int to_id)
{
  int res = 1;

  try
	 {
	   res = 1;
	 }
  catch (Exception &e)
	 {
	   res = 0;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::Unlink: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

KobzarEngine::KobzarEngine()
{
  write_log = false;
  ActiveItem = nullptr;
};
//-------------------------------------------------------------------------

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

int KobzarEngine::FindAnswersByDialog(int dlg_id, std::vector<TDlgBaseText*> *el_list)
{
  int cnt = 0;

  if (dlg_id == -1)
	return 0;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if ((items[i]->CardOfDialog == dlg_id) &&
			   (items[i]->Cathegory != DLG_TEXT_LIKE))
			 {
			   cnt++;
			   el_list->push_back(items[i]);
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder(CreateLog("KobzarEngine::FindAnswersByDialog", e.ToString());
	   cnt = -1;
	 }

  return cnt;
}
//---------------------------------------------------------------------------

int KobzarEngine::FindTextElementID(int crd_dlg)
{
  int res = -1;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if ((items[i]->CardOfDialog == crd_dlg) &&
			   (items[i]->Cathegory == DLG_TEXT_LIKE))
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

bool KobzarEngine::SaveDlgSchema(const wchar_t *file)
{
  bool res = false;

  try
	 {
	   std::unique_ptr<TFileStream> fs(new TFileStream(file, fmOpenWrite|fmCreate));

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
				WriteStringIntoBinaryStream(fs.get(), d->Result);
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

bool KobzarEngine::LoadDlgSchema(const wchar_t *file)
{
  bool res = false;

  try
	 {
	   std::unique_ptr<TFileStream> fs(new TFileStream(file, fmOpenRead));

	   ClearItems();

	   fs->Position = 0;
	   int id, linked_id, linked_from_id, left, top, card_of_dialog,
		   dlg_type, next_card_of_dialog;
	   int text_len;
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
		   fs->Position += fs->Read(&text_len, sizeof(int));

		   switch (dlg_type)
			 {
			   case DlgText:
				 {
				   TDlgScreenText *dl = new TDlgScreenText(id, card_of_dialog);

				   if (text_len > 0)
					 dl->Text = ReadStringFromBinaryStream(fs.get(), text_len);

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

				   if (text_len > 0)
					 dl->Text = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&end_dlg, sizeof(bool));
				   dl->EndDialog = end_dlg;

				   lnk = dl;

				   break;
				 }
			   case DlgScript:
				 {
				   TDlgScript *dl = new TDlgScript(id, card_of_dialog);

				   if (text_len > 0)
					 dl->Text = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->Params = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->Result = ReadStringFromBinaryStream(fs.get(), text_len);

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
	   _di_IXMLNode ScreenTextMassive;
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
				 if (CardOfDialog->ChildNodes->Nodes[j]->GetNodeName() == "ScreenTextMassive")
				   {
					 ScreenTextMassive = CardOfDialog->ChildNodes->Nodes[j];
					 ScreenText = ScreenTextMassive->ChildNodes->Nodes[0];
					 curr_card = StrToInt(CardOfDialog->GetAttribute("id"));

					 if (ScreenText->HasAttribute("Params")) //Script
					   {
						 TDlgScript *lnk = new TDlgScript(GenElementID());

						 lnk->CardOfDialog = curr_card;
						 lnk->Params = ScreenText->GetAttribute("Params");
						 lnk->Text = ScreenText->ChildNodes->Nodes[0]->Text;
						 lnk->Result = ScreenText->ChildNodes->Nodes[1]->Text;
						 items.push_back(lnk);
					   }
					 else  //ScreenText
					   {
						 TDlgScreenText *lnk = new TDlgScreenText(GenElementID());

						 lnk->CardOfDialog = curr_card;
						 lnk->Text = ScreenText->ChildNodes->Nodes[0]->Text;
						 items.push_back(lnk);
					   }
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

		BuildLinksAfterXMLImport();
	 }
  catch (Exception &e)
	 {
	   CreateLog("KobzarEngine::XMLImport", e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::XMLExport(const wchar_t *path)
{
  try
	 {
	   std::unique_ptr<TStringList> list(new TStringList());

	   String xml_exp = "<DialogFile>\r\n";

	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->Cathegory == DLG_TEXT_LIKE)
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

	   xml_exp += "</DialogFile>\r\n";

	   list->Text = xml_exp;
	   list->SaveToFile(path, TEncoding::UTF8);
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
			if (items[i]->Cathegory != DLG_TEXT_LIKE)
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

int KobzarEngine::GetLinkedID()
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::GetLinkedID", "No active element!");
	  return 0;
	}
  else
	return ActiveItem->LinkedID;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetLinkedID(int val)
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::SetLinkedID", "No active element!");
	  return;
	}

  if (Cathegory == DLG_TEXT_LIKE)
	return;

  TDlgBaseText *lnk = Library->FindElement(val);

  if (lnk && (lnk->Cathegory == DLG_TEXT_LIKE))
	{
	  l_id = val;
	  cd = lnk->CardOfDialog;
	}
  else if (lnk && (lnk->Cathegory != DLG_TEXT_LIKE))
	{
	  MessageBox(Application->Handle,
				 String("No TEXT_LIKE element with ID = LinkedID (" +
				 IntToStr(val) + ")").c_str(),
				 String("Element ID: " + IntToStr(ID) + " report").c_str(),
				 MB_OK | MB_ICONERROR);
	}
  else
	{
	  if (MessageBox(Application->Handle,
					 String("No TEXT_LIKE element with ID = LinkedID (" +
					 IntToStr(val) +
					 ").\nSet value anyway?").c_str(),
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes)
		{
		  l_id = val;
          cd = -1;
		}
	}
}
//---------------------------------------------------------------------------

int KobzarEngine::GetLinkedFromID()
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::GetLinkedFromID", "No active element!");
	  return 0;
	}
  else
	return ActiveItem->LinkedFromID;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetLinkedFromID(int val)
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::SetLinkedFromID", "No active element!");
	  return;
	}

  if (Cathegory == DLG_TEXT_LIKE)
	return;

  TDlgBaseText *lnk = Library->FindElement(val);

  if (lnk && (lnk->Cathegory == DLG_TEXT_LIKE))
	{
	  l_fr_id = val;
	  ncd = lnk->CardOfDialog;
	}
  else if (lnk && (lnk->Cathegory != DLG_TEXT_LIKE))
	{
	  MessageBox(Application->Handle,
				 String("Element with ID = LinkedFromID (" +
				 IntToStr(val) + ") is not TEXT_LIKE").c_str(),
				 String("Element ID: " + IntToStr(ID) + " report").c_str(),
				 MB_OK | MB_ICONERROR);
	}
  else
	{
	  if (MessageBox(Application->Handle,
					 String("No TEXT_LIKE element with ID = LinkedFromID (" +
							IntToStr(val) +
							").\nSet value anyway?").c_str(),
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes)
		{
		  l_fr_id = val;
          ncd = -1;
		}
	}
}
//---------------------------------------------------------------------------

int KobzarEngine::GetCardOfDialog()
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::GetCardOfDialog", "No active element!");
	  return 0;
	}
  else
	return ActiveItem->CardOfDialog;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetCardOfDialog(int val)
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::SetCardOfDialog", "No active element!");
	  return;
	}

  int old = cd;

  if (Cathegory != DLG_TEXT_LIKE)
	{
	  cd = val;
	  LinkedID = Library->FindTextElementID(val);
	}
  else
	{
	  cd = val;
	  std::vector<TDlgBaseText*> lnks;

	  if (Library->FindAnswersByDialog(cd, &lnks) > 0)
		{
		  if (MessageBox(Application->Handle,
						 String("Founded ANSW_LIKE elements with CardOfDialog = " +
								IntToStr(cd) +
								".\nCreate links?").c_str(),
						 String("Element ID: " + IntToStr(ID) + " report").c_str(),
						 MB_YESNO | MB_ICONASTERISK) == mrYes)
			{
			  for (int i = 0; i < lnks.size(); i++)
				 lnks[i]->LinkedID = ID;
			}
		}

	  if ((old != cd) && (FLibrary->SearchDependeciesDialog(old) > 0))
		{
          if (MessageBox(Application->Handle,
						 String("Founded links of old ANSW_LIKE elements.\nRebuild this links?").c_str(),
						 String("Element ID: " + IntToStr(ID) + " report").c_str(),
						 MB_YESNO | MB_ICONASTERISK) == mrYes)
			{
			  Library->UpdateCardOfDialog(old, CardOfDialog);
			}
		  else
			{
			  Library->UpdateLinkedID(ID, -1);
			}
		}
	}
}
//---------------------------------------------------------------------------

int KobzarEngine::GetNextCardOfDialog()
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::GetCardOfDialog", "No active element!");
	  return 0;
	}
  else
	return ActiveItem->NextCardOfDialog;
}
//---------------------------------------------------------------------------

void KobzarEngine::SetNextCardOfDialog(int val)
{
  if (!ActiveItem)
	{
	  CreateLog("KobzarEngine::SetNextCardOfDialog", "No active element!");
	  return;
	}

  if (Cathegory != DLG_TEXT_LIKE)
	{
	  ncd = val;
	  int new_dlg_id = Library->FindTextElementID(ncd);

	  if (new_dlg_id > -1)
		{
		  TDlgBaseText *ndlg = Library->FindElement(new_dlg_id);
		  ndlg->LinkedID = ID;
		  LinkedFromID = ndlg->ID;
		}
	}
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
