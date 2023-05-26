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

int KobzarEngine::AddScreenText()
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::AddScreenText: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

int KobzarEngine::AddAnswer()
{
  int res = 1;

  try
	 {
	   TDlgBaseAnswer *tmp = new TDlgBaseAnswer(GenElementID());
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

int KobzarEngine::AddAdvAnswer()
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::AddAdvAnswer: " + e.ToString());
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

int KobzarEngine::AddCondition()
{
  int res = 1;

  try
	 {
	   TDlgCondition *tmp = new TDlgCondition(GenElementID());
	   items.push_back(tmp);
	 }
  catch (Exception &e)
	 {
	   return res;
	   SaveLogToUserFolder("Engine.log", "Kobzar", "Story::AddCondition: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

KobzarEngine::KobzarEngine()
{
  write_log = false;
};
//-------------------------------------------------------------------------

const wchar_t * __stdcall KobzarEngine::GetVersion()
{
  return GetVersionInString(path).c_str();
}
//-------------------------------------------------------------------------
//------------------INTERNAL FUNCTIONS-------------------------------------
//-------------------------------------------------------------------------
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::DialogID: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::ElementID: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "DlgClass::FindElement: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::FindLinkedElements: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::FindAnswersByDialog: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::FindTextElementID: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::RemoveFromItems: " + e.ToString());
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

	   WriteStringIntoBinaryStream(fs.get(), QuestLibFile);

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

			if ((items[i]->Type == DlgSimpleAnsw) || (items[i]->Type == DlgAnsw))
			  {
				TDlgBaseAnswer *d = dynamic_cast<TDlgBaseAnswer*>(items[i]);
				bool chk = d->EndDialog;
				fs->Position += fs->Write(&chk, sizeof(bool));
			  }

			if (items[i]->Type == DlgAnsw)
			  {
				TDlgAnswer *d = dynamic_cast<TDlgAnswer*>(items[i]);

				WriteStringIntoBinaryStream(fs.get(), d->QuestName);
				WriteStringIntoBinaryStream(fs.get(), d->SetQuestValue);
				WriteStringIntoBinaryStream(fs.get(), d->NeedQuestValue);
			  }

			if (items[i]->Type == DlgCondition)
			  {
				TDlgCondition *d = dynamic_cast<TDlgCondition*>(items[i]);

				WriteStringIntoBinaryStream(fs.get(), d->Condition);
			  }
		  }

	   res = true;
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::SaveDlgSchema: " + e.ToString());
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

	   fs->Position += fs->Read(&text_len, sizeof(int));

	   if (text_len > 0)
		 QuestLibFile = ReadStringFromBinaryStream(fs.get(), text_len);

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
			   case DlgSimpleAnsw:
				 {
				   TDlgBaseAnswer *dl = new TDlgBaseAnswer(id,
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

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->QuestName = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->SetQuestValue = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->NeedQuestValue = ReadStringFromBinaryStream(fs.get(), text_len);

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
			   case DlgCondition:
				 {
				   TDlgCondition *dl = new TDlgCondition(id,
														 card_of_dialog,
														 next_card_of_dialog,
														 linked_id,
														 linked_from_id);

				   dl->Text = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->Condition = ReadStringFromBinaryStream(fs.get(), text_len);

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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::LoadDlgSchema: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::SearchDependeciesID: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::SearchDependeciesDialog: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::UpdateLinkedID: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::UpdateCardOfDialog: " + e.ToString());
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
	   _di_IXMLNode QuestName;
	   _di_IXMLNode EndDialog;
	   _di_IXMLNode QuestLib;

	   int curr_card = -1;
	   int i = 0;

	   if (DialogFile->ChildNodes->Nodes[i]->GetNodeName() == "QuestLibFile")
		 {
		   QuestLib = DialogFile->ChildNodes->Nodes[0];
		   QuestLibFile = QuestLib->Text;
		   i++;
		 }

	   for (i; i < DialogFile->ChildNodes->Count; i++)
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
						  int ncd, type;
						  bool end;
						  String setquest, needquest, quest, text, check;

						  if (Answer->ChildNodes->IndexOf("QuestName") >= 0)
							type = 1; //Answer
						  else if (Answer->ChildNodes->IndexOf("Check") >= 0)
							type = 2; //Condition
						  else
							type = 0; //BaseAnswer

						  if (Answer->HasAttribute("NextCardOfDialog"))
							ncd = StrToInt(Answer->GetAttribute("NextCardOfDialog"));

						  if (Answer->HasAttribute("NeedQuestValue"))
							needquest = Answer->GetAttribute("NeedQuestValue");

						  if (Answer->HasAttribute("SetQuestValue"))
							setquest = Answer->GetAttribute("SetQuestValue");

						  ind = Answer->ChildNodes->IndexOf("TextOfAnswer");

						  if (ind >= 0)
							text = Answer->ChildNodes->Nodes[ind]->Text;

						  ind = Answer->ChildNodes->IndexOf("Check");

						  if (ind >= 0)
							check = Answer->ChildNodes->Nodes[ind]->Text;

						  ind = Answer->ChildNodes->IndexOf("QuestName");

						  if (ind >= 0)
							quest = Answer->ChildNodes->Nodes[ind]->Text;

						  ind = Answer->ChildNodes->IndexOf("EndDialog");

						  if (ind < 0)
							end = false;
						  else if (Answer->ChildNodes->Nodes[ind]->Text == "True")
							end = true;
						  else
							end = false;

						  if (type == 1)
							{
							  TDlgAnswer *answ = new TDlgAnswer(GenElementID());

							  answ->CardOfDialog = curr_card;
							  answ->NextCardOfDialog = ncd;
							  answ->NeedQuestValue = needquest;
							  answ->SetQuestValue = setquest;
							  answ->QuestName = quest;
							  answ->EndDialog = end;
							  answ->Text = text;
							  items.push_back(answ);
							}
						  else if (type == 2)
							{
							  TDlgCondition *cond = new TDlgCondition(GenElementID());

							  cond->CardOfDialog = curr_card;
							  cond->NextCardOfDialog = ncd;
							  cond->Text = text;
							  cond->Condition = check;
							  items.push_back(cond);
							}
						  else
							{
							  TDlgBaseAnswer *answ = new TDlgBaseAnswer(GenElementID());

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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::XMLImport: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void KobzarEngine::XMLExport(const wchar_t *path)
{
  try
	 {
	   std::unique_ptr<TStringList> list(new TStringList());

	   String xml_exp = "<DialogFile>\r\n";

	   xml_exp += "\t<QuestLibFile>";
	   xml_exp += QuestLibFile;
	   xml_exp += "</QuestLibFile>\r\n";

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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::XMLExport: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::ClearItems: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::BuildLinksAfterXMLImport: " + e.ToString());
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
	   SaveLogToUserFolder("Engine.log", "Kobzar", "KobzarEngine::RemoveLimboLinks: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
