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
const wchar_t *TDlgAnswer::CreateXML()
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


