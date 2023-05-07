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

