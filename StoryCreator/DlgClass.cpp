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

#include "..\..\work-functions\MyFunc.h"
#include "main.h"
#include "DlgClass.h"

extern std::vector<TDlgBaseText*> items;
extern TDlgBaseText *Selected;
extern int Choice;
extern TStoryCreator *StoryCreator;
extern TMenuItem *edt;
extern TPopupMenu *pp;
extern bool changed;
extern TStringGrid *pl;
extern String LogPath;
extern String QuestLibFile;

//---------------------------------------------------------------------------
#pragma package(smart_init)

int GenDialogID()
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
	   SaveLog(LogPath + "\\exceptions.log", "DlgClass::DialogID: " + e.ToString());
	   max = -2;
	 }

  return max + 1;
}
//---------------------------------------------------------------------------

int GenElementID()
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
	   SaveLog(LogPath + "\\exceptions.log", "DlgClass::ElementID: " + e.ToString());
	   max = -2;
	 }

  return max + 1;
}
//---------------------------------------------------------------------------

TDlgBaseText *FindElement(int id)
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
	   SaveLog(LogPath + "\\exceptions.log", "DlgClass::FindElement: " + e.ToString());
	   res = NULL;
	 }

  return res;
}
//---------------------------------------------------------------------------

int FindLinkedElements(int id, std::vector<TDlgBaseText*> *el_list)
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::FindLinkedElements: " + e.ToString());
	   cnt = -1;
	 }

  return cnt;
}
//---------------------------------------------------------------------------

int FindAnswersByDialog(int dlg_id, std::vector<TDlgBaseText*> *el_list)
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::FindAnswersByDialog: " + e.ToString());
	   cnt = -1;
	 }

  return cnt;
}
//---------------------------------------------------------------------------

int FindTextElementID(int crd_dlg)
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::FindTextElementID: " + e.ToString());
	   res = -1;
	 }

  return res;
}
//---------------------------------------------------------------------------

void RemoveFromItems(TDlgBaseText *element)
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

	   StoryCreator->Repaint();
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::RemoveFromItems: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void RedrawLinks()
{
  try
	 {
	   TCanvas *canv = StoryCreator->Canvas;
	   canv->Refresh();
	   canv->Pen->Style = psSolid;
	   canv->Pen->Width = 1;

	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->LinkedID > -1)
			  {
				canv->Pen->Color = clGreen;

				TDlgBaseText *lnk = FindElement(items[i]->LinkedID);

				if (lnk)
				  {
					canv->MoveTo(items[i]->Container->Left,
								 items[i]->Container->Top);
					canv->LineTo(lnk->Container->Left + lnk->Container->Width,
								 lnk->Container->Top + lnk->Container->Height);
				  }
			  }

			if (items[i]->LinkedFromID > -1)
			  {
				canv->Pen->Color = clBlue;

				TDlgBaseText *lnk = FindElement(items[i]->LinkedFromID);

				if (lnk)
				  {
					canv->MoveTo(items[i]->Container->Left + items[i]->Container->Width,
								 items[i]->Container->Top + items[i]->Container->Height);
					canv->LineTo(lnk->Container->Left, lnk->Container->Top);
				  }
			  }
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::RedrawLinks: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void AddScreenText(int left, int top, TBitmap *pic, TForm *IconOwner)
{
  try
	 {
	   TDlgScreenText *tmp = new TDlgScreenText(left, top, pic, IconOwner);
	   items.push_back(tmp);
	   changed = true;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::AddScreenText: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void AddAnswer(int left, int top, TBitmap *pic, TForm *IconOwner)
{
  try
	 {
	   TDlgAnswer *tmp = new TDlgAnswer(left, top, pic, IconOwner);
	   items.push_back(tmp);
	   changed = true;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::AddAnswer: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void AddScript(int left, int top, TBitmap *pic, TForm *IconOwner)
{
  try
	 {
	   TDlgScript *tmp = new TDlgScript(left, top, pic, IconOwner);
	   items.push_back(tmp);
	   changed = true;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::AddScript: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

bool SaveDlgSchema(const wchar_t *file)
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

			val = items[i]->Left;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->Top;
			fs->Position += fs->Write(&val, sizeof(int));

			WriteStringIntoBinaryStream(fs.get(), items[i]->Text);

			if (items[i]->Type == DlgScript)
			  {
				TDlgScript *d = dynamic_cast<TDlgScript*>(items[i]);

				WriteStringIntoBinaryStream(fs.get(), d->Params);
				WriteStringIntoBinaryStream(fs.get(), d->ResultParam);
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::SaveDlgSchema: " + e.ToString());
	   res = false;
	 }

  return res;
}
//---------------------------------------------------------------------------

bool LoadDlgSchema(const wchar_t *file)
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
				   TDlgScreenText *dl = new TDlgScreenText(left,
														   top,
														   id,
														   card_of_dialog,
														   StoryCreator);

				   if (text_len > 0)
					 dl->Text = ReadStringFromBinaryStream(fs.get(), text_len);

				   lnk = dl;

				   break;
				 }
			   case DlgAnsw:
				 {
				   TDlgAnswer *dl = new TDlgAnswer(left,
												   top,
												   id,
												   card_of_dialog,
												   next_card_of_dialog,
												   linked_id,
												   linked_from_id,
												   StoryCreator);

				   if (text_len > 0)
					 dl->Text = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&end_dlg, sizeof(bool));
				   dl->EndDialog = end_dlg;

				   lnk = dl;

				   break;
				 }
			   case DlgScript:
				 {
				   TDlgScript *dl = new TDlgScript(left,
												   top,
												   id,
												   card_of_dialog,
												   StoryCreator);

				   if (text_len > 0)
					 dl->Text = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->Params = ReadStringFromBinaryStream(fs.get(), text_len);

				   fs->Position += fs->Read(&text_len, sizeof(int));

				   if (text_len > 0)
					 dl->ResultParam = ReadStringFromBinaryStream(fs.get(), text_len);

				   lnk = dl;

				   break;
				 }
			 }

		   items.push_back(lnk);
		 }

	   StoryCreator->VisualiseElements();
	   StoryCreator->Repaint();

	   res = true;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::LoadDlgSchema: " + e.ToString());
	   res = false;
	 }

  return res;
}
//---------------------------------------------------------------------------

