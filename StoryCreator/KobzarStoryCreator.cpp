//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("quests.cpp", QuestManager);
USEFORM("main.cpp", StoryCreator);
USEFORM("EditCondition.cpp", EditConditionForm);
USEFORM("EditAnswer.cpp", EditAnswerForm);
USEFORM("EditAdvAnswer.cpp", EditAdvAnswerForm);
USEFORM("EditText.cpp", EditTextForm);
USEFORM("EditScript.cpp", EditScriptForm);
USEFORM("Splash.cpp", SplashForm);
USEFORM("settings.cpp", AppSettings);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
        Application->CreateForm(__classid(TSplashForm), &SplashForm);
		Application->CreateForm(__classid(TStoryCreator), &StoryCreator);
		Application->CreateForm(__classid(TAppSettings), &AppSettings);
		Application->CreateForm(__classid(TEditTextForm), &EditTextForm);
		Application->CreateForm(__classid(TEditAnswerForm), &EditAnswerForm);
		Application->CreateForm(__classid(TEditAdvAnswerForm), &EditAdvAnswerForm);
		Application->CreateForm(__classid(TEditScriptForm), &EditScriptForm);
		Application->CreateForm(__classid(TEditConditionForm), &EditConditionForm);
		Application->CreateForm(__classid(TQuestManager), &QuestManager);
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
