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

#define TEXT_COLOR 0x3393FF
#define ANSW_COLOR 0xFFC133
#define SCRIPT_COLOR 0x33FF3C

#define CONTAINER_WIDTH 220
#define CONTAINER_HEIGHT 150

enum DlgActions
	 {ActNone = 0,
	  PlaceText = 1,
	  PlaceAnswer = 2,
	  PlaceScript = 3,
	  SelectText = 4,
	  SelectAnswer = 5,
	  SelectScript = 6};

enum DlgType
	 {DlgText = 1,
	  DlgAnsw = 2,
	  DlgScript = 3};

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
	TLabel *cont;
	int left;
	int top;

	int GetID();
	void SetID(int val);

	int GetLinkedID();
	void SetLinkedID(int val);

	int GetLinkedFromID();
	void SetLinkedFromID(int val);

	int GetDialog();
	void SetDialog(int val);

	int GetNextDialog();
	void SetNextDialog(int val);

	TLabel *GetContainer(){return cont;}
	void SetContainer(TLabel *val){cont = val;}

	int GetLeft();
	void SetLeft(int val);

	int GetTop();
	void SetTop(int val);

  public:
	TDlgBaseText();
	TDlgBaseText(int left, int top);
	TDlgBaseText(int left, int top, int el_id, TForm *ContainerOwner);
	TDlgBaseText(int left, int top, int el_id, int dlg_id, TForm *ContainerOwner);
	TDlgBaseText(int left, int top, TForm *ContainerOwner);
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
	TLabel *CreateContainer(TForm *owner);
    void SetContainerData();

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
	__property int Dialog = {read = GetDialog, write = SetDialog};
//номер следующего диалога (сцены)
	__property int NextDialog = {read = GetNextDialog, write = SetNextDialog};
//контейнер для хранения xml-структуры для экспорта элемента
	__property String XMLText = {read = xml, write = xml};
//контейнер для текста, который содержит элемент
	__property String Text = {read = txt, write = txt};
//иконка элемента, отображается на форме
	__property TLabeledEdit *Container = {read = GetContainer, write = SetContainer};
//позиции элемента на форме
	__property int Left = {read = GetLeft, write = SetLeft};
	__property int Top = {read = GetTop, write = SetTop};
};
//---------------------------------------------------------------------------

class TDlgScreenText : public TDlgBaseText
{
  public:
	TDlgScreenText(int left, int top) : TDlgBaseText(left, top)
	{
	  Type = DlgText;
	  Dialog = GenDialogID();
	}

	TDlgScreenText(int left, int top, TForm *ContainerOwner)
	: TDlgBaseText(left, top, ContainerOwner)
	{
	  Type = DlgText;
	  Dialog = GenDialogID();
	  SetContainerData();
	}

	TDlgScreenText(int left, int top, int el_id, int dlg_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, ContainerOwner)
	{
	  Type = DlgText;
      SetContainerData();
	}

	virtual ~TDlgScreenText(){};

	const wchar_t *CreateXML();
};
//---------------------------------------------------------------------------

class TDlgAnswer : public TDlgBaseText
{
  private:
	bool end_dlg;
	int prev_id;

  public:
	TDlgAnswer(int left, int top): TDlgBaseText(left, top)
	{
	  Type = DlgAnsw;
	  end_dlg = false;
	  prev_id = -1;
	}

	TDlgAnswer(int left, int top, TForm *ContainerOwner)
	: TDlgBaseText(left, top, ContainerOwner)
    {
	  Type = DlgAnsw;
	  end_dlg = false;
	  prev_id = -1;
	  SetContainerData();
	}

	TDlgAnswer(int left, int top, int el_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, ContainerOwner)
    {
	  Type = DlgAnsw;
      end_dlg = false;
	  prev_id = -1;
	  SetContainerData();
	}

	TDlgAnswer(int left, int top, int el_id, int dlg_id, int next_dlg_id,
				   int link_id, int link_fr_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, next_dlg_id,
				   link_id, link_fr_id, ContainerOwner)
	{
	  Type = DlgAnsw;
	  end_dlg = false;
	  prev_id = -1;
      SetContainerData();
	}

	virtual ~TDlgAnswer(){};

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
	TDlgScript(int left, int top) : TDlgBaseText(left, top)
	{
	  Type = DlgScript;
	}

	TDlgScript(int left, int top, TForm *ContainerOwner)
	: TDlgBaseText(left, top, ContainerOwner)
	{
	  Type = DlgScript;
	  SetContainerData();
	}

	TDlgScript(int left, int top, int el_id, int dlg_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, ContainerOwner)
	{
	  Type = DlgScript;
      SetContainerData();
	}

	TDlgScript(int left, int top, int el_id, int dlg_id, int next_dlg_id,
				   int link_id, int link_fr_id, TForm *ContainerOwner)
	: TDlgBaseText(left, top, el_id, dlg_id, next_dlg_id,
				   link_id, link_fr_id, ContainerOwner)
	{
	  Type = DlgScript;
	  SetContainerData();
	}

	virtual ~TDlgScript(){};

	const wchar_t *CreateXML();

//контейнер для списка параметров
	__property String Params = {read = params, write = params};
//результат выполнения
	__property String Result = {read = result};
};
//---------------------------------------------------------------------------

TDlgBaseText *FindElement(int id);
int FindLinkedElements(int id, std::vector<TDlgBaseText*> *el_list);
int FindAnswersByDialog(int id, std::vector<TDlgBaseText*> *el_list);
int FindTextElementID(int crd_dlg);
void RemoveFromItems(TDlgBaseText *element);
void RedrawLinks();
void UpdateContainers();
bool SaveDlgSchema(const wchar_t *file);
bool LoadDlgSchema(const wchar_t *file);
int SearchDependeciesID(int id);
int SearchDependeciesDialog(int id);
void UpdateLinkedID(int old_id, int new_id);
void UpdateDialog(int old_val, int new_val);
void AddScreenText(int left, int top, TForm *IconOwner);
void AddAnswer(int left, int top, TForm *IconOwner);
void AddScript(int left, int top, TForm *IconOwner);
void XMLImport(String xml_file);
void XMLExport(const wchar_t *path);
void ClearItems();
void BuildLinksAfterXMLImport();
void RemoveLimboLinks();
String CreateContainerCaption(TDlgBaseText *element);

#endif