int SearchDependeciesID(int id)
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::SearchDependeciesID: " + e.ToString());
       dpnd = -1;
	 }

  return dpnd;
}
//---------------------------------------------------------------------------

int SearchDependeciesDialog(int id)
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::SearchDependeciesDialog: " + e.ToString());
       dpnd = -1;
	 }

  return dpnd;
}
//---------------------------------------------------------------------------

void UpdateLinkedID(int old_id, int new_id)
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::UpdateLinkedID: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void UpdateCardOfDialog(int old_val, int new_val)
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::UpdateCardOfDialog: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void XMLImport(String xml_file)
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

	   int scene_left = StoryCreator->ItemList->Left + StoryCreator->ItemList->Width + 20;
	   int left = scene_left + 50, top = 50, step = 0, curr_card = -1;

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
						 TDlgScript *lnk = new TDlgScript(scene_left, top, StoryCreator);

						 lnk->CardOfDialog = curr_card;
						 lnk->Params = ScreenText->GetAttribute("Params");
						 lnk->Text = ScreenText->ChildNodes->Nodes[0]->Text;
						 lnk->ResultParam = ScreenText->ChildNodes->Nodes[1]->Text;
						 items.push_back(lnk);
					   }
					 else  //ScreenText
					   {
						 TDlgScreenText *lnk = new TDlgScreenText(scene_left, top, StoryCreator);

						 lnk->CardOfDialog = curr_card;
						 lnk->Text = ScreenText->ChildNodes->Nodes[0]->Text;
						 items.push_back(lnk);
					   }

					 left += 55;
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

						  TDlgAnswer *answ = new TDlgAnswer(left, top, StoryCreator);

						  answ->CardOfDialog = curr_card;
						  answ->NextCardOfDialog = ncd;
						  answ->Text = text;
						  answ->EndDialog = end;
						  items.push_back(answ);

