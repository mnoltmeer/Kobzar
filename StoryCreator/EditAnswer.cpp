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

#include <vcl.h>
#pragma hdrstop

#include "DlgClass.h"
#include "main.h"
#include "EditAnswer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditAnswerForm *EditAnswerForm;

extern TDlgBaseText *Selected;
//extern TStringGrid *PropList;
extern bool changed;
extern int Choice;

//---------------------------------------------------------------------------
__fastcall TEditAnswerForm::TEditAnswerForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = StoryCreator;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::FormShow(TObject *Sender)
{
  Text->Text = Selected->Text;
  Dialog->Text = IntToStr(Selected->Dialog);
  NextDialog->Text = IntToStr(Selected->NextDialog);
  PrevID->Text = IntToStr(Selected->PrevID);
  NextID->Text = IntToStr(Selected->NextID);
  EndDialog->Checked = dynamic_cast<TDlgAnswer*>(Selected)->EndDialog;
  ID->Text = IntToStr(Selected->ID);
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::TextChange(TObject *Sender)
{
  changed = true;

  Selected->Text = Text->Text;
  Selected->SetContainerData();
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::IDChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][0] = ID->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::DialogChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][3] = Dialog->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::NextDialogChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][4] = NextDialog->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::PrevIDChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][1] = PrevID->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::NextIDChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][2] = NextID->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::EndDialogClick(TObject *Sender)
{
  changed = true;

  TDlgAnswer *itm = dynamic_cast<TDlgAnswer*>(Selected);

  itm->EndDialog = EndDialog->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  Choice = ActNone;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::EditExit(TObject *Sender)
{
  TEdit *edit = dynamic_cast<TEdit*>(Sender);

  if (edit->Name == "ID")
	StoryCreator->ChangeID(edit->Text.ToInt());
  else if (edit->Name == "Dialog")
	StoryCreator->ChangeDialog(edit->Text.ToInt());
  else if (edit->Name == "NextDialog")
	StoryCreator->ChangeNextDialog(edit->Text.ToInt());
  else if (edit->Name == "PrevID")
	StoryCreator->ChangePrevID(edit->Text.ToInt());
  else if (edit->Name == "NextID")
	StoryCreator->ChangeNextID(edit->Text.ToInt());

  PrevID->Text = IntToStr(Selected->PrevID);
  NextID->Text = IntToStr(Selected->NextID);
  UpdateItemsList(StoryCreator->ItemList);
  StoryCreator->Repaint();
  Selected->SetContainerData();
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::EditKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
  if (Key == 13)
	dynamic_cast<TEdit*>(Sender)->OnExit(Sender);
}
//---------------------------------------------------------------------------
