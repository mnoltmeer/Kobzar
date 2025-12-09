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

#include "..\..\work-functions\Logs.h"
#include "main.h"
#include "EditText.h"
#include "EditScript.h"
#include "EditAnswer.h"
#include "settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAppSettings *AppSettings;

extern String FontName;
extern int FontSize;
extern bool SyntaxHighlight;
extern String LogPath;

//---------------------------------------------------------------------------
__fastcall TAppSettings::TAppSettings(TComponent* Owner)
	: TForm(Owner)
{
  Owner = StoryCreator;
}
//---------------------------------------------------------------------------

void __fastcall TAppSettings::FormShow(TObject *Sender)
{
  try
	 {
	   CurrentFont->Text = FontDialog->Font->Name;
	   CurrentFontSize->Text = IntToStr(FontDialog->Font->Size);
	   EnableSyntaxHighlight->Checked = SyntaxHighlight;
	   CancelSettings->SetFocus();
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("StoryCreator.log", "Kobzar", "AppSettings::FormShow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TAppSettings::CancelSettingsClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TAppSettings::SaveSettingsClick(TObject *Sender)
{
  try
	 {
	   if (FontDialog->Font->Size != CurrentFontSize->Text.ToInt())
		 FontDialog->Font->Size = CurrentFontSize->Text.ToInt();

	   FontName = FontDialog->Font->Name;
	   FontSize = FontDialog->Font->Size;

	   SyntaxHighlight = EnableSyntaxHighlight->Checked;
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("StoryCreator.log", "Kobzar", "AppSettings::SaveSettingsClick: " + e.ToString());
	 }

  Close();
}
//---------------------------------------------------------------------------

void __fastcall TAppSettings::SelectFontClick(TObject *Sender)
{
  if (FontDialog->Execute())
	{
	  CurrentFont->Text = FontDialog->Font->Name;
	  CurrentFontSize->Text = IntToStr(FontDialog->Font->Size);

	  EditScriptForm->Text->Font->Assign(FontDialog->Font);
	  EditScriptForm->Text->SelectAll();
	  EditScriptForm->Text->SelAttributes->Assign(FontDialog->Font);
	  EditScriptForm->Text->SelLength = 0;

	  EditTextForm->Text->Font->Assign(FontDialog->Font);
	  EditAnswerForm->Text->Font->Assign(FontDialog->Font);

//посилаємо спец. пустий символ для підсвітки
	  if (SyntaxHighlight)
		SendMessage(EditScriptForm->Text->Handle, WM_KEYUP, 0, NULL);
	}
}
//---------------------------------------------------------------------------


