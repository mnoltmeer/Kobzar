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
#include "DlgClass.h"
#include "quests.h"
#include "main.h"
#include "EditAdvAnswer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditAdvAnswerForm *EditAdvAnswerForm;

int QuestValue; //0 - NeedQuestValue, 1 - SetQuestValue
extern TDlgBaseText *Selected;
//extern TStringGrid *PropList;
extern bool changed;

//---------------------------------------------------------------------------
__fastcall TEditAdvAnswerForm::TEditAdvAnswerForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = StoryCreator;
}
//---------------------------------------------------------------------------

void __fastcall TEditAdvAnswerForm::UpdateInfo()
{
  Text->Text = Selected->Text;
  CardOfDialog->Text = IntToStr(Selected->CardOfDialog);
  NextCardOfDialog->Text = IntToStr(Selected->NextCardOfDialog);
  LinkedID->Text = IntToStr(Selected->LinkedID);
  LinkedFromID->Text = IntToStr(Selected->LinkedFromID);
  EndDialog->Checked = dynamic_cast<TDlgAnswer*>(Selected)->EndDialog;
  QuestName->Text = dynamic_cast<TDlgAnswer*>(Selected)->QuestName;
  NeedQuestValue->Text = dynamic_cast<TDlgAnswer*>(Selected)->NeedQuestValue;
  SetQuestValue->Text = dynamic_cast<TDlgAnswer*>(Selected)->SetQuestValue;
  ID->Text = IntToStr(Selected->ID);
}
//---------------------------------------------------------------------------

void __fastcall TEditAdvAnswerForm::FormShow(TObject *Sender)
{
  UpdateInfo();
}
//---------------------------------------------------------------------------

void __fastcall TEditAdvAnswerForm::ConfirmClick(TObject *Sender)
{
  if (!EndDialog->Checked)
	{
	  dynamic_cast<TDlgAnswer*>(Selected)->EndDialog = false;
	}

  StoryCreator->PropList->Cells[1][0] = ID->Text;
  StoryCreator->PropList->Cells[1][3] = CardOfDialog->Text;
  StoryCreator->PropList->Cells[1][4] = NextCardOfDialog->Text;
  StoryCreator->PropList->Cells[1][1] = LinkedID->Text;
  StoryCreator->PropList->Cells[1][2] = LinkedFromID->Text;

  if (Selected->Text != Text->Text)
	changed = true;
  else if (dynamic_cast<TDlgAnswer*>(Selected)->QuestName != QuestName->Text)
	changed = true;
  else if (dynamic_cast<TDlgAnswer*>(Selected)->NeedQuestValue != NeedQuestValue->Text)
	changed = true;
  else if (dynamic_cast<TDlgAnswer*>(Selected)->SetQuestValue != SetQuestValue->Text)
	changed = true;

  Selected->Text = Text->Text;

  dynamic_cast<TDlgAnswer*>(Selected)->QuestName = QuestName->Text;
  dynamic_cast<TDlgAnswer*>(Selected)->NeedQuestValue = NeedQuestValue->Text;
  dynamic_cast<TDlgAnswer*>(Selected)->SetQuestValue = SetQuestValue->Text;

  StoryCreator->ChangeElement();

  if (EndDialog->Checked)
	{
	  dynamic_cast<TDlgAnswer*>(Selected)->EndDialog = true;
	  StoryCreator->Repaint();
	}

  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditAdvAnswerForm::DiscardClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditAdvAnswerForm::BtSelectQuestClick(TObject *Sender)
{
  QuestManager->Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditAdvAnswerForm::BtNeedQuestClick(TObject *Sender)
{
  QuestValue = 0;
  QuestManager->Show();
}
//---------------------------------------------------------------------------

void __fastcall TEditAdvAnswerForm::BtSetQuestClick(TObject *Sender)
{
  QuestValue = 1;
  QuestManager->Show();
}
//---------------------------------------------------------------------------

