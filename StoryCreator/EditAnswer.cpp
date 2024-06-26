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
  UpdateInfo();
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::UpdateInfo()
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

void __fastcall TEditAnswerForm::ConfirmClick(TObject *Sender)
{
  StoryCreator->PropList->Cells[1][0] = ID->Text;
  StoryCreator->PropList->Cells[1][3] = CardOfDialog->Text;
  StoryCreator->PropList->Cells[1][4] = NextCardOfDialog->Text;
  StoryCreator->PropList->Cells[1][1] = LinkedID->Text;
  StoryCreator->PropList->Cells[1][2] = LinkedFromID->Text;

  if (Selected->Text != Text->Text)
	changed = true;

  Selected->Text = Text->Text;

  StoryCreator->ChangeElement();

  TDlgAnswer *itm = dynamic_cast<TDlgAnswer*>(Selected);

  itm->EndDialog = EndDialog->Checked;
  StoryCreator->Repaint();

  Choice = ActNone;
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditAnswerForm::DiscardClick(TObject *Sender)
{
  Choice = ActNone;
  Close();
}
//---------------------------------------------------------------------------


