//---------------------------------------------------------------------------
/*!
Copyright 2020 Maxim Noltmeer (m.noltmeer@gmail.com)

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
  CardOfDialog->Text = IntToStr(Selected->CardOfDialog);
  ID->Text = IntToStr(Selected->ID);
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::DiscardClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditTextForm::ConfirmClick(TObject *Sender)
{
  StoryCreator->PropList->Cells[1][0] = ID->Text;
  StoryCreator->PropList->Cells[1][3] = CardOfDialog->Text;

  if (Selected->Text != Text->Text)
	changed = true;

  Selected->Text = Text->Text;

  StoryCreator->ChangeElement();
  Close();
}
//---------------------------------------------------------------------------
