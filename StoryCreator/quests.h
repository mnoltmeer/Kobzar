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

#ifndef questsH
#define questsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ValEdit.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------

class TQuestManager : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *PopupStates;
	TMenuItem *PPAddState;
	TMenuItem *PPDelState;
	TPopupMenu *PopupQuests;
	TMenuItem *PPAddQuest;
	TMenuItem *PPDelQuest;
	TValueListEditor *QuestStates;
	TValueListEditor *QuestList;
	TPanel *Panel1;
	TEdit *QuestLibPath;
	TButton *SetQuestLibPath;
	TOpenDialog *OpenQuestLibFile;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PPAddStateClick(TObject *Sender);
	void __fastcall PPDelStateClick(TObject *Sender);
	void __fastcall PPAddQuestClick(TObject *Sender);
	void __fastcall PPDelQuestClick(TObject *Sender);
	void __fastcall QuestStatesSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall QuestListSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall SetQuestLibPathClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall QuestListValidate(TObject *Sender, int ACol, int ARow, const UnicodeString KeyName,
          const UnicodeString KeyValue);
	void __fastcall QuestStatesValidate(TObject *Sender, int ACol, int ARow, const UnicodeString KeyName,
		  const UnicodeString KeyValue);




private:	// User declarations
public:		// User declarations
	__fastcall TQuestManager(TComponent* Owner);

	void Import(String xml_file);
	void Export(String path);
    void ShowQuests();
};
//---------------------------------------------------------------------------
extern PACKAGE TQuestManager *QuestManager;
//---------------------------------------------------------------------------
#endif
