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

#include <vcl.h>
#pragma hdrstop

#include "..\..\work-functions\MyFunc.h"
#include "ELISourceHighlighter.h"
#include "DlgClass.h"
#include "settings.h"
#include "EditText.h"
#include "EditAnswer.h"
#include "EditScript.h"
#include "Splash.h"
#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStoryCreator *StoryCreator;

bool FullScreen;
String FontName;
int FontSize;
bool SyntaxHighlight;
String AppDir, LogPath;

String Version;
String Title;
TStringList *LastFiles;
TMenuItem *MenuLastFiles[5];

String cur_proj_path;
TDlgBaseText *Selected;     //элемент на форме, который активен на данный момент
int Choice;                 //флаг нажатой менюшки
bool changed;               //флаг показывающий внесены ли изменения в схему
std::vector<TDlgBaseText*> items;
TMenuItem *edt;
TPopupMenu *pp;
TStringGrid *pl;
TXMLDocument *ixml;

int MouseX, MouseY;
bool drag;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TStoryCreator::TStoryCreator(TComponent* Owner)
	: TForm(Owner)
{
  LastFiles = new TStringList();

  Title = "Kobzar Story Creator ";
  Version = GetVersionInString(Application->ExeName.c_str());

  Caption = Title + Version + ": <new>";

  LogPath = GetEnvironmentVariable("USERPROFILE") + "\\Documents";

  AppDir = Application->ExeName;
  int pos = AppDir.LastDelimiter("\\");
  AppDir.Delete(pos, AppDir.Length() - (pos - 1));

  ReadSettings();
  CreateMenuLastFiles();

  edt = this->MenuEdit;
  pp = this->ElementPopupMenu;

  cur_proj_path = "";
  changed = false;
  SaveSchema->InitialDir = GetCurrentDir();
  OpenSchema->InitialDir = GetCurrentDir();

  pl = PropList;

  if (FullScreen)
	WindowState = wsMaximized;
  else
	WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormCreate(TObject *Sender)
{
  BtUnselect->Glyph = Images->GetBitmap(0, 24, 24);
  BtUnselect->Glyph->Transparent = true;
  BtScreenText->Glyph = Images->GetBitmap(DlgText, 24, 24);
  BtScreenText->Glyph->Transparent = true;
  BtAnswer->Glyph = Images->GetBitmap(DlgAnsw, 24, 24);
  BtAnswer->Glyph->Transparent = true;
  BtScript->Glyph = Images->GetBitmap(DlgScript, 24, 24);
  BtScript->Glyph->Transparent = true;

  MenuPlaceText->Bitmap = Images->GetBitmap(DlgText, 16, 16);
  MenuPlaceText->Bitmap->TransparentColor = clBlack;
  MenuPlaceText->Bitmap->Transparent = true;
  MenuPlaceAnswer->Bitmap = Images->GetBitmap(DlgAnsw, 16, 16);
  MenuPlaceAnswer->Bitmap->TransparentColor = clBlack;
  MenuPlaceAnswer->Bitmap->Transparent = true;
  MenuPlaceScript->Bitmap = Images->GetBitmap(DlgScript, 16, 16);
  MenuPlaceScript->Bitmap->TransparentColor = clBlack;
  MenuPlaceScript->Bitmap->Transparent = true;

  MPPScrTxt->Bitmap = Images->GetBitmap(DlgText, 16, 16);
  MPPScrTxt->Bitmap->TransparentColor = clBlack;
  MPPScrTxt->Bitmap->Transparent = true;
  MPPAnsw->Bitmap = Images->GetBitmap(DlgAnsw, 16, 16);
  MPPAnsw->Bitmap->TransparentColor = clBlack;
  MPPAnsw->Bitmap->Transparent = true;
  MPPScript->Bitmap = Images->GetBitmap(DlgScript, 16, 16);
  MPPScript->Bitmap->TransparentColor = clBlack;
  MPPScript->Bitmap->Transparent = true;

  QBtNew->Glyph = Images->GetBitmap(4, 22, 22);
  QBtNew->Glyph->TransparentColor = clBlack;
  QBtNew->Glyph->Transparent = true;
  QBtOpen->Glyph = Images->GetBitmap(5, 24, 24);
  QBtOpen->Glyph->TransparentColor = clBlack;
  QBtOpen->Glyph->Transparent = true;
  QBtSave->Glyph = Images->GetBitmap(6, 24, 24);
  QBtSave->Glyph->TransparentColor = clBlack;
  QBtSave->Glyph->Transparent = true;
  QBtImport->Glyph = Images->GetBitmap(7, 24, 24);
  QBtImport->Glyph->TransparentColor = clBlack;
  QBtImport->Glyph->Transparent = true;
  QBtExport->Glyph = Images->GetBitmap(8, 24, 24);
  QBtExport->Glyph->TransparentColor = clBlack;
  QBtExport->Glyph->Transparent = true;

  MenuNew->Bitmap = Images->GetBitmap(4, 16, 16);
  MenuNew->Bitmap->TransparentColor = clBlack;
  MenuNew->Bitmap->Transparent = true;
  MenuLoad->Bitmap = Images->GetBitmap(5, 16, 16);
  MenuLoad->Bitmap->TransparentColor = clBlack;
  MenuLoad->Bitmap->Transparent = true;
  MenuSave->Bitmap = Images->GetBitmap(6, 16, 16);
  MenuSave->Bitmap->TransparentColor = clBlack;
  MenuSave->Bitmap->Transparent = true;
  MenuSaveAs->Bitmap = Images->GetBitmap(6, 16, 16);
  MenuSaveAs->Bitmap->TransparentColor = clBlack;
  MenuSaveAs->Bitmap->Transparent = true;
  MenuImportXML->Bitmap = Images->GetBitmap(7, 16, 16);
  MenuImportXML->Bitmap->TransparentColor = clBlack;
  MenuImportXML->Bitmap->Transparent = true;
  MenuExportXML->Bitmap = Images->GetBitmap(8, 16, 16);
  MenuExportXML->Bitmap->TransparentColor = clBlack;
  MenuExportXML->Bitmap->Transparent = true;
  MenuExportText->Bitmap = Images->GetBitmap(9, 16, 16);
  MenuExportText->Bitmap->TransparentColor = clBlack;
  MenuExportText->Bitmap->Transparent = true;

  MenuEdit->Bitmap = Images->GetBitmap(11, 16, 16);
  MenuEdit->Bitmap->TransparentColor = clBlack;
  MenuEdit->Bitmap->Transparent = true;
  MenuDel->Bitmap = Images->GetBitmap(10, 16, 16);
  MenuDel->Bitmap->TransparentColor = clBlack;
  MenuDel->Bitmap->Transparent = true;

  EPPEdit->Bitmap = Images->GetBitmap(11, 16, 16);
  EPPEdit->Bitmap->TransparentColor = clBlack;
  EPPEdit->Bitmap->Transparent = true;
  EPPDel->Bitmap = Images->GetBitmap(10, 16, 16);
  EPPDel->Bitmap->TransparentColor = clBlack;
  EPPDel->Bitmap->Transparent = true;
  EPPUnsel->Bitmap = Images->GetBitmap(0, 16, 16);
  EPPUnsel->Bitmap->TransparentColor = clBlack;
  EPPUnsel->Bitmap->Transparent = true;
  EPPStandAlone->Bitmap = Images->GetBitmap(12, 16, 16);
  EPPStandAlone->Bitmap->TransparentColor = clBlack;
  EPPStandAlone->Bitmap->Transparent = true;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormShow(TObject *Sender)
{
  try
	 {
	   InitExprColors(ESH_LIGHT_THEME);

	   AppSettings->FontDialog->Font->Name = FontName;
       AppSettings->FontDialog->Font->Size = FontSize;
	   EditScriptForm->Text->Font->Assign(AppSettings->FontDialog->Font);
	   EditTextForm->Text->Font->Assign(AppSettings->FontDialog->Font);
	   EditAnswerForm->Text->Font->Assign(AppSettings->FontDialog->Font);

	   if (ParamStr(1) != "")
		 {
		   cur_proj_path = ParamStr(1);

		   String ext = cur_proj_path.SubString(cur_proj_path.Length() - 3, 4);

		   if (ext == ".scs")
			 LoadDlgSchema(cur_proj_path.c_str());
		   else if (ext == ".xml")
			 XMLImport(cur_proj_path);

		   Caption = Title + Version + ": <" + CreateProjName(cur_proj_path) + ">";
		   UpdateItemsList(ItemList);
		   changed = false;

		   AddLastFile(ParamStr(1));
		 }

       if (MenuToolPalette->Checked)
		 {
			Palette->Show();
			PreparePalette();
		 }
	   else
		 {
		   Palette->Hide();
		 }

	   if (MenuShowItems->Checked)
		 {
		   PropsPanel->Show();
		   UpdateItemsList(ItemList);
		 }
	   else
		 {
		   PropsPanel->Hide();
		 }

       if (MenuButtonPanel->Checked)
		 {
		   QuickButtonsPanel->Show();
		   PropsPanel->Top = QuickButtonsPanel->ClientHeight;
		   Palette->Top = QuickButtonsPanel->ClientHeight;
		 }
	   else
		 {
		   QuickButtonsPanel->Hide();
		   PropsPanel->Top = 0;
		   Palette->Top = 0;
		 }

	   PropList->ColWidths[0] = 110;
	   PropList->ColWidths[1] = 50;
	   PropList->Cols[0]->Add("ID");
	   PropList->Cols[0]->Add("LinkedID");
	   PropList->Cols[0]->Add("LinkedFromID");
	   PropList->Cols[0]->Add("CardOfDialog");
	   PropList->Cols[0]->Add("NextCardOfDialog");
	   PropList->Cols[0]->Add("Position X");
	   PropList->Cols[0]->Add("Position Y");
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "FormShow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ReadSettings()
{
  try
	 {
	   std::unique_ptr<TRegistry> reg(new TRegistry());

	   reg->RootKey = HKEY_CURRENT_USER;

	   if (reg->OpenKey("Software\\Kobzar\\TextSceneCreator\\Editor", false))
		 {
		   if (reg->ValueExists("FontName"))
			 FontName = reg->ReadString("FontName");
		   else
			 {
			   FontName = "Tahoma";
			   reg->WriteString("FontName", "Tahoma");
			 }

		   if (reg->ValueExists("FontSize"))
			 FontSize = reg->ReadInteger("FontSize");
		   else
			 {
			   FontSize = 10;
			   reg->WriteInteger("FontSize", 10);
			 }

		   if (reg->ValueExists("SyntaxHighlight"))
			 SyntaxHighlight = reg->ReadBool("SyntaxHighlight");
		   else
			 {
			   SyntaxHighlight = true;
			   reg->WriteBool("SyntaxHighlight", true);
			 }

		   reg->CloseKey();
		 }

	   if (reg->OpenKey("Software\\Kobzar\\StoryCreator\\UI", false))
		 {
		   if (reg->ValueExists("FullScreen"))
			 FullScreen = reg->ReadBool("FullScreen");

		   if (reg->ValueExists("ShowToolPalette"))
				  MenuToolPalette->Checked = reg->ReadBool("ShowToolPalette");

		   if (reg->ValueExists("ShowItems"))
				  MenuShowItems->Checked = reg->ReadBool("ShowItems");

		   if (reg->ValueExists("ShowQuickButtonsPanel"))
				  MenuButtonPanel->Checked = reg->ReadBool("ShowQuickButtonsPanel");

		   if (reg->ValueExists("LastFiles"))
				  ReadLastFilesList();
				else
				  reg->WriteString("LastFiles", "");

		   if (reg->ValueExists("FormHeight"))
				  ClientHeight = reg->ReadInteger("FormHeight");
				else
				  {
					ClientHeight = 600;
					reg->WriteInteger("FormHeight", 600);
				  }

				if (reg->ValueExists("FormWidth"))
				  ClientWidth = reg->ReadInteger("FormWidth");
				else
				  {
                    ClientWidth = 800;
					reg->WriteInteger("FormWidth", 800);
				  }

                reg->CloseKey();
			  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ReadSettings: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::WriteSettings()
{
  try
	 {
	   std::unique_ptr<TRegistry> reg(new TRegistry());

	   reg->RootKey = HKEY_CURRENT_USER;

	   if (!reg->KeyExists("Software\\Kobzar\\StoryCreator\\Editor"))
		 reg->CreateKey("Software\\Kobzar\\StoryCreator\\Editor");

	   if (!reg->KeyExists("Software\\StoryTeller\\StoryCreator\\UI"))
		 reg->CreateKey("Software\\StoryTeller\\StoryCreator\\UI");

	   if (reg->OpenKey("Software\\Kobzar\\StoryCreator\\Editor", false))
		 {
		   reg->WriteString("FontName", FontName);
		   reg->WriteInteger("FontSize", FontSize);
		   reg->WriteBool("SyntaxHighlight", SyntaxHighlight);

		   reg->CloseKey();
		 }

	   if (reg->OpenKey("Software\\Kobzar\\StoryCreator\\UI", false))
		 {
		   reg->WriteInteger("FormHeight", ClientHeight);
		   reg->WriteInteger("FormWidth", ClientWidth);

		   if (WindowState == wsMaximized)
			 reg->WriteBool("FullScreen", true);
		   else
			 reg->WriteBool("FullScreen", false);

		   reg->WriteBool("ShowItems", MenuShowItems->Checked);
		   reg->WriteBool("ShowToolPalette", MenuToolPalette->Checked);
		   reg->WriteBool("ShowQuickButtonsPanel", MenuButtonPanel->Checked);

		   reg->CloseKey();
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "WriteSettings: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::CreateMenuLastFiles()
{
  for (int i = 0; i < 5; i++)
	 {
	   MenuLastFiles[i] = new TMenuItem(MainMenu);
	   MainMenu->Items->Find("File")->Add(MenuLastFiles[i]);
	   MenuLastFiles[i]->Tag = i;
	   MenuLastFiles[i]->Visible = false;
	   MenuLastFiles[i]->OnClick = PPLastFileClick;
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::WriteLastFilesList()
{
  std::unique_ptr<TRegistry> reg(new TRegistry());

  try
	 {
	   reg->RootKey = HKEY_CURRENT_USER;

	   if (!reg->KeyExists("Software\\Kobzar\\StoryCreator\\UI"))
		 reg->CreateKey("Software\\Kobzar\\StoryCreator\\UI");

	   if (reg->OpenKey("Software\\Kobzar\\StoryCreator\\UI", false))
		 {
		   String list = ListToStr(LastFiles, ";");

		   reg->WriteString("LastFiles", list);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "WriteLastFilesList: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ReadLastFilesList()
{
  try
	 {
	   std::unique_ptr<TRegistry> reg(new TRegistry());

	   reg->RootKey = HKEY_CURRENT_USER;

	   if (reg->OpenKey("Software\\Kobzar\\StoryCreator\\UI", false))
		 StrToList(LastFiles, reg->ReadString("LastFiles"), ";");
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ReadLastFilesList: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::AddLastFile(String file)
{
  try
	 {
	   int exists_file_ind = LastFiles->IndexOf(file);

	   if (exists_file_ind > -1)
		 {
		   LastFiles->Delete(exists_file_ind);
		   LastFiles->Insert(0, file);
		 }
	   else
		 LastFiles->Insert(0, file);

	   while (LastFiles->Count > 5)
		 LastFiles->Delete(LastFiles->Count - 1);

	   WriteLastFilesList();
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "AddLastFile: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ShowLastFiles()
{
  ReadLastFilesList();

  try
	 {
	   for (int i = 0; i < LastFiles->Count; i++)
		  {
			MenuLastFiles[i]->Caption = LastFiles->Strings[i];
            MenuLastFiles[i]->Visible = true;
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ShowLastFiles: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::PPLastFileClick(TObject *Sender)
{
  try
	 {
	   TMenuItem *menu = dynamic_cast<TMenuItem*>(Sender);

	   SaveCurrentProject(true);

	   cur_proj_path = LastFiles->Strings[menu->Tag];

       String ext = cur_proj_path.SubString(cur_proj_path.Length() - 3, 4);

	   if (ext == ".scs")
		 LoadDlgSchema(cur_proj_path.c_str());
	   else if (ext == ".xml")
		 XMLImport(cur_proj_path);

	   Caption = Title + Version + ": <" + CreateProjName(cur_proj_path) + ">";
	   UpdateItemsList(ItemList);
       VisualiseElements();
	   changed = false;
	   AddLastFile(LastFiles->Strings[menu->Tag]);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "PPLastFileClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::VisualiseElements()
{
  try
	 {
	   for (int i = 0; i < items.size(); i++)
		  {
			if (!items[i]->Container)
			  items[i]->Container = items[i]->CreateContainer(Images->GetBitmap(items[i]->Type, 32, 32), this);
			else
			  items[i]->SetContainerImage(Images->GetBitmap(items[i]->Type, 32, 32));
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "VisualiseElements: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::SaveCurrentProject(bool confirm)
{
  if (confirm)
	{
	  if (MessageDlg("Save current project?",
					 mtWarning,
					 TMsgDlgButtons() <<mbYes <<mbNo, 0) == mrYes)
		{
		  MenuSave->Click();
		}
	}
  else
    MenuSave->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ScrTextClick(TObject *Sender)
{
  Choice = PlaceText;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::AnswerClick(TObject *Sender)
{
  Choice = PlaceAnswer;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::SimpleAnswerClick(TObject *Sender)
{
  Choice = PlaceAnswer;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuEditClick(TObject *Sender)
{
  try
	 {
	   if (Selected)
		 {
		   PropList->Cols[1]->Clear();
		   Selected->GiveInfo(PropList->Cols[1]);

		   switch (Selected->Type)
			 {
			   case DlgText: EditTextForm->Hide(); EditTextForm->Show(); break;
			   case DlgAnsw: EditAnswerForm->Hide(); EditAnswerForm->Show(); break;
			   case DlgScript: EditScriptForm->Hide(); EditScriptForm->Show(); break;
			 }
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "MenuEditClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuDelClick(TObject *Sender)
{
  if (Selected)
	{
	  if (MessageDlg("Remove element?",
		  mtConfirmation,
		  TMsgDlgButtons() <<mbOK <<mbCancel, 0) == mrOk)
		{
		  RemoveFromItems(Selected);
		  UpdateItemsList(ItemList);
		  Repaint();
		  changed = true;
		}
	}

  Selected = NULL;
  Choice = ActNone;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::PlaceFreeClick(TObject *Sender)
{
  Choice = ActNone;
  Selected = NULL;
  PropList->Cols[1]->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuExportXMLClick(TObject *Sender)
{
  SaveSchema->Filter = "XML docs|*.xml";
  SaveSchema->DefaultExt = "xml";
  SaveSchema->FileName = CreateProjName(cur_proj_path);

  if (SaveSchema->Execute())
	{
	  XMLExport(SaveSchema->FileName.c_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuImportXMLClick(TObject *Sender)
{
  OpenSchema->Filter = "XML docs|*.xml";
  OpenSchema->DefaultExt = "XML";
  OpenSchema->FileName = "";

  if (OpenSchema->Execute())
	{
	  SaveCurrentProject(true);
	  XMLImport(OpenSchema->FileName);
	  cur_proj_path = "";
	  Caption = Title + Version + ": <" + CreateProjName(cur_proj_path) + ">";
	  UpdateItemsList(ItemList);
	  changed = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuExportTextClick(TObject *Sender)
{
  SaveSchema->Filter = "Text files|*.txt";
  SaveSchema->DefaultExt = "txt";
  SaveSchema->FileName = "";

  if (SaveSchema->Execute())
	{
	  try
		 {
		   std::unique_ptr<TStringList> list(new TStringList());

		   String txt, type;

		   for (int i = 0; i < items.size(); i++)
			  {
				switch (items[i]->Type)
				  {
					case DlgText: type = "Text"; break;
					case DlgAnsw: type = "Answer"; break;
					case DlgScript: type = "Script"; break;
					default: type = "<unknown>"; break;
				  }

				txt += "[" + IntToStr(items[i]->ID) + "] " + type +
					   ":\r\n" + items[i]->Text + "\r\n\r\n";
			  }

		   list->Text = txt;
		   list->SaveToFile(SaveSchema->FileName, TEncoding::UTF8);
		 }
	   catch (Exception &e)
		 {
		   SaveLog(LogPath + "\\exceptions.log", "MenuExportTextClick: " + e.ToString());
		 }
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuSaveClick(TObject *Sender)
{
  if (cur_proj_path == "")
    MenuSaveAs->Click();
  else
	{
	  SaveDlgSchema(cur_proj_path.c_str());
      changed = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuSaveAsClick(TObject *Sender)
{
  SaveSchema->Filter = "Story Creator schemas|*.scs";
  SaveSchema->DefaultExt = "scs";
  SaveSchema->FileName = "";

  if (SaveSchema->Execute())
	{
	  SaveDlgSchema(SaveSchema->FileName.c_str());
	  cur_proj_path = SaveSchema->FileName;
	  Caption = Title + Version + ": <" + CreateProjName(cur_proj_path) + ">";
      changed = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuLoadClick(TObject *Sender)
{
  OpenSchema->Filter = "Story Creator schemas|*.scs";
  OpenSchema->DefaultExt = "scs";
  OpenSchema->FileName = "";

  if (OpenSchema->Execute())
	{
	  SaveCurrentProject(true);
	  LoadDlgSchema(OpenSchema->FileName.c_str());
	  cur_proj_path = OpenSchema->FileName;
	  Caption = Title + Version + ": <" + CreateProjName(cur_proj_path) + ">";
	  UpdateItemsList(ItemList);
      VisualiseElements();
	  changed = false;
	  AddLastFile(cur_proj_path);
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuNewClick(TObject *Sender)
{
  SaveCurrentProject(true);
  ClearItems();
  Repaint();
  cur_proj_path = "";
  Caption = Title + Version + ": <new>";
  UpdateItemsList(ItemList);
  changed = false;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormClose(TObject *Sender, TCloseAction &Action)
{
  if (changed)
	{
	  if (MessageDlg("Save current project?",
					 mtWarning,
					 TMsgDlgButtons() <<mbYes <<mbNo, 0) == mrYes)
		{
		  MenuSave->Click();
		}
	}

  WriteSettings();
  ClearItems();

  delete LastFiles;

  SplashForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormPaint(TObject *Sender)
{
  RedrawLinks();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormResize(TObject *Sender)
{
  PreparePalette();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::PreparePalette()
{
  Palette->Left = ClientWidth - Palette->Width;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ChangeElement()
{
  try
	 {
	   if (Selected)
		 {
		   int cd = PropList->Cells[1][3].ToInt(),
			   ncd = PropList->Cells[1][4].ToInt(),
			   l_id = PropList->Cells[1][1].ToInt(),
			   l_fr_id = PropList->Cells[1][2].ToInt(),
			   id = PropList->Cells[1][0].ToInt(),
			   x = PropList->Cells[1][5].ToInt(),
			   y = PropList->Cells[1][6].ToInt();

		   ChangeID(id); //ID
           ChangeCardOfDialog(cd); //CardOfDialog
		   ChangeNextCardOfDialog(ncd); //NextCardOfDialog
           ChangeLinkedID(l_id); //LinkedID
		   ChangeLinkedFromID(l_fr_id); //LinkedFromID

		   if (Selected->InLink.X != x) //Pos X
			 Selected->Left = x;

		   if (Selected->InLink.Y != y) //Pos Y
			 Selected->Top = y;

		   UpdateItemsList(ItemList);
		   Repaint();
		   PropList->Cols[1]->Clear();
		   Selected->GiveInfo(PropList->Cols[1]);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ChangeElements: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ChangeID(int new_val)
{
  if (Selected->ID != new_val)
	{
	  Selected->ID = new_val;
	  changed = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ChangeCardOfDialog(int new_val)
{
  if (Selected->CardOfDialog != new_val)
	{
	  Selected->CardOfDialog = new_val;
	  changed = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ChangeNextCardOfDialog(int new_val)
{
  if (Selected->NextCardOfDialog != new_val)
	{
	  Selected->NextCardOfDialog = new_val;
	  changed = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ChangeLinkedID(int new_val)
{
  if (Selected->Cathegory == DLG_TEXT_LIKE)
	return;

  if (Selected->LinkedID != new_val)
	{
	  Selected->LinkedID = new_val;
	  changed = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ChangeLinkedFromID(int new_val)
{
  if (Selected->Cathegory == DLG_TEXT_LIKE)
	return;

  if (Selected->LinkedFromID != new_val)
	{
	  Selected->LinkedFromID = new_val;
	  changed = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MPPScrTxtClick(TObject *Sender)
{
  try
	 {
	   AddScreenText(MouseX, MouseY, Images->GetBitmap(DlgText, 32, 32), this);
	   UpdateItemsList(ItemList);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "MPPScrTxtClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MPPAnswClick(TObject *Sender)
{
  try
	 {
	   AddAnswer(MouseX, MouseY, Images->GetBitmap(DlgAnsw, 32, 32), this);
	   UpdateItemsList(ItemList);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "MPPAnswClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MPPScriptClick(TObject *Sender)
{
  try
	 {
	   AddScript(MouseX, MouseY, Images->GetBitmap(DlgScript, 32, 32), this);
	   UpdateItemsList(ItemList);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "MPPScriptClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::EPPEditClick(TObject *Sender)
{
  MenuEdit->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::EPPDelClick(TObject *Sender)
{
  MenuDel->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::EPPUnselClick(TObject *Sender)
{
  Selected = NULL;
  PropList->Cols[1]->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::EPPStandAloneClick(TObject *Sender)
{
  if (MessageDlg("Are you sure to make this element standalone? All links to element will be deleted",
				 mtWarning,
				 TMsgDlgButtons() <<mbYes <<mbNo, 0) == mrYes)
	{
	  ChangeLinkedID(-1);
	  ChangeLinkedFromID(-1);

      UpdateItemsList(ItemList);
	  Repaint();
	  PropList->Cols[1]->Clear();
	  Selected->GiveInfo(PropList->Cols[1]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuShowItemsClick(TObject *Sender)
{
  MenuShowItems->Checked = !MenuShowItems->Checked;

  if (MenuShowItems->Checked)
	{
	  PropsPanel->Show();
	  UpdateItemsList(ItemList);
	}
  else
	{
	  PropsPanel->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuButtonPanelClick(TObject *Sender)
{
  MenuButtonPanel->Checked = !MenuButtonPanel->Checked;

  if (MenuButtonPanel->Checked)
	{
	  QuickButtonsPanel->Show();
	  PropsPanel->Top = QuickButtonsPanel->ClientHeight;
	  Palette->Top = QuickButtonsPanel->ClientHeight;
	}
  else
	{
	  QuickButtonsPanel->Hide();
	  PropsPanel->Top = 0;
      Palette->Top = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuToolPaletteClick(TObject *Sender)
{
  MenuToolPalette->Checked = !MenuToolPalette->Checked;

  if (MenuToolPalette->Checked)
	{
	  Palette->Show();
	  PreparePalette();
	}
  else
	{
	  Palette->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuSettingsClick(TObject *Sender)
{
  AppSettings->Show();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuAboutClick(TObject *Sender)
{
  String text = "Version: " + GetVersionInString(Application->ExeName.c_str());

  text += "\r\n\r\nCopyright 2020 Maxim Noltmeer (m.noltmeer@gmail.com)\r\n\r\n\
This software is a part of StoryTeller Construction Set\r\n\r\n\
Kobzar Story Creator is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License\r\n\
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\r\n\
Kobzar Story Creator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty\r\n\
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\r\n\r\n\
You should have received a copy of the GNU General Public License along with Kobzar Story Creator. \
If not, see <http://www.gnu.org/licenses/>.\r\n\r\n\
Any questions, feedback and suggestions about this software are\
accepted by the Developer by email: m.noltmeer@gmail.com";

  MessageBox(Application->Handle, text.c_str(), L"About", MB_OK);
}
//---------------------------------------------------------------------------

void UpdateItemsList(TListBox *list)
{
  try
	 {
	   list->Clear();

	   String type;

	   for (int i = 0; i < items.size(); i++)
		  {
			switch (items[i]->Type)
			  {
				case DlgText: type = "ScreenText"; break;
				case DlgAnsw: type = "Answer"; break;
				case DlgScript: type = "Script"; break;
				default: type = "<unknown>";
			  }

			list->Items->Add(IntToStr(items[i]->ID) + ": " + type);
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "UpdateItemsList: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

String CreateProjName(String proj_path)
{
  String name = proj_path;
  int pos = name.LastDelimiter("\\");

  name.Delete(1, pos);
  pos = name.LastDelimiter(".");
  name.Delete(pos, 4);

  return name;
}
//---------------------------------------------------------------------------
void __fastcall TStoryCreator::MenuFileClick(TObject *Sender)
{
  ShowLastFiles();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::BtUnselectClick(TObject *Sender)
{
  Choice = ActNone;
  Selected = NULL;
  PropList->Cols[1]->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::BtScreenTextClick(TObject *Sender)
{
  Choice = PlaceText;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::BtAnswerClick(TObject *Sender)
{
  Choice = PlaceAnswer;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::BtScriptClick(TObject *Sender)
{
  Choice = PlaceScript;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuPlaceTextClick(TObject *Sender)
{
  Choice = PlaceText;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuPlaceAnswerClick(TObject *Sender)
{
  Choice = PlaceAnswer;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::MenuPlaceScriptClick(TObject *Sender)
{
  Choice = PlaceScript;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ContainerKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if (Key == VK_DELETE)
	MenuDel->Click();
  else if (Key == VK_RETURN)
	MenuEdit->Click();
  else if ((Key == 78) && Shift.Contains(ssCtrl)) //обособити елемент (прибрати LinkedID та LinkedFromID)
	EPPStandAlone->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ContainerMouseDown(TObject *Sender,
												   TMouseButton Button,
												   TShiftState Shift,
												   int X, int Y)
{
  if (Button == mbRight)
	{
	  TBitBtn *clicked = dynamic_cast<TBitBtn*>(Sender);
	  Selected = FindElement(clicked->Tag);
	  TPoint cursor;
	  GetCursorPos(&cursor);
	  pp->Popup(cursor.X, cursor.Y);
	}
  else if (Button == mbLeft)
	{
	  MouseX = X;
	  MouseY = Y;
	  drag = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ContainerMouseUp(TObject *Sender,
												 TMouseButton Button,
												 TShiftState Shift,
												 int X, int Y)
{
  if (Button == mbLeft)
	{
	  drag = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ContainerMouseMove(TObject *Sender,
												   TShiftState Shift,
												   int X, int Y)
{
  if (drag)
	{
	  TBitBtn *clicked = dynamic_cast<TBitBtn*>(Sender);
	  TDlgBaseText *elm = FindElement(clicked->Tag);
	  elm->Left += X - MouseX;
	  elm->Top += Y - MouseY;
      changed = true;

	  Repaint();
	}
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
  try
	 {
       MouseX = X;
	   MouseY = Y;

	   if (Button == mbRight)
		 {
		   TPoint cursor;
		   GetCursorPos(&cursor);
		   MainPopupMenu->Popup(cursor.X, cursor.Y);
		 }
	   else
		 {
		   switch (Choice)
			 {
			   case PlaceText:
				 {
				   AddScreenText(X,
								 Y,
								 Images->GetBitmap(DlgText, 32, 32),
								 this);
				   UpdateItemsList(ItemList);

				   break;
				 }
			   case PlaceAnswer:
				 {
				   AddAnswer(X,
							 Y,
							 Images->GetBitmap(DlgAnsw, 32, 32),
							 this);
				   UpdateItemsList(ItemList);

				   break;
				 }
			   case PlaceScript:
				 {
				   AddScript(X,
							 Y,
							 Images->GetBitmap(DlgScript, 32, 32),
							 this);
				   UpdateItemsList(ItemList);

				   break;
				 }
			 }

		   PropList->Cols[1]->Clear();
		 }

       Choice = ActNone;
	   Selected = NULL;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "FormMouseDown: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
	drag = false;

  MouseX = X;
  MouseY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
  drag = false;
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::QBtNewClick(TObject *Sender)
{
  MenuNew->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::QBtOpenClick(TObject *Sender)
{
  MenuLoad->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::QBtSaveClick(TObject *Sender)
{
  MenuSave->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::QBtImportClick(TObject *Sender)
{
  MenuImportXML->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::QBtExportClick(TObject *Sender)
{
  MenuExportXML->Click();
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ItemListClick(TObject *Sender)
{
  try
	 {
	   String s_id = ItemList->Items->Strings[ItemList->ItemIndex];
	   int pos = s_id.Pos(":");

	   s_id.Delete(pos, s_id.Length() - pos + 1);

	   int id = s_id.ToInt();

	   Selected = FindElement(id);

	   if (Selected)
		 MenuEdit->Click();
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ItemListClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TStoryCreator::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
  if (Msg.message == WM_KEYUP)
	{
	  switch (Msg.wParam)
		{
		  case 'S': //ctrl+s
			{
			  if ((GetKeyState(VK_RCONTROL)|GetKeyState(VK_LCONTROL)) & 0x80)
				{
				  MenuSave->Click();
				  break;
				}
            }

		  case 'O': //ctrl+o
			{
			  if ((GetKeyState(VK_RCONTROL)|GetKeyState(VK_LCONTROL)) & 0x80)
				{
				  MenuLoad->Click();
				  break;
				}
			}

		  case 'I': //ctrl+i
			{
			  if ((GetKeyState(VK_RCONTROL)|GetKeyState(VK_LCONTROL)) & 0x80)
				{
				  MenuImportXML->Click();
				  break;
				}
			}

		  case 'E': //ctrl+e
			{
			  if ((GetKeyState(VK_RCONTROL)|GetKeyState(VK_LCONTROL)) & 0x80)
				{
				  MenuExportXML->Click();
				  break;
				}
			}

		  case '1': //1 - PlaceText
			{
			  Choice = PlaceText;
			  break;
			}

		  case '2': //2 - PlaceAnswer
			{
			  Choice = PlaceAnswer;
			  break;
			}

		  case '5': //3 - PlaceScript
			{
			  Choice = PlaceScript;
			  break;
			}

		  default:
			{
		  	  break;
			}
		}
    }
}
//---------------------------------------------------------------------------

