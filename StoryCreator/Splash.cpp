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
#include "main.h"
#include "Splash.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TSplashForm *SplashForm;
//---------------------------------------------------------------------------
__fastcall TSplashForm::TSplashForm(TComponent* Owner)
	: TForm(Owner)
{
  LbVersion->Caption = GetVersionInString(Application->ExeName.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TSplashForm::SplashCloseTimerTimer(TObject *Sender)
{
  SplashCloseTimer->Enabled = false;
  StoryCreator->Show();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormShow(TObject *Sender)
{
  SplashCloseTimer->Enabled = true;
}
//---------------------------------------------------------------------------

