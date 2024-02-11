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

#ifndef engineH
#define engineH
//---------------------------------------------------------------------------

#include <windows.h>
#include <vector>

#define BUILD_DLL

#include <System.Classes.hpp>
#include "ke_interface.h"
#include "DlgClass.h"

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
	bool write_log;
	std::vector<TDlgBaseText*> items;

	int GenDialogID();
	int GenElementID();
	void RemoveFromItems(TDlgBaseText *element);
	bool SaveDlgSchema(const wchar_t *file);
	bool LoadDlgSchema(const wchar_t *file);
	void XMLImport(String xml_file);
	void XMLExport(const wchar_t *path);
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

  public:
	KobzarEngine();
	inline virtual ~KobzarEngine(){};

	virtual const wchar_t * __stdcall GetVersion();
    virtual int __stdcall CreateStory(const wchar_t *story_file);
	virtual int __stdcall LoadStory(const wchar_t *story_file);
	virtual int __stdcall SaveStory();
	virtual void __stdcall CloseStory();
	virtual void __stdcall ClearStory();
	virtual int __stdcall AddScene();
	virtual int __stdcall AddAnswer();
	virtual int __stdcall AddScript();
	virtual int __stdcall ActivateElement(int id);
	virtual int __stdcall RemoveElement(int id);
	virtual int __stdcall GetID();
	virtual int __stdcall SetID(int new_id);
	virtual int __stdcall GetDialog();
	virtual int __stdcall SetDialog(int new_id);
	virtual int __stdcall GetLinkedID();
	virtual int __stdcall SetLinkedID(int new_id);
	virtual int __stdcall GetLinkedFromID();
	virtual int __stdcall SetLinkedFromID(int new_id);
	virtual int __stdcall GetType();
	virtual const wchar_t *__stdcall GetTex();
	virtual void __stdcall SetText(const wchar_t *new_text);
	virtual int __stdcall IsEndDialog();
	virtual void __stdcall SetEndDialog(bool end_dialog);
};
//-------------------------------------------------------------------------------
#endif
