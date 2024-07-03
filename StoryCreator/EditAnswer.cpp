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

#include "..\..\work-functions\MyFunc.h"
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
  CardOfDialog->Text = IntToStr(Selected->CardOfDialog);
  NextCardOfDialog->Text = IntToStr(Selected->NextCardOfDialog);
  LinkedID->Text = IntToStr(Selected->LinkedID);
  LinkedFromID->Text = IntToStr(Selected->LinkedFromID);
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

void __fastcall TEditAnswerForm::CardOfDialogChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][3] = CardOfDialog->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::NextCardOfDialogChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][4] = NextCardOfDialog->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::LinkedIDChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][1] = LinkedID->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::LinkedFromIDChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][2] = LinkedFromID->Text;
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
  else if (edit->Name == "CardOfDialog")
	StoryCreator->ChangeCardOfDialog(edit->Text.ToInt());
  else if (edit->Name == "NextCardOfDialog")
	StoryCreator->ChangeNextCardOfDialog(edit->Text.ToInt());
  else if (edit->Name == "LinkedID")
	StoryCreator->ChangeLinkedID(edit->Text.ToInt());
  else if (edit->Name == "LinkedFromID")
	StoryCreator->ChangeLinkedFromID(edit->Text.ToInt());

  LinkedID->Text = IntToStr(Selected->LinkedID);
  LinkedFromID->Text = IntToStr(Selected->LinkedFromID);
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
