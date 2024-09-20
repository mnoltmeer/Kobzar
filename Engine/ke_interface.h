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

#ifndef KE_INTERFACE_H_INCLUDED
#define KE_INTERFACE_H_INCLUDED

struct KE_INTERFACE
{
//functions for creating and managing stories
  virtual const wchar_t* __stdcall GetVersion() = 0;
  virtual const wchar_t* __stdcall GetLastError() = 0;
  virtual int __stdcall CreateStory(const wchar_t *story_file) = 0;
  virtual int __stdcall LoadStory(const wchar_t *story_file) = 0;
  virtual int __stdcall SaveStory() = 0;
  virtual void __stdcall CloseStory() = 0;
  virtual void __stdcall ClearStory() = 0;
  virtual int __stdcall AddScene() = 0;
  virtual int __stdcall AddAnswer() = 0;
  virtual int __stdcall AddScript() = 0;
  virtual const wchar_t *__stdcall RunScript(const wchar_t *text) = 0;
  virtual int __stdcall Select(int id) = 0;
  virtual int __stdcall Remove(int id) = 0;
  virtual int __stdcall Link(int id, int to_id) = 0;
  virtual int __stdcall Unlink(int id, int to_id) = 0;
  virtual int __stdcall GetID() = 0;
  virtual void __stdcall SetID(int new_id) = 0;
  virtual int __stdcall GetDialog() = 0;
  virtual void __stdcall SetDialog(int val) = 0;
  virtual int __stdcall GetNextDialog() = 0;
  virtual void __stdcall SetNextDialog(int val) = 0;
  virtual int __stdcall GetPrevID() = 0;
  virtual void __stdcall SetPrevID(int val) = 0;
  virtual int __stdcall GetNextID() = 0;
  virtual void __stdcall SetNextID(int val) = 0;
  virtual int __stdcall GetType() = 0;
  virtual const wchar_t *__stdcall GetText() = 0;
  virtual void __stdcall SetText(const wchar_t *new_text) = 0;
  virtual int __stdcall IsEndDialog() = 0;
  virtual void __stdcall SetEndDialog(bool end_dialog) = 0;
  virtual const wchar_t* __stdcall GetParams() = 0;
  virtual void __stdcall SetParams(const wchar_t *params) = 0;
  virtual int __stdcall Run() = 0;
  virtual const wchar_t* __stdcall GetResult() = 0;

//functions for telling stories
  virtual int __stdcall TellStory(const wchar_t *story_file) = 0;
  virtual int __stdcall LoadDialog(int dlg_id) = 0;
  virtual const wchar_t *__stdcall GetScene() = 0;
  virtual int __stdcall GetAnswerCount() = 0;
  virtual const wchar_t *__stdcall GetAnswer(int index) = 0;
  virtual void __stdcall SelectAnswer(int index) = 0;
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
  DLL_EXPORT int __stdcall GetKEInterface(KE_INTERFACE **eInterface);
  DLL_EXPORT int __stdcall FreeKEInterface(KE_INTERFACE **eInterface);
}

typedef int (__stdcall *GETKEINTERFACE)(KE_INTERFACE **eInterface);
typedef int (__stdcall *FREEKEINTERFACE)(KE_INTERFACE **eInterface);
#endif // ELI_INTERFACE_H_INCLUDED
