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

#ifndef DlgClassH
#define DlgClassH

#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <Xml.adomxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.Win.msxmldom.hpp>
#include <vector>
//---------------------------------------------------------------------------

#define DLG_TEXT_LIKE 0
#define DLG_ANSW_LIKE 1

namespace DblClick_BitBtn
{
  class TBitBtn : public Vcl::Buttons::TBitBtn
  {
	private:
	  TNotifyEvent FOnDblClick;

	  MESSAGE void __fastcall WmButtonDoubleClick(TMessage &Message)
	  {
		Vcl::Buttons::TBitBtn::Dispatch(&Message);

		if (FOnDblClick)
		  FOnDblClick(this);
	  }

      MESSAGE void __fastcall WmButtonClick(TMessage &Message)
	  {
		TMsg msg;
		long tm = ::GetTickCount() + ::GetDoubleClickTime(); // время для реакции на двойной клик

        while(::GetTickCount() < tm) // ждем возможного получения WM_LBUTTONDBLCLK компонентом
		  {
			if (::PeekMessage(&msg, Handle, WM_LBUTTONDBLCLK,
							 WM_LBUTTONDBLCLK, PM_NOREMOVE))
			  {
				return; // Дождались - двойной клик, здесь делать нечего
			  }
		  }

        Vcl::Buttons::TBitBtn::Dispatch(&Message);

		if (OnClick)
		  OnClick(this);
	  }

      BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, TMessage, WmButtonDoubleClick);
		MESSAGE_HANDLER(WM_LBUTTONDOWN, TMessage, WmButtonClick)
	  END_MESSAGE_MAP(Vcl::Buttons::TBitBtn);

    public:
	  __fastcall virtual TBitBtn(System::Classes::TComponent* AOwner)
	  : Vcl::Buttons::TBitBtn(AOwner){}

      __property TNotifyEvent OnDblClick = {read = FOnDblClick, write = FOnDblClick};
  };
}
#define TBitBtn DblClick_BitBtn::TBitBtn


enum DlgActions
	 {ActNone = 0,
	  PlaceText = 1,
	  PlaceAnswer = 2,
	  SelectText = 3,
	  SelectAnswer = 4,
	  PlaceSimpleAnswer = 5,
	  PlaceScript = 6,
	  PlaceCondition = 7,
	  SelectScript = 8,
	  SelectCondition = 9};

enum DlgType
	 {DlgText = 1,
	  DlgAnsw = 2,
	  DlgSimpleAnsw = 3,
	  DlgScript = 4,
	  DlgCondition = 5};

int GenDialogID();
int GenElementID();

class TDlgBaseText
{
  private:
	DlgType dt;
	int id;        //ид элемента
	int l_id;      //ид элемента к которому есть привязка
	int l_fr_id;   //id привязанного объекта SreenText
	TPoint il;     //точка для входящего линка (верхн. л. угол)
	TPoint ol;     //точка для исходящего линка (нижн. пр. угол)
	int cd;
	int ncd;
	String xml;
	String txt;
	TBitBtn *cont;
	int left;
	int top;
	int cath;

	int GetID();
	void SetID(int val);

	int GetLinkedID();
	void SetLinkedID(int val);

	int GetLinkedFromID();
	void SetLinkedFromID(int val);

	int GetCardOfDialog();
	void SetCardOfDialog(int val);

	int GetNextCardOfDialog();
	void SetNextCardOfDialog(int val);

	TBitBtn *GetContainer(){return cont;}
	void SetContainer(TBitBtn *val){cont = val;}

	int GetLeft();
	void SetLeft(int val);

	int GetTop();
	void SetTop(int val);

	int GetCathegory();

  public:
	TDlgBaseText();
	TDlgBaseText(int left, int top);
	TDlgBaseText(int left, int top, int el_id, TForm *ContainerOwner);
	TDlgBaseText(int left, int top, int el_id, int dlg_id, TForm *ContainerOwner);
	TDlgBaseText(int left, int top, TForm *ContainerOwner);
	TDlgBaseText(int left, int top, TBitmap *pic, TForm *ContainerOwner);
	TDlgBaseText(int left,
				 int top,
				 int el_id,
				 int dlg_id,
				 int next_dlg_id,
				 int link_id,
				 int link_fr_id,
				 TForm *ContainerOwner);
	virtual ~TDlgBaseText();

	void SetPos(int left, int top);
	TBitBtn *CreateContainer(TBitmap *pic, TForm *owner);
	void SetContainerImage(TBitmap *pic);

	virtual const wchar_t *CreateXML() = 0;
	virtual const wchar_t *GetInfo();
	virtual void GiveInfo(TStrings *lst);

