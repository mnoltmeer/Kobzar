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
#include "QuestLibrary.h"
#include "EditAdvAnswer.h"
#include "quests.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TQuestManager *QuestManager;
TQuestLibrary *Quests;

String quest_state_item, quest_item;
int row, col; //для отслеживания столбца и строки, где находится курсор мыши

extern String LogPath;
extern int QuestValue; //0 - NeedQuestValue, 1 - SetQuestValue
extern String QuestLibFile;

//---------------------------------------------------------------------------
__fastcall TQuestManager::TQuestManager(TComponent* Owner)
	: TForm(Owner)
{
  Owner = StoryCreator;
  Quests = new TQuestLibrary();

  if (QuestLibFile != "")
	Import(QuestLibFile);
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::FormResize(TObject *Sender)
{
  QuestStates->ColWidths[1] = QuestStates->Width - QuestStates->ColWidths[0] - 25;
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::FormShow(TObject *Sender)
{
  QuestList->TitleCaptions->Strings[0] = "ID";
  QuestList->TitleCaptions->Strings[1] = "Name";
  QuestStates->TitleCaptions->Strings[0] = "State";
  QuestStates->TitleCaptions->Strings[1] = "Journal log";

  quest_state_item = "";
  quest_item = "";

  if (QuestLibFile != "")
	Import(QuestLibFile);
}
//---------------------------------------------------------------------------

void TQuestManager::Import(String xml_file)
{
  try
	 {
	   Quests->Clear();

	   std::unique_ptr<TXMLDocument> ixml(new TXMLDocument(Application));

	   ixml->DOMVendor = GetDOMVendor("MSXML");
	   ixml->FileName = xml_file;
	   ixml->Active = true;
	   ixml->Encoding = "UTF-8";
	   ixml->Options = ixml->Options << doNodeAutoIndent;

	   _di_IXMLNode QuestLib = ixml->DocumentElement;
	   _di_IXMLNode Quest;
	   _di_IXMLNode Record;
	   _di_IXMLNode JournalRecord;

	   String quest_id, quest_name, rec_state, rec_jrn;

	   for (int i = 0; i < QuestLib->ChildNodes->Count; i++)
		  {
			Quest = QuestLib->ChildNodes->Nodes[i];

			quest_id = Quest->GetAttribute("ID");
			quest_name = Quest->GetAttribute("Name");

			TQuest *imp = new TQuest(quest_id, quest_name);

			for (int j = 0; j < Quest->ChildNodes->Count; j++)
			   {
				 Record = Quest->ChildNodes->Nodes[j];

				 rec_state = Record->GetAttribute("State");

				 rec_jrn = Record->ChildNodes->Nodes[0]->Text;

				 imp->AddRecord(rec_state, rec_jrn);
			   }

			Quests->AddQuest(imp);
		  }

	   QuestStates->Strings->Clear();
	   QuestList->Strings->Clear();

	   for (int i = 0; i < Quests->Count; i++)
		  QuestList->Strings->Add(Quests->Items[i]->ID + "=" + Quests->Items[i]->Name);

	   QuestLibPath->Text = xml_file;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "TQuestManager::Import: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void TQuestManager::Export(String path)
{
  try
	 {
	   std::unique_ptr<TStringList> list(new TStringList());

	   String xml_exp = "<QuestLibrary>\r\n";

	   for (int i = 0; i < Quests->Count; i++)
		  {
			TQuest *exp = Quests->Items[i];

			xml_exp += "\t<Quest ID = '" + exp->ID + "' Name = '" + exp->Name + "'>\r\n";

			for (int j = 0; j < exp->Count; j++)
			   {
				 xml_exp += "\t\t<Record State = '" + exp->Records[j]->State + "'>\r\n";
				 xml_exp += "\t\t\t<Journal>" + exp->Records[j]->JournalRecord;
				 xml_exp += "</Journal>\r\n";
				 xml_exp += "\t\t</Record>\r\n";
			   }

			xml_exp += "\t</Quest>\r\n";
		  }

	   xml_exp += "</QuestLibrary>\r\n";

	   list->Text = xml_exp;
	   list->SaveToFile(path, TEncoding::UTF8);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "TQuestManager::Export: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::PPAddQuestClick(TObject *Sender)
{
  String name = InputBox("Inserting new Quest",
						 "Set ID", "");

  if (name != "")
	{
	  if (QuestList->Strings->Count == 0)
		QuestList->Strings->Add(name + "=");
	  else
		QuestList->Strings->Insert(row, name + "=");

	  Quests->AddQuest(new TQuest(name, ""));
	}
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::PPDelQuestClick(TObject *Sender)
{
  if (MessageBox(Application->Handle,
				 L"Deleting the Quest will delete all Quest States.\nAre you sure?",
				 L"Confirm deleting",
				 MB_YESNO | MB_ICONWARNING) == mrYes)
	{
	  Quests->RemoveQuest(quest_item);
	  QuestList->DeleteRow(row);
	  QuestStates->Strings->Clear();
	}
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::QuestListValidate(TObject *Sender, int ACol, int ARow,
		  const UnicodeString KeyName, const UnicodeString KeyValue)
{
  quest_item = KeyName;

  int ind = Quests->IndexOf(quest_item);

  if (ind > -1)
	{
	  Quests->Items[ind]->Name = KeyValue;
	}
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::QuestListSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
  col = ACol;
  row = ARow;

  try
	 {
	   quest_item = QuestList->Keys[ARow];
	   int ind = Quests->IndexOf(quest_item);

	   if (ind > -1)
		 {
		   QuestStates->Strings->Clear();
		   QuestStates->Strings->AddStrings(Quests->Items[ind]->RecordStrings);
		   EditAdvAnswerForm->QuestName->Text = Quests->Items[ind]->Name;
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log",
			   "TQuestManager::QuestListSelectCell: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::PPAddStateClick(TObject *Sender)
{
  String name = InputBox("Inserting new Quest State",
						 "Set State value", "");

  if (name != "")
	{
	  if (QuestStates->Strings->Count == 0)
		QuestStates->Strings->Add(name + "=");
	  else
		QuestStates->Strings->Insert(row, name + "=");

      int ind = Quests->IndexOf(quest_item);

	  if (ind > -1)
		{
		  TQuest *edt = Quests->Items[ind];

		  edt->AddRecord(name, "");
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::PPDelStateClick(TObject *Sender)
{
  int ind = Quests->IndexOf(quest_item);

  if (ind > -1)
	{
	  TQuest *edt = Quests->Items[ind];

	  edt->RemoveRecord(quest_state_item);
	}

  QuestStates->DeleteRow(row);
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::QuestStatesSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
  col = ACol;
  row = ARow;

  try
	 {
	   quest_state_item = QuestStates->Keys[row];

	   if (QuestValue == 0)
		 EditAdvAnswerForm->NeedQuestValue->Text = quest_state_item;
	   else
		 EditAdvAnswerForm->SetQuestValue->Text = quest_state_item;
	 }
  catch (Exception &e)
	 {
       SaveLog(LogPath + "\\exceptions.log",
			   "TQuestManager::QuestStatesSelectCell: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::QuestStatesValidate(TObject *Sender, int ACol, int ARow,
		  const UnicodeString KeyName, const UnicodeString KeyValue)
{
  quest_state_item = KeyName;

  int ind = Quests->IndexOf(quest_item);

  if (ind > -1)
	{
	  TQuest *edt = Quests->Items[ind];

	  ind = edt->IndexOf(quest_state_item);

	  if (ind > -1)
		edt->Records[ind]->JournalRecord = KeyValue;
	}
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::SetQuestLibPathClick(TObject *Sender)
{
  OpenQuestLibFile->FileName = "";

  if (OpenQuestLibFile->Execute())
	{
	  QuestLibFile = OpenQuestLibFile->FileName;

	  QuestLibPath->Text = QuestLibFile;

	  if (!FileExists(QuestLibFile))
		SaveToFile(QuestLibFile, "<QuestLibrary>\r\n</QuestLibrary>\r\n");

	  Import(QuestLibFile);
    }
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::FormDestroy(TObject *Sender)
{
  delete Quests;
}
//---------------------------------------------------------------------------

void __fastcall TQuestManager::FormClose(TObject *Sender, TCloseAction &Action)
{
  if (QuestLibFile != "")
	Export(QuestLibFile);

  StoryCreator->MenuQuestOrganiser->Checked = !StoryCreator->MenuQuestOrganiser->Checked;
}
//---------------------------------------------------------------------------


