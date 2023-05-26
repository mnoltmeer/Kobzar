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

#pragma hdrstop

#include <Xml.adomxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.Win.msxmldom.hpp>
#include <vector>

#include "..\..\work-functions\MyFunc.h"
#include "DlgClass.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TDlgBaseText::TDlgBaseText(int el_id)
{
  id = el_id;
  cd = -1;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int el_id, int dlg_id)
{
  id = el_id;
  cd = dlg_id;
  ncd = -1;
  l_id = -1;
  l_fr_id = -1;
}
//---------------------------------------------------------------------------

TDlgBaseText::TDlgBaseText(int el_id, int dlg_id, int next_dlg_id, int link_id, int link_fr_id)
{
  id = el_id;
  cd = dlg_id;
  ncd = next_dlg_id;
  l_id = link_id;
  l_fr_id = link_fr_id;
}
//---------------------------------------------------------------------------

/*
int TDlgBaseText::GetLinkedID()
{
  return l_id;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetLinkedID(int val)
{
  if (Cathegory == DLG_TEXT_LIKE)
	return;

  TDlgBaseText *lnk = Library->FindElement(val);

  if (lnk && (lnk->Cathegory == DLG_TEXT_LIKE))
	{
	  l_id = val;
	  cd = lnk->CardOfDialog;
	}
  else if (lnk && (lnk->Cathegory != DLG_TEXT_LIKE))
	{
	  MessageBox(Application->Handle,
				 String("No TEXT_LIKE element with ID = LinkedID (" +
				 IntToStr(val) + ")").c_str(),
				 String("Element ID: " + IntToStr(ID) + " report").c_str(),
				 MB_OK | MB_ICONERROR);
	}
  else
	{
	  if (MessageBox(Application->Handle,
					 String("No TEXT_LIKE element with ID = LinkedID (" +
					 IntToStr(val) +
					 ").\nSet value anyway?").c_str(),
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes)
		{
		  l_id = val;
          cd = -1;
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetLinkedFromID()
{
  return l_fr_id;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetLinkedFromID(int val)
{
  if (Cathegory == DLG_TEXT_LIKE)
	return;

  TDlgBaseText *lnk = Library->FindElement(val);

  if (lnk && (lnk->Cathegory == DLG_TEXT_LIKE))
	{
	  l_fr_id = val;
	  ncd = lnk->CardOfDialog;
	}
  else if (lnk && (lnk->Cathegory != DLG_TEXT_LIKE))
	{
	  MessageBox(Application->Handle,
				 String("Element with ID = LinkedFromID (" +
				 IntToStr(val) + ") is not TEXT_LIKE").c_str(),
				 String("Element ID: " + IntToStr(ID) + " report").c_str(),
				 MB_OK | MB_ICONERROR);
	}
  else
	{
	  if (MessageBox(Application->Handle,
					 String("No TEXT_LIKE element with ID = LinkedFromID (" +
							IntToStr(val) +
							").\nSet value anyway?").c_str(),
					 String("Element ID: " + IntToStr(ID) + " report").c_str(),
					 MB_YESNO | MB_ICONWARNING) == mrYes)
		{
		  l_fr_id = val;
          ncd = -1;
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetCardOfDialog()
{
  return cd;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetCardOfDialog(int val)
{
  int old = cd;

  if (Cathegory != DLG_TEXT_LIKE)
	{
	  cd = val;
	  LinkedID = Library->FindTextElementID(val);
	}
  else
	{
	  cd = val;
	  std::vector<TDlgBaseText*> lnks;

	  if (Library->FindAnswersByDialog(cd, &lnks) > 0)
		{
		  if (MessageBox(Application->Handle,
						 String("Founded ANSW_LIKE elements with CardOfDialog = " +
								IntToStr(cd) +
								".\nCreate links?").c_str(),
						 String("Element ID: " + IntToStr(ID) + " report").c_str(),
						 MB_YESNO | MB_ICONASTERISK) == mrYes)
			{
			  for (int i = 0; i < lnks.size(); i++)
				 lnks[i]->LinkedID = ID;
			}
		}

	  if ((old != cd) && (FLibrary->SearchDependeciesDialog(old) > 0))
		{
          if (MessageBox(Application->Handle,
						 String("Founded links of old ANSW_LIKE elements.\nRebuild this links?").c_str(),
						 String("Element ID: " + IntToStr(ID) + " report").c_str(),
						 MB_YESNO | MB_ICONASTERISK) == mrYes)
			{
			  Library->UpdateCardOfDialog(old, CardOfDialog);
			}
		  else
			{
			  Library->UpdateLinkedID(ID, -1);
			}
		}
	}
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetNextCardOfDialog()
{
  return ncd;
}
//---------------------------------------------------------------------------

void TDlgBaseText::SetNextCardOfDialog(int val)
{
  if (Cathegory != DLG_TEXT_LIKE)
	{
	  ncd = val;
	  int new_dlg_id = Library->FindTextElementID(ncd);

	  if (new_dlg_id > -1)
		{
		  TDlgBaseText *ndlg = Library->FindElement(new_dlg_id);
		  ndlg->LinkedID = ID;
		  LinkedFromID = ndlg->ID;
		}
	}
}
//---------------------------------------------------------------------------
*/

