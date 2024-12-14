/*!
Copyright 2017-2019, 2021 Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of Extern Logic Interpreter.

    Extern Logic Interpreter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Extern Logic Interpreter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Extern Logic Interpreter.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ELI_INTERFACE_H_INCLUDED
#define ELI_INTERFACE_H_INCLUDED

typedef void (__stdcall *func_ptr)(void*);   //��������� ��������� �� �-� �� ��������� ����������

struct ELI_INTERFACE
{
//��������� ������� � ����
	virtual void __stdcall AddFunction(const wchar_t *name, const wchar_t *params, func_ptr fptr) = 0;
//������� ������� �� �����
	virtual void __stdcall DeleteFunction(const wchar_t *name) = 0;
//�������� �������
	virtual void __stdcall CallFunction(const wchar_t *name) = 0;
//����������� ������������ �������� ������� � ������ � ���������� ��������� �� ���
//� ������ ������ ���������� NULL
	virtual wchar_t* __stdcall GetFunctionResult(const wchar_t *name) = 0;
//������������� ������������ �������� �������
	virtual void __stdcall SetFunctionResult(const wchar_t *name, const wchar_t* result) = 0;
//������������� ����� �������� ��������� ��� ��������� ����� ��������
	virtual void __stdcall SetParam(const wchar_t *name, const wchar_t *new_val) = 0;
//����������� �������� � integer � ���������� ���
	virtual int __stdcall GetParamToInt(const wchar_t *name) = 0;
//����������� �������� � float � ���������� ���
	virtual float __stdcall GetParamToFloat(const wchar_t *name) = 0;
//����������� �������� � ������ � ���������� ��������� �� ���
//� ������ ������ ���������� NULL
	virtual const wchar_t* __stdcall GetParamToStr(const wchar_t *name) = 0;

	virtual const wchar_t* __stdcall GetVersion() = 0;
	virtual const wchar_t* __stdcall ShowVarStack() = 0;
	virtual const wchar_t* __stdcall ShowObjStack() = 0;
	virtual const wchar_t* __stdcall ShowClassStack() = 0;
	virtual const wchar_t* __stdcall ShowProcStack() = 0;
	virtual const wchar_t* __stdcall ShowInfoMessages() = 0;
	virtual const wchar_t* __stdcall RunScript(const wchar_t *imptext,
											   const wchar_t *parameter,
											   bool log) = 0;
	virtual const wchar_t* __stdcall RunScriptFromFile(const wchar_t *filepath,
													   const wchar_t *parameter,
													   bool log) = 0;
	virtual const wchar_t* __stdcall ShowFuncStack() = 0;
	virtual const wchar_t* __stdcall ShowParamStack() = 0;
	virtual const wchar_t* __stdcall ShowFragmentStack() = 0;
	virtual const wchar_t* __stdcall GetCurrentFuncName() = 0;
	virtual void __stdcall SetDebug(bool enable_dbg, bool in_file) = 0;
	virtual bool __stdcall DebugEnabled() = 0;
//���������� ���� � ��������, �� �������� ������� ELI
	virtual const wchar_t* __stdcall GetInitDir() = 0;
    virtual void __stdcall AddToLog(const wchar_t *msg) = 0;
};
//-------------------------------------------------------------------------------

#if defined(BUILD_DLL)
	#define DLL_EXPORT __declspec(dllexport)
#else
	#if defined(BUILD_APP)
		#define DLL_EXPORT __declspec(dllimport)
	#else
		#define DLL_EXPORT
	#endif
#endif

extern "C"
{
  DLL_EXPORT int __stdcall GetELIInterface(ELI_INTERFACE **eInterface);
  DLL_EXPORT int __stdcall FreeELIInterface(ELI_INTERFACE **eInterface);
}

typedef int (__stdcall *GETELIINTERFACE)(ELI_INTERFACE **eInterface);
typedef int (__stdcall *FREEELIINTERFACE)(ELI_INTERFACE **eInterface);
#endif // ELI_INTERFACE_H_INCLUDED