//приблизительные отступы по высоте и ширине
						  left += 55;
						  step = 60;
						}

					 top += 60;
					 left = 180;
				  }
			   }
		  }

		StoryCreator->VisualiseElements();
		BuildLinksAfterXMLImport();
		StoryCreator->Repaint();
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::XMLImport: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void XMLExport(const wchar_t *path)
{
  try
	 {
       TStringList *list = new TStringList();

	   try
		  {
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
	   __finally {delete list;}
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::XMLExport: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void ClearItems()
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  delete items[i];

	   items.clear();
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::ClearItems: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void BuildLinksAfterXMLImport()
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::BuildLinksAfterXMLImport: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void RemoveLimboLinks()
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
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::RemoveLimboLinks: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TDlgBaseText::TDlgBaseText()
{
  id = GenElementID();
  this->left = 0;
  this->top = 0;
  cd = -1;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
  Container = NULL;
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int left, int top)
{
  id = GenElementID();
  this->left = left;
  this->top = top;
  cd = -1;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
  SetPos(left, top);
  Container = NULL;
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int left, int top, int el_id, TForm *ContainerOwner)
{
  id = el_id;
  this->left = left;
  this->top = top;
  cd = -1;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
  Container = CreateContainer(NULL, ContainerOwner);
  SetPos(left, top);
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int left, int top, int el_id, int dlg_id, TForm *ContainerOwner)
{
  id = el_id;
  this->left = left;
  this->top = top;
  cd = dlg_id;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
  Container = CreateContainer(NULL, ContainerOwner);
  SetPos(left, top);
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int left, int top, TForm *ContainerOwner)
{
  id = GenElementID();
  this->left = left;
  this->top = top;
  cd = -1;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
  Container = CreateContainer(NULL, ContainerOwner);
  SetPos(left, top);
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int left, int top, TBitmap *pic, TForm *ContainerOwner)
{
  id = GenElementID();
  this->left = left;
  this->top = top;
  cd = -1;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
  Container = CreateContainer(pic, ContainerOwner);
  SetPos(left, top);
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int left, int top, int el_id, int dlg_id, int next_dlg_id,
						   int link_id, int link_fr_id, TForm *ContainerOwner)
{
  id = el_id;
  this->left = left;
  this->top = top;
  cd = dlg_id;
  ncd = next_dlg_id;
  l_id = link_id;
  l_fr_id = link_fr_id;
  Container = CreateContainer(NULL, ContainerOwner);
  SetPos(left, top);
}
//---------------------------------------------------------------------------

TDlgBaseText::~TDlgBaseText()
{
  if (Container)
	{
	  delete Container;
	  Container = NULL;
	}
};
//---------------------------------------------------------------------------

void TDlgBaseText::SetPos(int left, int top)
{
  Left = left;
  Top = top;
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetLeft()
{
  return left;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetLeft(int val)
{
  left = val;
  InLink.X = left;

  if (Container)
	Container->Left = left;
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetTop()
{
  return top;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetTop(int val)
{
  top = val;
  InLink.Y = top;

  if (Container)
	Container->Top = top;
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetID()
{
  return id;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetID(int val)
{
  int old_id = id;
  id = val;

  if (SearchDependeciesID(old_id) > 0)
	{
	  if (MessageBox(Application->Handle,
					 L"Founded linked elements.\nRebuild links?",
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes)
		{
		  UpdateLinkedID(old_id, id);
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetLinkedID()
{
  return l_id;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetLinkedID(int val)
{
  if (Cathegory == DLG_TEXT_LIKE)
	return;

  TDlgBaseText *lnk = FindElement(val);

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

int TDlgBaseText::GetLinkedFromID()
{
  return l_fr_id;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetLinkedFromID(int val)
{
  if (Cathegory == DLG_TEXT_LIKE)
	return;

  TDlgBaseText *lnk = FindElement(val);

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

int TDlgBaseText::GetCardOfDialog()
{
  return cd;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetCardOfDialog(int val)
{
  int old = cd;

  if (Cathegory != DLG_TEXT_LIKE)
	{
	  cd = val;
	  LinkedID = FindTextElementID(val);
	}
  else
	{
	  cd = val;
	  std::vector<TDlgBaseText*> lnks;

	  if (FindAnswersByDialog(cd, &lnks) > 0)
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

	  if ((old != cd) && (SearchDependeciesDialog(old) > 0))
		{
          if (MessageBox(Application->Handle,
						 String("Founded links of old ANSW_LIKE elements.\nRebuild this links?").c_str(),
						 String("Element ID: " + IntToStr(ID) + " report").c_str(),
						 MB_YESNO | MB_ICONASTERISK) == mrYes)
			{
			  UpdateCardOfDialog(old, CardOfDialog);
			}
		  else
			{
			  UpdateLinkedID(ID, -1);
			}
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetNextCardOfDialog()
{
  return ncd;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetNextCardOfDialog(int val)
{
  if (Cathegory != DLG_TEXT_LIKE)
	{
	  ncd = val;
	  int new_dlg_id = FindTextElementID(ncd);

	  if (new_dlg_id > -1)
		{
		  TDlgBaseText *ndlg = FindElement(new_dlg_id);
		  ndlg->LinkedID = ID;
		  LinkedFromID = ndlg->ID;
		}
	}
}
//---------------------------------------------------------------------------

const wchar_t *TDlgBaseText::GetInfo()
{
  String res;

  TStringList *lst = new TStringList();

  try
	 {
	   lst->Add("ID = " + String(ID));
	   lst->Add("LinkedID = " + String(LinkedID));
	   lst->Add("LinkedFromID = " + String(LinkedFromID));
	   lst->Add("CardOfDialog = " + String(CardOfDialog));
	   lst->Add("NextCardOfDialog = " + String(NextCardOfDialog));

	   res = lst->Text;
	 }
  __finally {delete lst;}

  res.Delete(res.Length() - 1, 2);

  return res.c_str();
}
//---------------------------------------------------------------------------

void TDlgBaseText::GiveInfo(TStrings *lst)
{
  lst->Add(String(ID));
  lst->Add(String(LinkedID));
  lst->Add(String(LinkedFromID));
  lst->Add(String(CardOfDialog));
  lst->Add(String(NextCardOfDialog));
  lst->Add(String(Left));
  lst->Add(String(Top));
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetCathegory()
{
  int res;

  switch (Type)
	{
	  case DlgText: res = DLG_TEXT_LIKE; break;
	  case DlgScript: res = DLG_TEXT_LIKE; break;
	  case DlgAnsw: res = DLG_ANSW_LIKE; break;
	}

  return res;
}
//---------------------------------------------------------------------------

void __fastcall TDlgBaseText::ContainerDblClick(TObject *Sender)
{
  Selected = this;
  edt->Click();
}
//---------------------------------------------------------------------------

void __fastcall TDlgBaseText::ContainerClick(TObject *Sender)
{
  if (Selected && (Selected != this))
	{
	  if ((Selected->Cathegory == DLG_ANSW_LIKE) &&
		  (this->Cathegory == DLG_TEXT_LIKE))
		{
		  if (this->LinkedID > -1) //если ScreenText уже привязан к Answer'у
			{
			  TDlgBaseText *old_answ = FindElement(this->LinkedID);

			  if (old_answ) //найдем Answer и изменим привязки
				{
				  old_answ->NextCardOfDialog = -1;
				  old_answ->LinkedFromID = -1;
				}
			  else
			    throw Exception("Неверный ID в стеке items!");
			}

		  Selected->NextCardOfDialog = this->CardOfDialog;
		  Selected->LinkedFromID = this->ID;

          if (Selected->Type == DlgAnsw)
			{
			  if (this->CardOfDialog != 0)
				dynamic_cast<TDlgAnswer*>(Selected)->EndDialog = false;
			  else
				dynamic_cast<TDlgAnswer*>(Selected)->EndDialog = true;
			}
		}
	  else if ((Selected->Cathegory == DLG_TEXT_LIKE) &&
			   (this->Cathegory == DLG_ANSW_LIKE))
		{
		  this->CardOfDialog = Selected->CardOfDialog;
		  this->LinkedID = Selected->ID;
		}

	  StoryCreator->Repaint();

	  Selected = NULL;
	  Choice = ActNone;
	}
  else if (!Selected)
	{
	  Selected = this;
	  Choice = SelectText;

	  pl->Cols[1]->Clear();
	  this->GiveInfo(pl->Cols[1]);
	}
  else
	{
	  Choice = SelectText;

	  pl->Cols[1]->Clear();
	  this->GiveInfo(pl->Cols[1]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TDlgBaseText::ContainerMouseEnter(TObject *Sender)
{
  TBitBtn *clicked = dynamic_cast<TBitBtn*>(Sender);

  clicked->Hint = GetInfo();
  clicked->ShowHint = true;
}
//---------------------------------------------------------------------------

TBitBtn *TDlgBaseText::CreateContainer(TBitmap *pic, TForm *owner)
{
  TBitBtn *res = new TBitBtn(owner);

  res->Parent = owner;
  res->DoubleBuffered = true;

  res->Glyph = pic;
  //res->Glyph->Assign(pic);
  res->Width = res->Glyph->Width + 6;
  res->Height = res->Glyph->Height + 6;
  res->Top = Top;
  res->Left = Left;
  res->Tag = ID;
  res->OnClick = ContainerClick;
  res->OnDblClick = ContainerDblClick;
  res->OnMouseEnter = ContainerMouseEnter;
  res->OnKeyUp = StoryCreator->ContainerKeyUp;
  res->OnMouseDown = StoryCreator->ContainerMouseDown;
  res->OnMouseUp = StoryCreator->ContainerMouseUp;
  res->OnMouseMove = StoryCreator->ContainerMouseMove;
  res->PopupMenu = pp;
  res->Show();

  return res;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetContainerImage(TBitmap *pic)
{
  Container->Glyph = pic;
  Container->Left = Left;
  Container->Top = Top;
  Container->Width = Container->Glyph->Width + 6;
  Container->Height = Container->Glyph->Height + 6;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgScreenText::CreateXML()
{
  XMLText = "\t\t<ScreenTextMassive>\r\n\t\t\t<ScreenText>\r\n";
  XMLText = XMLText + "\t\t\t\t<Text>" + Text + "</Text>\r\n";
  XMLText = XMLText + "\t\t\t</ScreenText>\r\n\t\t</ScreenTextMassive>\r\n";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgAnswer::CreateXML()
{
  XMLText = "\t\t\t<Answer NextCardOfDialog = '" + String(NextCardOfDialog) + "'>\r\n";
  XMLText = XMLText + "\t\t\t\t<TextOfAnswer>" + Text + "</TextOfAnswer>\r\n";

  if (EndDialog)
	XMLText = XMLText + "\t\t\t\t<EndDialog>True</EndDialog>\r\n";

  XMLText = XMLText + "\t\t\t</Answer>";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

bool TDlgAnswer::GetEndDialog()
{
  return end_dlg;
}
//---------------------------------------------------------------------------

void TDlgAnswer::SetEndDialog(bool val)
{
  end_dlg = val;

  if (end_dlg)
	{
	  if (LinkedFromID > -1)
		{
		  TDlgBaseText *lnk = FindElement(LinkedFromID);

		  if (lnk)
		  	lnk->LinkedID = -1;
		}

	  PrevLinkedFromID = LinkedFromID;
	  LinkedFromID = FindTextElementID(0);
	  NextCardOfDialog = 0;
	}
  else
	{
	  if (PrevLinkedFromID == -1)
        return;

	  TDlgBaseText *lnk = FindElement(PrevLinkedFromID);

	  if (lnk && (lnk->Cathegory == DLG_TEXT_LIKE))
		{
		  NextCardOfDialog = lnk->CardOfDialog;
		  LinkedFromID = lnk->ID;
		}
	  else
		{
		  NextCardOfDialog = -1;
		  LinkedFromID = -1;
		}
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgScript::CreateXML()
{
  XMLText = "\t\t<ScreenTextMassive>\r\n";
  XMLText = XMLText + "\t\t\t<Script Params = '" + Params + "'>\r\n";
  XMLText = XMLText + "\t\t\t\t<Text>" + Text + "</Text>\r\n";
  XMLText = XMLText + "\t\t\t\t<Return>" + ResultParam + "</Return>\r\n";
  XMLText = XMLText + "\t\t\t</Script>\r\n";
  XMLText = XMLText + "\t\t</ScreenTextMassive>\r\n";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