	void __fastcall ContainerClick(TObject *Sender);
	void __fastcall ContainerDblClick(TObject *Sender);
    void __fastcall ContainerMouseEnter(TObject *Sender);

//тип элемента
	__property DlgType Type = {read = dt, write = dt};
//ид элемента
	__property int ID = {read = GetID, write = SetID};
//ид элемента к которому есть привязка
	__property int LinkedID = {read = GetLinkedID, write = SetLinkedID};
//id привязанного объекта SreenText
	__property int LinkedFromID = {read = GetLinkedFromID, write = SetLinkedFromID};
//точка для входящего линка (верхн. л. угол)
	__property TPoint InLink = {read = il, write = il};
//точка для исходящего линка (нижн. пр. угол)
	__property TPoint OutLink = {read = ol, write = ol};
//номер диалога (сцены) к которому относится элемент
	__property int CardOfDialog = {read = GetCardOfDialog, write = SetCardOfDialog};
//номер следующего диалога (сцены)
	__property int NextCardOfDialog = {read = GetNextCardOfDialog, write = SetNextCardOfDialog};
//контейнер для хранения xml-структуры для экспорта элемента
	__property String XMLText = {read = xml, write = xml};
//контейнер для текста, который содержит элемент
	__property String Text = {read = txt, write = txt};
//иконка элемента, отображается на форме
	__property TBitBtn *Container = {read = GetContainer, write = SetContainer};
//позиции элемента на форме
	__property int Left = {read = GetLeft, write = SetLeft};
	__property int Top = {read = GetTop, write = SetTop};
//категория сцена или ответ
	__property int Cathegory = {read = GetCathegory};
};
//---------------------------------------------------------------------------

class TDlgScreenText : public TDlgBaseText
{
  public:
	TDlgScreenText(int left, int top) : TDlgBaseText(left, top)
	{
	  Type = DlgText;
	  CardOfDialog = GenDialogID();
	}

	TDlgScreenText(int left, int top, TForm *ContainerOwner)
	: TDlgBaseText(left, top, ContainerOwner)
	{
	  Type = DlgText;
	  CardOfDialog = GenDialogID();
	}

	TDlgScreenText(int left, int top, TBitmap *pic, TForm *ContainerOwner)
	: TDlgBaseText(left, top, pic, ContainerOwner)
	{
	  Type = DlgText;
	  CardOfDialog = GenDialogID();
	}

	TDlgScreenText(int left, int top, int el_id, int dlg_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, ContainerOwner){Type = DlgText;}

	virtual ~TDlgScreenText(){};

	const wchar_t *CreateXML();
};
//---------------------------------------------------------------------------

class TDlgBaseAnswer : public TDlgBaseText
{
  private:
	bool end_dlg;
	int prev_id;

	bool GetEndDialog();
	void SetEndDialog(bool val);

  public:
	TDlgBaseAnswer(int left, int top): TDlgBaseText(left, top)
	{
	  Type = DlgSimpleAnsw;
	  end_dlg = false;
	  prev_id = -1;
	}

	TDlgBaseAnswer(int left, int top, TForm *ContainerOwner)
	: TDlgBaseText(left, top, ContainerOwner)
    {
	  Type = DlgSimpleAnsw;
	  end_dlg = false;
	  prev_id = -1;
	}

	TDlgBaseAnswer(int left, int top, TBitmap *pic, TForm *ContainerOwner)
	: TDlgBaseText(left, top, pic, ContainerOwner)
    {
	  Type = DlgSimpleAnsw;
	  end_dlg = false;
	  prev_id = -1;
	}

	TDlgBaseAnswer(int left, int top, int el_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, ContainerOwner)
    {
	  Type = DlgSimpleAnsw;
      end_dlg = false;
	  prev_id = -1;
	}

	TDlgBaseAnswer(int left, int top, int el_id, int dlg_id, int next_dlg_id,
				   int link_id, int link_fr_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, next_dlg_id,
				   link_id, link_fr_id, ContainerOwner)
	{
	  Type = DlgSimpleAnsw;
	  end_dlg = false;
	  prev_id = -1;
	}

	virtual ~TDlgBaseAnswer(){};

	const wchar_t *CreateXML();

//для хранения старого id при выборе EndDialog
	__property int PrevLinkedFromID = {read = prev_id, write = prev_id};
//отметка о том, что этот элемент закрывает сцену
	__property bool EndDialog = {read = GetEndDialog, write = SetEndDialog};
};
//---------------------------------------------------------------------------

class TDlgAnswer : public TDlgBaseAnswer
{
  private:
	String quest_name;
	String need_quest;
	String set_quest;

  public:
	TDlgAnswer(int left, int top): TDlgBaseAnswer(left, top){Type = DlgAnsw;}

	TDlgAnswer(int left, int top, TForm *ContainerOwner)
	: TDlgBaseAnswer(left, top, ContainerOwner){Type = DlgAnsw;}

