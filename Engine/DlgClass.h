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
			  DlgSimpleAnsw = 3,
			  DlgScript = 4,
			  DlgCondition = 5};

class TDlgBaseText
{
  private:
	DlgType dt;
	int id;        //�� ��������
	int l_id;      //�� �������� � �������� ���� ��������
	int l_fr_id;   //id ������������ ������� SreenText
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

//��� ��������
	__property DlgType Type = {read = dt, write = dt};
//�� ��������
	__property int ID = {read = id, write = id};
//�� �������� � �������� ���� ��������
	__property int LinkedID = {read = l_id, write = l_id};
//id ������������ ������� SreenText
	__property int LinkedFromID = {read = l_fr_id, write = l_fr_id};
//����� ������� (�����) � �������� ��������� �������
	__property int CardOfDialog = {read = cd, write = cd};
//����� ���������� ������� (�����)
	__property int NextCardOfDialog = {read = ncd, write = ncd};
//��������� ��� �������� xml-��������� ��� �������� ��������
	__property String XMLText = {read = xml, write = xml};
//��������� ��� ������, ������� �������� �������
	__property String Text = {read = txt, write = txt};
//��������� ����� ��� �����
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

class TDlgBaseAnswer : public TDlgBaseText
{
  private:
	bool end_dlg;
	int prev_id;

  public:
	TDlgBaseAnswer(int el_id): TDlgBaseText(el_id)
	{
	  Type = DlgSimpleAnsw;
      end_dlg = false;
	  prev_id = -1;
	}

	TDlgBaseAnswer(int el_id, int dlg_id, int next_dlg_id, int link_id, int link_fr_id)
	: TDlgBaseText(el_id, dlg_id, next_dlg_id, link_id, link_fr_id)
	{
	  Type = DlgSimpleAnsw;
	  end_dlg = false;
	  prev_id = -1;
	}

	inline virtual ~TDlgBaseAnswer(){};

	const wchar_t *CreateXML();

//��� �������� ������� id ��� ������ EndDialog
	__property int PrevLinkedFromID = {read = prev_id, write = prev_id};
//������� � ���, ��� ���� ������� ��������� �����
	__property bool EndDialog = {read = end_dlg, write = end_dlg};
};
//---------------------------------------------------------------------------

class TDlgAnswer : public TDlgBaseAnswer
{
  private:
	String quest_name;
	String need_quest;
	String set_quest;

  public:
	TDlgAnswer(int el_id): TDlgBaseAnswer(el_id){Type = DlgAnsw;}

	TDlgAnswer(int el_id, int dlg_id, int next_dlg_id, int link_id, int link_fr_id)
	: TDlgBaseAnswer(el_id, dlg_id, next_dlg_id, link_id, link_fr_id)
    {
	  Type = DlgAnsw;
	}

	inline virtual ~TDlgAnswer(){};

	const wchar_t *CreateXML();

//��������� ��� ����� ������, � ������� �������� �������
	__property String QuestName = {read = quest_name, write = quest_name};
//�������� ������, ������� ����� ��� ����������� ������
	__property String NeedQuestValue = {read = need_quest, write = need_quest};
//�������� ������, ������� ����� ����������� ��� ������ ������
	__property String SetQuestValue = {read = set_quest, write = set_quest};
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

//��������� ��� ������ ����������
	__property String Params = {read = params, write = params};
//��������� ����������
	__property String Result = {read = result, write = result};
};
//---------------------------------------------------------------------------

class TDlgCondition : public TDlgBaseText
{
  private:
	String cond;
	String script;
    String GetConditionScript();

  public:
	TDlgCondition(int el_id): TDlgBaseText(el_id){Type = DlgCondition;}

	TDlgCondition(int el_id, int dlg_id, int next_dlg_id, int link_id, int link_fr_id)
	: TDlgBaseText(el_id, dlg_id, next_dlg_id, link_id, link_fr_id)
	{
	  Type = DlgCondition;
	}

	virtual ~TDlgCondition(){};

	const wchar_t *CreateXML();

//������� ��������
	__property String Condition = {read = cond, write = cond};
	__property String ConditionScript = {read = GetConditionScript};
};
//---------------------------------------------------------------------------

#endif
