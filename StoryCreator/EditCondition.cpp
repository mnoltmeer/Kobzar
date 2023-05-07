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
#include "DlgClass.h"
#include "main.h"
#include "EditCondition.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditConditionForm *EditConditionForm;
extern TDlgBaseText *Selected;
extern bool changed;

//---------------------------------------------------------------------------
__fastcall TEditConditionForm::TEditConditionForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = StoryCreator;
}
//---------------------------------------------------------------------------

void __fastcall TEditConditionForm::FormShow(TObject *Sender)
{
  UpdateInfo();
}
//---------------------------------------------------------------------------

void __fastcall TEditConditionForm::UpdateInfo()
{
  Condition->Text = dynamic_cast<TDlgCondition*>(Selected)->Condition;
  Text->Text = Selected->Text;
  CardOfDialog->Text = IntToStr(Selected->CardOfDialog);
  NextCardOfDialog->Text = IntToStr(Selected->NextCardOfDialog);
  LinkedID->Text = IntToStr(Selected->LinkedID);
  LinkedFromID->Text = IntToStr(Selected->LinkedFromID);
  ID->Text = IntToStr(Selected->ID);
}
//---------------------------------------------------------------------------

void __fastcall TEditConditionForm::ConfirmClick(TObject *Sender)
{
  StoryCreator->PropList->Cells[1][0] = ID->Text;
  StoryCreator->PropList->Cells[1][3] = CardOfDialog->Text;
  StoryCreator->PropList->Cells[1][4] = NextCardOfDialog->Text;
  StoryCreator->PropList->Cells[1][1] = LinkedID->Text;
  StoryCreator->PropList->Cells[1][2] = LinkedFromID->Text;

  TDlgCondition *tmp = dynamic_cast<TDlgCondition*>(Selected);

  if (Selected->Text != Text->Text)
	changed = true;
  else if (tmp->Condition != Condition->Text)
	changed = true;

  Selected->Text = Text->Text;
  tmp->Condition = Condition->Text;

  StoryCreator->ChangeElement();
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditConditionForm::DiscardClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