	TDlgAnswer(int left, int top, TBitmap *pic, TForm *ContainerOwner)
	: TDlgBaseAnswer(left, top, pic, ContainerOwner){Type = DlgAnsw;}

	TDlgAnswer(int left, int top, int el_id, TForm *ContainerOwner)
	: TDlgBaseAnswer(left, top, el_id, ContainerOwner){Type = DlgAnsw;}

	TDlgAnswer(int left, int top, int el_id, int dlg_id, int next_dlg_id,
			   int link_id, int link_fr_id, TForm *ContainerOwner)
	: TDlgBaseAnswer(left, top, el_id, dlg_id, next_dlg_id,
					 link_id, link_fr_id, ContainerOwner)
    {
	  Type = DlgAnsw;
	}

	virtual ~TDlgAnswer(){};

	const wchar_t *CreateXML();

//контейнер для имени квеста, с которым работает элемент
	__property String QuestName = {read = quest_name, write = quest_name};
//значение квеста, которое нужно для отображения ответа
	__property String NeedQuestValue = {read = need_quest, write = need_quest};
//значение квеста, которое будет установлено при выборе ответа
	__property String SetQuestValue = {read = set_quest, write = set_quest};
};
//---------------------------------------------------------------------------

class TDlgScript : public TDlgBaseText
{
  private:
	String params;
	String result;
	String res_param;

  public:
	TDlgScript(int left, int top) : TDlgBaseText(left, top)
	{
	  Type = DlgScript;
	  CardOfDialog = GenDialogID();
	}

	TDlgScript(int left, int top, TForm *ContainerOwner)
	: TDlgBaseText(left, top, ContainerOwner)
	{
	  Type = DlgScript;
	  CardOfDialog = GenDialogID();
	}

	TDlgScript(int left, int top, TBitmap *pic, TForm *ContainerOwner)
	: TDlgBaseText(left, top, pic, ContainerOwner)
	{
	  Type = DlgScript;
	  CardOfDialog = GenDialogID();
	}

	TDlgScript(int left, int top, int el_id, int dlg_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, ContainerOwner){Type = DlgScript;}

	virtual ~TDlgScript(){};

	const wchar_t *CreateXML();

//контейнер для списка параметров
	__property String Params = {read = params, write = params};
//результат выполнения
	__property String Result = {read = result};
//имя параметра, в который сохраняется результат
	__property String ResultParam = {read = res_param, write = res_param};
};
//---------------------------------------------------------------------------

class TDlgCondition : public TDlgBaseText
{
  private:
	String cond;
	String script;
    String GetConditionScript();

  public:
	TDlgCondition(int left, int top) : TDlgBaseText(left, top)
	{Type = DlgCondition;}

	TDlgCondition(int left, int top, TForm *ContainerOwner)
	: TDlgBaseText(left, top, ContainerOwner){Type = DlgCondition;}

	TDlgCondition(int left, int top, TBitmap *pic, TForm *ContainerOwner)
	: TDlgBaseText(left, top, pic, ContainerOwner){Type = DlgCondition;}

	TDlgCondition(int left, int top, int el_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, ContainerOwner){Type = DlgCondition;}

	TDlgCondition(int left, int top, int el_id, int dlg_id, int next_dlg_id,
				  int link_id, int link_fr_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, next_dlg_id,
				   link_id, link_fr_id, ContainerOwner)
	{
	  Type = DlgCondition;
	}

	virtual ~TDlgCondition(){};

	const wchar_t *CreateXML();

//условие проверки
	__property String Condition = {read = cond, write = cond};
	__property String ConditionScript = {read = GetConditionScript};
};
//---------------------------------------------------------------------------

TDlgBaseText *FindElement(int id);
int FindLinkedElements(int id, std::vector<TDlgBaseText*> *el_list);
int FindAnswersByDialog(int id, std::vector<TDlgBaseText*> *el_list);
int FindTextElementID(int crd_dlg);
void RemoveFromItems(TDlgBaseText *element);
void RedrawLinks();
bool SaveDlgSchema(const wchar_t *file);
bool LoadDlgSchema(const wchar_t *file);
int SearchDependeciesID(int id);
int SearchDependeciesDialog(int id);
void UpdateLinkedID(int old_id, int new_id);
void UpdateCardOfDialog(int old_val, int new_val);
void AddScreenText(int left, int top, TBitmap *pic, TForm *IconOwner);
void AddAnswer(int left, int top, TBitmap *pic, TForm *IconOwner);
void AddAdvAnswer(int left, int top, TBitmap *pic, TForm *IconOwner);
void AddScript(int left, int top, TBitmap *pic, TForm *IconOwner);
void AddCondition(int left, int top, TBitmap *pic, TForm *IconOwner);
void XMLImport(String xml_file);
void XMLExport(const wchar_t *path);
void ClearItems();
void BuildLinksAfterXMLImport();
void RemoveLimboLinks();

#endif
