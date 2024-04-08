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
//---------------------------------------------------------------------------

#define DLG_TEXT_LIKE 0
#define DLG_ANSW_LIKE 1

enum DlgType {DlgText = 1,
			  DlgAnsw = 2,
			  DlgScript = 3};

class TDlgBaseText
{
  private:
	DlgType dt;
	int id;        //ид элемента
	int l_id;      //ид элемента к которому есть привязка
	int l_fr_id;   //id привязанного объекта SreenText
	int cd;
	int ncd;
	String xml;
	String txt;
	int cath;

	int GetCathegory();

    int GenDialogID();
	int GenElementID();

  public:
	TDlgBaseText(int el_id);
	TDlgBaseText(int el_id, int dlg_id);
	TDlgBaseText(int el_id, int dlg_id, int next_dlg_id, int link_id, int link_fr_id);
	inline virtual ~TDlgBaseText(){};

	virtual const wchar_t *CreateXML() = 0;
	virtual const wchar_t *GetInfo();
	virtual void GiveInfo(TStrings *lst);

//тип элемента
	__property DlgType Type = {read = dt, write = dt};
//ид элемента
	__property int ID = {read = id, write = id};
//ид элемента к которому есть привязка
	__property int LinkedID = {read = l_id, write = l_id};
//id привязанного объекта SreenText
	__property int LinkedFromID = {read = l_fr_id, write = l_fr_id};
//номер диалога (сцены) к которому относится элемент
	__property int CardOfDialog = {read = cd, write = cd};
//номер следующего диалога (сцены)
	__property int NextCardOfDialog = {read = ncd, write = ncd};
//контейнер для хранения xml-структуры для экспорта элемента
	__property String XMLText = {read = xml, write = xml};
//контейнер для текста, который содержит элемент
	__property String Text = {read = txt, write = txt};
//категория сцена или ответ
	__property int Cathegory = {read = GetCathegory};
};
//---------------------------------------------------------------------------

class TDlgScreenText : public TDlgBaseText
{
  public:
    TDlgScreenText(int el_id): TDlgBaseText(el_id){Type = DlgText;}
	TDlgScreenText(int el_id, int dlg_id): TDlgBaseText(el_id, dlg_id){Type = DlgText;}

	inline virtual ~TDlgScreenText(){};

	const wchar_t *CreateXML();
};
//---------------------------------------------------------------------------

class TDlgAnswer : public TDlgBaseText
{
  private:
	bool end_dlg;
	int prev_id;

  public:
	TDlgAnswer(int el_id): TDlgBaseText(el_id)
	{
	  Type = DlgAnsw;
      end_dlg = false;
	  prev_id = -1;
	}

	TDlgAnswer(int el_id, int dlg_id, int next_dlg_id, int link_id, int link_fr_id)
	: TDlgBaseText(el_id, dlg_id, next_dlg_id, link_id, link_fr_id)
	{
	  Type = DlgAnsw;
	  end_dlg = false;
	  prev_id = -1;
	}

	inline virtual ~TDlgAnswer(){};

	const wchar_t *CreateXML();

//для хранения старого id при выборе EndDialog
	__property int PrevLinkedFromID = {read = prev_id, write = prev_id};
//отметка о том, что этот элемент закрывает сцену
	__property bool EndDialog = {read = end_dlg, write = end_dlg};
};
//---------------------------------------------------------------------------

class TDlgScript : public TDlgBaseText
{
  private:
	String params;
	String result;

  public:
    TDlgScript(int el_id): TDlgBaseText(el_id)
	{Type = DlgScript;}

	TDlgScript(int el_id, int dlg_id): TDlgBaseText(el_id, dlg_id)
	{Type = DlgScript;}

	inline virtual ~TDlgScript(){};

	const wchar_t *CreateXML();

//контейнер для списка параметров
	__property String Params = {read = params, write = params};
//результат выполнения
	__property String Result = {read = result};
};
//---------------------------------------------------------------------------

#endif
