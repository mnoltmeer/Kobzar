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
extern bool NoWarningsAtImport;

//---------------------------------------------------------------------------
#pragma package(smart_init)

int GenDialogID()
{
  int max = -1;

  try
	 {
	   for (int i = 0; i < items.size(); i++)
		 {
		   if (items[i]->Dialog > max)
			 max = items[i]->Dialog;
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
  int max = 0;

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
	   max = -1;
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
		   if (items[i]->PrevID == id)
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
		   if ((items[i]->Dialog == dlg_id) &&
			   (items[i]->Type != DlgText))
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
		   if ((items[i]->Dialog == crd_dlg) &&
			   (items[i]->Type == DlgText))
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
			if (items[i]->PrevID > -1)
			  {
				canv->Pen->Color = clGreen;

				TDlgBaseText *lnk = FindElement(items[i]->PrevID);

				if (lnk)
				  {
					canv->MoveTo(items[i]->Container->Left,
								 items[i]->Container->Top);
					canv->LineTo(lnk->Container->Left + lnk->Container->Width,
								 lnk->Container->Top + lnk->Container->Height);
				  }
			  }

			if (items[i]->NextID > -1)
			  {
				canv->Pen->Color = clBlue;

				TDlgBaseText *lnk = FindElement(items[i]->NextID);

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

void UpdateContainers()
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  items[i]->SetContainerData();
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::UpdateContainers: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void AddScreenText(int left, int top, TForm *IconOwner)
{
  try
	 {
	   TDlgScreenText *tmp = new TDlgScreenText(left, top, IconOwner);
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

void AddAnswer(int left, int top, TForm *IconOwner)
{
  try
	 {
	   TDlgAnswer *tmp = new TDlgAnswer(left, top, IconOwner);
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

void AddScript(int left, int top, TForm *IconOwner)
{
  try
	 {
	   TDlgScript *tmp = new TDlgScript(left, top, IconOwner);
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

			val = items[i]->PrevID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->NextID;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->Dialog;
			fs->Position += fs->Write(&val, sizeof(int));

			val = items[i]->NextDialog;
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
	   NoWarningsAtImport = true;

	   std::unique_ptr<TFileStream> fs(new TFileStream(file, fmOpenRead));

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
				   TDlgScreenText *dl = new TDlgScreenText(left,
														   top,
														   id,
														   card_of_dialog,
														   StoryCreator);

				   dl->Text = ReadStringFromBinaryStream(fs.get());

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

				   dl->Text = ReadStringFromBinaryStream(fs.get());

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
												   next_card_of_dialog,
												   linked_id,
												   linked_from_id,
												   StoryCreator);

				   dl->Text = ReadStringFromBinaryStream(fs.get());
				   dl->Params = ReadStringFromBinaryStream(fs.get());

				   lnk = dl;

				   break;
				 }
			 }

		   items.push_back(lnk);
		 }

	   StoryCreator->VisualiseElements();
	   StoryCreator->Repaint();
	   NoWarningsAtImport = false;

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
			if ((items[i]->PrevID == id) || (items[i]->NextID == id))
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
			if (items[i]->Dialog == id)
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

void UpdatePrevID(int old_id, int new_id)
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->PrevID == old_id)
			  items[i]->PrevID = new_id;

			if (items[i]->NextID == old_id)
			  items[i]->NextID = new_id;
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::UpdatePrevID: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void UpdateDialog(int old_val, int new_val)
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  {
			if (items[i]->Dialog == old_val)
			  items[i]->Dialog = new_val;
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::UpdateDialog: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void XMLImport(String xml_file)
{
  try
	 {
	   NoWarningsAtImport = true;

	   std::unique_ptr<TXMLDocument> ixml(new TXMLDocument(Application));

	   ClearItems();

	   ixml->DOMVendor = GetDOMVendor("MSXML");
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

	   int scene_left = StoryCreator->ItemList->Left + StoryCreator->ItemList->Width + 20;
	   int left = scene_left + 80, top = 50, step = 0, curr_card = -1;

	   for (int i = 0; i < DialogFile->ChildNodes->Count; i++)
		  {
			Dialog = DialogFile->ChildNodes->Nodes[i];

			for (int j = 0; j < Dialog->ChildNodes->Count; j++)
			   {
				 if (Dialog->ChildNodes->Nodes[j]->GetNodeName() == "ScreenText")
				   {
					 ScreenText = Dialog->ChildNodes->Nodes[j];
					 curr_card = StrToInt(Dialog->GetAttribute("id"));

					 TDlgScreenText *lnk = new TDlgScreenText(scene_left, top, StoryCreator);

						 lnk->Dialog = curr_card;
						 lnk->Text = ScreenText->Text;
						 items.push_back(lnk);

					 left = left + CONTAINER_WIDTH + 10;
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
							  TDlgScript *lnk = new TDlgScript(left, top, StoryCreator);

							  lnk->Dialog = curr_card;
							  lnk->Params = Answer->GetAttribute("Params");
                              lnk->NextDialog = -1;
							  lnk->Text = text;
							  items.push_back(lnk);
							}
						  else //Answer
							{
							  TDlgAnswer *answ = new TDlgAnswer(left, top, StoryCreator);

							  answ->Dialog = curr_card;
							  answ->NextDialog = ncd;
							  answ->Text = text;
							  answ->EndDialog = end;
							  items.push_back(answ);
							}

//приблизительные отступы по высоте и ширине
						  left = left + CONTAINER_WIDTH + 10;
						  step = 60;
						}

					 top = top + CONTAINER_HEIGHT + 10;
					 //scene_left += 80;
				  }

				 left = scene_left + CONTAINER_WIDTH + 10;
			   }
		  }

		StoryCreator->VisualiseElements();
		BuildLinksAfterXMLImport();
		StoryCreator->Repaint();
		NoWarningsAtImport = false;
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
			String xml_exp = "<StoryFile>\r\n";

			for (int i = 0; i < items.size(); i++)
			   {
				 if (items[i]->Type == DlgText)
				   {
					 xml_exp += "\t<Dialog id = '" +
								IntToStr(items[i]->Dialog) +
								"'>\r\n";
					 xml_exp += items[i]->CreateXML();
					 xml_exp += "\t\t<AnswersMassive>\r\n";

					 for (int j = 0; j < items.size(); j++)
						{
						  if (items[j]->PrevID == items[i]->ID)
							{
							  xml_exp += items[j]->CreateXML();
							  xml_exp += "\r\n";
							}
						}

					 xml_exp += "\t\t</AnswersMassive>\r\n";
					 xml_exp += "\t</Dialog>\r\n";
				   }
			   }

			xml_exp += "</StoryFile>\r\n";

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
			if (items[i]->Type != DlgText)
			  {
				items[i]->PrevID = FindTextElementID(items[i]->Dialog);
				items[i]->NextID = FindTextElementID(items[i]->NextDialog);
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
			if ((items[i]->PrevID > -1) && (!FindElement(items[i]->PrevID)))
			  items[i]->PrevID = -1;

			if ((items[i]->NextID > -1) && (!FindElement(items[i]->NextID)))
			  items[i]->NextID = -1;
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::RemoveLimboLinks: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

String CreateContainerCaption(TDlgBaseText *element)
{
  String res = "";

  try
	 {
	   res += "{" + IntToStr(element->ID) + "}";

	   switch (element->Type)
		 {
		   case DlgText: res += " <Scene>"; break;
		   case DlgAnsw: res += " <Answer>"; break;
		   case DlgScript: res += " <Script>"; break;
		   default: res = "<unknown>";
		 }

	   res += " [Dialog: " + IntToStr(element->Dialog) + "]";

	   if ((element->Type == DlgAnsw) && reinterpret_cast<TDlgAnswer*>(element)->EndDialog)
		 res += " (END)";
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "DlgClass::CreateContainerCaption: " + e.ToString());
	 }

  return res;
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
  Container = CreateContainer(ContainerOwner);
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
  Container = CreateContainer(ContainerOwner);
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
  Container = CreateContainer(ContainerOwner);
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
  Container = CreateContainer(ContainerOwner);
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
	  if (NoWarningsAtImport ||
		  (MessageBox(Application->Handle,
					 L"Founded linked elements.\nRebuild links?",
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes))
		{
		  UpdatePrevID(old_id, id);
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetPrevID()
{
  return l_id;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetPrevID(int val)
{
  if (Type == DlgText)
	return;

  TDlgBaseText *lnk = FindElement(val);

  if (lnk && (lnk->Type == DlgText))
	{
	  l_id = val;
	  cd = lnk->Dialog;
	}
  else if (lnk && (lnk->Type != DlgText))
	{
	  MessageBox(Application->Handle,
				 String("No TEXT_LIKE element with ID = PrevID (" +
				 IntToStr(val) + ")").c_str(),
				 String("Element ID: " + IntToStr(ID) + " report").c_str(),
				 MB_OK | MB_ICONERROR);
	}
  else
	{
	  if (NoWarningsAtImport ||
		  (MessageBox(Application->Handle,
					 String("No TEXT_LIKE element with ID = PrevID (" +
					 IntToStr(val) +
					 ").\nSet value anyway?").c_str(),
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes))
		{
		  l_id = val;
          cd = -1;
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetNextID()
{
  return l_fr_id;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetNextID(int val)
{
  if (Type == DlgText)
	return;

  TDlgBaseText *lnk = FindElement(val);

  if (lnk && (lnk->Type == DlgText))
	{
	  l_fr_id = val;
	  ncd = lnk->Dialog;
	}
  else if (lnk && (lnk->Type != DlgText))
	{
	  MessageBox(Application->Handle,
				 String("Element with ID = NextID (" +
				 IntToStr(val) + ") is not TEXT_LIKE").c_str(),
				 String("Element ID: " + IntToStr(ID) + " report").c_str(),
				 MB_OK | MB_ICONERROR);
	}
  else
	{
	  if (NoWarningsAtImport ||
		  (MessageBox(Application->Handle,
					 String("No TEXT_LIKE element with ID = NextID (" +
							IntToStr(val) +
							").\nSet value anyway?").c_str(),
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes))
		{
		  l_fr_id = val;
          ncd = -1;
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetDialog()
{
  return cd;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetDialog(int val)
{
  int old = cd;

  if (Type != DlgText)
	{
	  cd = val;
	  PrevID = FindTextElementID(val);
	}
  else
	{
	  cd = val;
	  std::vector<TDlgBaseText*> lnks;

	  if (FindAnswersByDialog(cd, &lnks) > 0)
		{
		  if (NoWarningsAtImport ||
			  (MessageBox(Application->Handle,
						 String("Founded ANSW_LIKE elements with Dialog = " +
								IntToStr(cd) +
								".\nCreate links?").c_str(),
						 String("Element ID: " + IntToStr(ID) + " report").c_str(),
						 MB_YESNO | MB_ICONASTERISK) == mrYes))
			{
			  for (int i = 0; i < lnks.size(); i++)
				 lnks[i]->PrevID = ID;
			}
		}

	  if ((old != cd) && (SearchDependeciesDialog(old) > 0))
		{
		  if (NoWarningsAtImport ||
			  (MessageBox(Application->Handle,
						 String("Founded links of old ANSW_LIKE elements.\nRebuild this links?").c_str(),
						 String("Element ID: " + IntToStr(ID) + " report").c_str(),
						 MB_YESNO | MB_ICONASTERISK) == mrYes))
			{
			  UpdateDialog(old, Dialog);
			}
		  else
			{
			  UpdatePrevID(ID, -1);
			}
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetNextDialog()
{
  return ncd;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetNextDialog(int val)
{
  if (Type != DlgText)
	{
	  ncd = val;
	  int new_dlg_id = FindTextElementID(ncd);

	  if (new_dlg_id > -1)
		{
		  TDlgBaseText *ndlg = FindElement(new_dlg_id);
		  ndlg->PrevID = ID;
		  NextID = ndlg->ID;
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
	   lst->Add("PrevID = " + String(PrevID));
	   lst->Add("NextID = " + String(NextID));
	   lst->Add("Dialog = " + String(Dialog));
	   lst->Add("NextDialog = " + String(NextDialog));

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
  lst->Add(String(PrevID));
  lst->Add(String(NextID));
  lst->Add(String(Dialog));
  lst->Add(String(NextDialog));
  lst->Add(String(Left));
  lst->Add(String(Top));
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
	  if ((Selected->Type == DlgAnsw) && (this->Type == DlgText))
		{
		  if (this->PrevID > -1) //если ScreenText уже привязан к Answer'у
			{
			  TDlgBaseText *old_answ = FindElement(this->PrevID);

			  if (old_answ) //найдем Answer и изменим привязки
				{
				  old_answ->NextDialog = -1;
				  old_answ->NextID = -1;
				}
			}

		  Selected->NextDialog = this->Dialog;
		  Selected->NextID = this->ID;

		  if (reinterpret_cast<TDlgAnswer*>(Selected)->EndDialog)
            reinterpret_cast<TDlgAnswer*>(Selected)->EndDialog = false;
		}
	  else if ((Selected->Type == DlgText) && ((this->Type == DlgAnsw) || (this->Type == DlgScript)))
		{
		  this->Dialog = Selected->Dialog;
		  this->PrevID = Selected->ID;
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

	  TLabel *clicked = dynamic_cast<TLabel*>(Sender);
	  clicked->Color = clYellow;
	}
  else
	{
	  Choice = SelectText;

	  pl->Cols[1]->Clear();
	  this->GiveInfo(pl->Cols[1]);

      TLabel *clicked = dynamic_cast<TLabel*>(Sender);
	  clicked->Color = clYellow;
	}
}
//---------------------------------------------------------------------------

void __fastcall TDlgBaseText::ContainerMouseEnter(TObject *Sender)
{
  TLabel *clicked = dynamic_cast<TLabel*>(Sender);

  clicked->Hint = GetInfo();
  clicked->ShowHint = true;
}
//---------------------------------------------------------------------------

TLabel *TDlgBaseText::CreateContainer(TForm *owner)
{
  TLabel *res = new TLabel(owner);

  res->Parent = owner;
  res->ParentColor = false;
  res->AutoSize = false;
  res->WordWrap = true;
  res->Transparent = false;
  res->Cursor = crArrow;
  res->Width = CONTAINER_WIDTH;
  res->Height = CONTAINER_HEIGHT;
  res->Top = Top;
  res->Left = Left;
  res->Tag = ID;
  res->OnClick = ContainerClick;
  res->OnDblClick = ContainerDblClick;
  res->OnMouseEnter = ContainerMouseEnter;
  res->OnMouseDown = StoryCreator->ContainerMouseDown;
  res->OnMouseUp = StoryCreator->ContainerMouseUp;
  res->OnMouseMove = StoryCreator->ContainerMouseMove;
  res->PopupMenu = pp;
  res->Show();

  return res;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetContainerData()
{
  if (Container)
	{
      switch (Type)
		{
		  case DlgText: Container->Color = (TColor)TEXT_COLOR; break;
		  case DlgAnsw: Container->Color = (TColor)ANSW_COLOR; break;
		  case DlgScript: Container->Color = (TColor)SCRIPT_COLOR; break;
		}

	  String text;

	  if (Text.Length() >= 50)
		text = Text.SubString(1, 50) + "...";
	  else
		text = Text;

	  Container->Caption = CreateContainerCaption(this) + "\r\n\r\n" + text;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
const wchar_t *TDlgScreenText::CreateXML()
{
  XMLText = "\t\t<ScreenText>" + Text + "</ScreenText>\r\n";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgAnswer::CreateXML()
{
  XMLText = "\t\t\t<Answer NextDialog = '" + String(NextDialog) + "'>\r\n";
  XMLText = XMLText + "\t\t\t\t<Text>" + Text + "</Text>\r\n";

  if (EndDialog)
	XMLText = XMLText + "\t\t\t\t<EndDialog>True</EndDialog>\r\n";

  XMLText = XMLText + "\t\t\t</Answer>";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgScript::CreateXML()
{
  XMLText = "\t\t\t<Script Params = '" + Params + "'>\r\n";
  XMLText = XMLText + "\t\t\t\t<Text>" + Text + "</Text>\r\n";
  XMLText = XMLText + "\t\t\t</Script>";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

