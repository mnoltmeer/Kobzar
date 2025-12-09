//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("settings.cpp", AppSettings);
USEFORM("main.cpp", StoryCreator);
USEFORM("EditScript.cpp", EditScriptForm);
USEFORM("EditAnswer.cpp", EditAnswerForm);
USEFORM("EditText.cpp", EditTextForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
        Application->CreateForm(__classid(TStoryCreator), &StoryCreator);
		Application->CreateForm(__classid(TAppSettings), &AppSettings);
		Application->CreateForm(__classid(TEditTextForm), &EditTextForm);
		Application->CreateForm(__classid(TEditAnswerForm), &EditAnswerForm);
		Application->CreateForm(__classid(TEditScriptForm), &EditScriptForm);
		Application->CreateForm(__classid(TAppSettings), &AppSettings);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
