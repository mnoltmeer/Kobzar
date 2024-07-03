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

#ifndef EditAnswerH
#define EditAnswerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TEditAnswerForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *CardOfDialog;
	TMemo *Text;
	TLabel *Label2;
	TEdit *NextCardOfDialog;
	TLabel *Label3;
	TEdit *ID;
	TLabel *Label4;
	TEdit *LinkedID;
	TLabel *Label5;
	TEdit *LinkedFromID;
	TCheckBox *EndDialog;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TextChange(TObject *Sender);
	void __fastcall IDChange(TObject *Sender);
	void __fastcall CardOfDialogChange(TObject *Sender);
	void __fastcall NextCardOfDialogChange(TObject *Sender);
	void __fastcall LinkedIDChange(TObject *Sender);
	void __fastcall LinkedFromIDChange(TObject *Sender);
	void __fastcall EndDialogClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

	void __fastcall EditExit(TObject *Sender);
	void __fastcall EditKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
	void __fastcall ApplyThemeOnGUI();

public:		// User declarations
	__fastcall TEditAnswerForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditAnswerForm *EditAnswerForm;
//---------------------------------------------------------------------------
#endif
