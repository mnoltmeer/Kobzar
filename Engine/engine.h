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

#ifndef engineH
#define engineH
//---------------------------------------------------------------------------

#include <windows.h>
#include <vector>

#define BUILD_DLL

#include <System.Classes.hpp>
#include "ke_interface.h"
#include "DlgClass.h"
#include "eli_script.h"

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

String LogPath;
wchar_t path[4096];

class KobzarEngine: public KE_INTERFACE
{
  private:
	std::vector<TDlgBaseText*> items;
	String LastError;
    String CurrentFile;
	TDlgBaseText *ActiveItem;

	void CreateLog(const String &method_name, const String &error);
	void LoadFunctionsToELI(ELI_INTERFACE *FEIface);

  //функції для створення Розповіді
	int GenDialogID();
	int GenElementID();
	void RemoveFromItems(TDlgBaseText *element);
	bool SaveDlgSchema(String file);
	bool LoadDlgSchema(String file);
	void XMLImport(String xml_file);
	void XMLExport(String xml_file);
	void ClearItems();
	void BuildLinksAfterXMLImport();
	void RemoveLimboLinks();
	TDlgBaseText *FindElement(int id);
	int FindLinkedElements(int id, std::vector<TDlgBaseText*> *el_list);
	int FindAnswersByDialog(int id, std::vector<TDlgBaseText*> *el_list);
	int FindTextElementID(int crd_dlg);
    int SearchDependeciesID(int id);
	int SearchDependeciesDialog(int id);
	void UpdateLinkedID(int old_id, int new_id);
	void UpdateCardOfDialog(int old_val, int new_val);

  //функції візуалізації

  public:
	KobzarEngine();
	inline virtual ~KobzarEngine(){};

	virtual const wchar_t * __stdcall GetVersion();
    virtual const wchar_t* __stdcall GetLastError();
	virtual int __stdcall CreateStory(const wchar_t *story_file);
	virtual int __stdcall LoadStory(const wchar_t *story_file);
	virtual int __stdcall SaveStory();
	virtual void __stdcall CloseStory();
	virtual void __stdcall ClearStory();
	virtual int __stdcall AddScene();
	virtual int __stdcall AddAnswer();
	virtual int __stdcall AddScript();
	virtual int __stdcall Activate(int id);
	virtual int __stdcall RunScript(int id);
	virtual int __stdcall Remove(int id);
	virtual int __stdcall Link(int id, int to_id);
	virtual int __stdcall Unlink(int id, int to_id);
	virtual int __stdcall GetID();
	virtual void __stdcall SetID(int val);
	virtual int __stdcall GetDialog();
	virtual void __stdcall SetDialog(int val);
	virtual int __stdcall GetNextDialog();
	virtual void __stdcall SetNextDialog(int val);
	virtual int __stdcall GetLinkedID();
	virtual void __stdcall SetLinkedID(int val);
	virtual int __stdcall GetLinkedFromID();
	virtual void __stdcall SetLinkedFromID(int val);
	virtual int __stdcall GetType();
	virtual const wchar_t *__stdcall GetText();
	virtual void __stdcall SetText(const wchar_t *val);
	virtual int __stdcall IsEndDialog();
	virtual void __stdcall SetEndDialog(bool val);
	virtual const wchar_t* __stdcall GetParams();
	virtual void __stdcall SetParams(const wchar_t *val);
	virtual const wchar_t* __stdcall GetResult();

	virtual int __stdcall TellStory(const wchar_t *story_file);
	virtual const wchar_t *__stdcall GetScene();
	virtual int __stdcall GetAnswerCount();
	virtual const wchar_t *__stdcall GetAnswer(int index);
    virtual void __stdcall SelectAnswer(int index);

    static void __stdcall eFoo(void *p);
};
//-------------------------------------------------------------------------------
#endif
