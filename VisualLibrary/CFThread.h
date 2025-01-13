/*!
Copyright 2024 Maxim Noltmeer (m.noltmeer@gmail.com)

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

#ifndef CFThreadH
#define CFThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------

class TCFThread : public TThread
{
private:
	HINSTANCE FHinst;
	HWND FHandle;

	void CreateHostWindow();

protected:
	void __fastcall Execute();

public:
	__fastcall TCFThread(bool CreateSuspended);

	void __fastcall OnTerminate(TObject *Sender);

	__property HINSTANCE Instance = {read = FHinst, write = FHinst};
	__property HWND WindowHandle = {read = FHandle};
};

LRESULT CALLBACK HostWindowProcedure(HWND, UINT, WPARAM, LPARAM);
//---------------------------------------------------------------------------
#endif
