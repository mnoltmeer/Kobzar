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
#include "main.h"
#include "EditScript.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditScriptForm *EditScriptForm;
extern TDlgBaseText *Selected;
extern bool SyntaxHighlight;
extern String AppDir, LogPath;
extern bool changed;

//---------------------------------------------------------------------------
__fastcall TEditScriptForm::TEditScriptForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = StoryCreator;
}
//---------------------------------------------------------------------------

void __fastcall TEditScriptForm::FormShow(TObject *Sender)
{
  Text->Text = Selected->Text;

  CardOfDialog->Text = IntToStr(Selected->CardOfDialog);
  ID->Text = IntToStr(Selected->ID);
  Params->Text = dynamic_cast<TDlgScript*>(Selected)->Params;
  ResultParam->Text = dynamic_cast<TDlgScript*>(Selected)->ResultParam;

//посилаємо спец. пустий символ для підсвітки
  if (SyntaxHighlight)
	SendMessage(Text->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TEditScriptForm::DiscardClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditScriptForm::ConfirmClick(TObject *Sender)
{
  StoryCreator->PropList->Cells[1][0] = ID->Text;
  StoryCreator->PropList->Cells[1][3] = CardOfDialog->Text;

  TDlgScript *tmp = dynamic_cast<TDlgScript*>(Selected);

  Text->PlainText = true;

  if (Selected->Text != Text->Text)
	changed = true;
  else if (tmp->Params != Params->Text)
	changed = true;
  else if (tmp->ResultParam != ResultParam->Text)
	changed = true;

  tmp->Params = Params->Text;
  tmp->ResultParam = ResultParam->Text;
  Selected->Text = Text->Text;
  Text->PlainText = false;

  dynamic_cast<TDlgScript*>(Selected)->Params = Params->Text;

  StoryCreator->ChangeElement();
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditScriptForm::TextKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if (Key == 120)
	return;

  if (Key == 9) //tab
	{
	  TPoint user_caret = Text->CaretPos;
	  user_caret.X += 2 - 1;
	  LockWindowUpdate(Text->Handle);
	  String str = Text->Lines->Strings[Text->CaretPos.Y];
	  str = RemoveTabs(str, 2);
	  Text->Lines->Strings[Text->CaretPos.Y] = str;
	  Text->CaretPos = user_caret;
	  LockWindowUpdate(NULL);
	}
  else if (Key == 13) //enter
	{
	  if (SyntaxHighlight)
		{
		  LockWindowUpdate(Text->Handle);
		  HighlightSource(Text, Text->CaretPos.Y - 1);
		  LockWindowUpdate(NULL);
		}
	}
  else if (Key == 0) //пустий символ для активації підсвітки
	{
	  LockWindowUpdate(Text->Handle);
	  HighlightSourceFull(Text);
	  LockWindowUpdate(NULL);
	}

  if (!(Shift.Contains(ssCtrl) || Shift.Contains(ssShift) || (Key == 16) || (Key == 17) || (Key == 18)))
	{
	  if (SyntaxHighlight)
		{
		  LockWindowUpdate(Text->Handle);
		  HighlightSource(Text, Text->CaretPos.Y);
		  LockWindowUpdate(NULL);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TEditScriptForm::TextKeyPress(TObject *Sender, System::WideChar &Key)
{
  switch (Key)
	{
	  case 22: //ctrl+v
		{
		  if (SyntaxHighlight)
			SendMessage(Text->Handle, WM_KEYUP, 0, NULL);

		  break;
		}
	  default:break;
	}
}
//---------------------------------------------------------------------------
