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

#ifndef EditScriptH
#define EditScriptH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TEditScriptForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Dialog;
	TEdit *ID;
	TLabel *Label3;
	TEdit *Params;
	TLabel *Label2;
	TRichEdit *Text;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TextKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TextKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall EditExit(TObject *Sender);
	void __fastcall EditKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ParamsChange(TObject *Sender);
	void __fastcall TextChange(TObject *Sender);
	void __fastcall IDChange(TObject *Sender);
	void __fastcall DialogChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TEditScriptForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditScriptForm *EditScriptForm;
//---------------------------------------------------------------------------
#endif