const wchar_t *TDlgBaseText::GetInfo()
{
  String res;

  std::unique_ptr<TStringList> lst(new TStringList());

  lst->Add("ID = " + String(ID));
  lst->Add("LinkedID = " + String(LinkedID));
  lst->Add("LinkedFromID = " + String(LinkedFromID));
  lst->Add("CardOfDialog = " + String(CardOfDialog));
  lst->Add("NextCardOfDialog = " + String(NextCardOfDialog));

  res = lst->Text;

  res.Delete(res.Length() - 1, 2);

  return res.c_str();
}
//---------------------------------------------------------------------------

void TDlgBaseText::GiveInfo(TStrings *lst)
{
  lst->Add(String(ID));
  lst->Add(String(LinkedID));
  lst->Add(String(LinkedFromID));
  lst->Add(String(CardOfDialog));
  lst->Add(String(NextCardOfDialog));
}
//---------------------------------------------------------------------------

int TDlgBaseText::GetCathegory()
{
  int res;

  switch (Type)
	{
	  case DlgText: res = DLG_TEXT_LIKE; break;
	  case DlgScript: res = DLG_TEXT_LIKE; break;
	  case DlgAnsw: res = DLG_ANSW_LIKE; break;
	  case DlgSimpleAnsw: res = DLG_ANSW_LIKE; break;
	  case DlgCondition: res = DLG_ANSW_LIKE; break;
	}

  return res;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgScreenText::CreateXML()
{
  XMLText = "\t\t<ScreenTextMassive>\r\n\t\t\t<ScreenText>\r\n";
  XMLText = XMLText + "\t\t\t\t<Text>" + Text + "</Text>\r\n";
  XMLText = XMLText + "\t\t\t</ScreenText>\r\n\t\t</ScreenTextMassive>\r\n";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgBaseAnswer::CreateXML()
{
  XMLText = "\t\t\t<Answer NextCardOfDialog = '" + String(NextCardOfDialog) + "'>\r\n";
  XMLText = XMLText + "\t\t\t\t<TextOfAnswer>" + Text + "</TextOfAnswer>\r\n";

  if (EndDialog)
	XMLText = XMLText + "\t\t\t\t<EndDialog>True</EndDialog>\r\n";

  XMLText = XMLText + "\t\t\t</Answer>";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgAnswer::CreateXML()
{
  XMLText = "\t\t\t<Answer NextCardOfDialog = '" + String(NextCardOfDialog) + "'";

  if (SetQuestValue != "")
	XMLText = XMLText + " SetQuestValue = '" + SetQuestValue + "'";

  if (NeedQuestValue != "")
	XMLText = XMLText + " NeedQuestValue = '" + NeedQuestValue + "'";

  XMLText = XMLText + ">\r\n";

  XMLText = XMLText + "\t\t\t\t<TextOfAnswer>" + Text + "</TextOfAnswer>\r\n";

  XMLText = XMLText + "\t\t\t\t<QuestName>" + QuestName + "</QuestName>\r\n";

  if (EndDialog)
	XMLText = XMLText + "\t\t\t\t<EndDialog>True</EndDialog>\r\n";

  XMLText = XMLText + "\t\t\t</Answer>";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const wchar_t *TDlgScript::CreateXML()
{
  XMLText = "\t\t<ScreenTextMassive>\r\n";
  XMLText = XMLText + "\t\t\t<Script Params = '" + Params + "'>\r\n";
  XMLText = XMLText + "\t\t\t\t<Text>" + Text + "</Text>\r\n";
  XMLText = XMLText + "\t\t\t\t<Return>" + Result + "</Return>\r\n";
  XMLText = XMLText + "\t\t\t</Script>\r\n";
  XMLText = XMLText + "\t\t</ScreenTextMassive>\r\n";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
String TDlgCondition::GetConditionScript()
{
  script = "#begin Condition;\r\n";
  script = script + "if (" + cond + "){_return(1);}\r\n";
  script = script + "else{_return(0);}\r\n";
  script = script + "#end;\r\n";

  return script;
}
//---------------------------------------------------------------------------

const wchar_t *TDlgCondition::CreateXML()
{
  XMLText = "\t\t\t<Condition NextCardOfDialog = '" + String(NextCardOfDialog) + "'>\r\n";
  XMLText = XMLText + "\t\t\t\t<Check>" + Condition + "</Check>\r\n";
  XMLText = XMLText + "\t\t\t\t<TextOfAnswer>" + Text + "</TextOfAnswer>\r\n";
  XMLText = XMLText + "\t\t\t</Condition>";

  return XMLText.c_str();
}
//---------------------------------------------------------------------------

