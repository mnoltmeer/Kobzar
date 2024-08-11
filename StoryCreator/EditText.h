//---------------------------------------------------------------------------
/*!
Copyright 2023-2024 Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of Text Scene Creator.

	Text Scene Creator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Text Scene Creator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Text Scene Creator.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#ifndef EditTextH
#define EditTextH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TEditTextForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Text;
	TEdit *Dialog;
	TLabel *Label1;
	TLabel *Label3;
	TEdit *ID;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall IDChange(TObject *Sender);
	void __fastcall DialogChange(TObject *Sender);
    void __fastcall EditExit(TObject *Sender);
	void __fastcall EditKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TextChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TEditTextForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditTextForm *EditTextForm;
//---------------------------------------------------------------------------
#endif
