//---------------------------------------------------------------------------
/*!
Copyright 2023-2024 Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of Text Scene Creator.

	Text Scene Creator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Text Scene Creator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Text Scene Creator.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\..\work-functions\MyFunc.h"
#include "DlgClass.h"
#include "main.h"
#include "EditText.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditTextForm *EditTextForm;
extern TDlgBaseText *Selected;
extern bool changed;
extern int Choice;

//---------------------------------------------------------------------------
__fastcall TEditTextForm::TEditTextForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = StoryCreator;
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::FormShow(TObject *Sender)
{
  Text->Text = Selected->Text;
  Dialog->Text = IntToStr(Selected->Dialog);
  ID->Text = IntToStr(Selected->ID);
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::IDChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][0] = ID->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::DialogChange(TObject *Sender)
{
  changed = true;

  StoryCreator->PropList->Cells[1][3] = Dialog->Text;
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::EditExit(TObject *Sender)
{
  TEdit *edit = dynamic_cast<TEdit*>(Sender);

  if (edit->Name == "ID")
	StoryCreator->ChangeID(edit->Text.ToInt());
  else if (edit->Name == "Dialog")
	StoryCreator->ChangeDialog(edit->Text.ToInt());

  UpdateItemsList(StoryCreator->ItemList);
  StoryCreator->Repaint();
  Selected->SetContainerData();
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::EditKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)

{
  if (Key == 13)
	dynamic_cast<TEdit*>(Sender)->OnExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  Choice = ActNone;
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::TextChange(TObject *Sender)
{
  changed = true;

  Selected->Text = Text->Text;
  Selected->SetContainerData();
}
//---------------------------------------------------------------------------
