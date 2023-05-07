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

#ifndef EditAdvAnswerH
#define EditAdvAnswerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TEditAdvAnswerForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *CardOfDialog;
	TMemo *Text;
	TBitBtn *Confirm;
	TBitBtn *Discard;
	TLabel *Label2;
	TEdit *NextCardOfDialog;
	TLabel *Label3;
	TEdit *ID;
	TLabel *Label4;
	TEdit *LinkedID;
	TLabel *Label5;
	TEdit *LinkedFromID;
	TLabel *Label6;
	TEdit *NeedQuestValue;
	TLabel *Label7;
	TEdit *SetQuestValue;
	TEdit *QuestName;
	TLabel *Label8;
	TButton *BtSelectQuest;
	TCheckBox *EndDialog;
	TLabel *Label9;
	TButton *BtNeedQuest;
	TButton *BtSetQuest;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ConfirmClick(TObject *Sender);
	void __fastcall DiscardClick(TObject *Sender);
	void __fastcall BtSelectQuestClick(TObject *Sender);
	void __fastcall BtNeedQuestClick(TObject *Sender);
	void __fastcall BtSetQuestClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TEditAdvAnswerForm(TComponent* Owner);

	void __fastcall UpdateInfo();
};
//---------------------------------------------------------------------------
extern PACKAGE TEditAdvAnswerForm *EditAdvAnswerForm;
//---------------------------------------------------------------------------
#endif
