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

#ifndef KE_INTERFACE_H_INCLUDED
#define KE_INTERFACE_H_INCLUDED

struct KE_INTERFACE
{
  virtual const wchar_t* __stdcall GetVersion() = 0;
  virtual int __stdcall CreateStory(const wchar_t *story_file) = 0;
  virtual int __stdcall LoadStory(const wchar_t *story_file) = 0;
  virtual int __stdcall SaveStory() = 0;
  virtual void __stdcall CloseStory() = 0;
  virtual void __stdcall ClearStory() = 0;
  virtual int __stdcall AddScene() = 0;
  virtual int __stdcall AddAnswer() = 0;
  virtual int __stdcall AddAdvAnswer() = 0;
  virtual int __stdcall AddScript() = 0;
  virtual int __stdcall AddCondition() = 0;
  virtual int __stdcall ActivateElement(int id) = 0;
  virtual int __stdcall RemoveElement(int id) = 0;
  virtual int __stdcall GetID() = 0;
  virtual int __stdcall SetID(int new_id) = 0;
  virtual int __stdcall GetDialog() = 0;
  virtual int __stdcall SetDialog(int new_id) = 0;
  virtual int __stdcall GetLinkedID() = 0;
  virtual int __stdcall SetLinkedID(int new_id) = 0;
  virtual int __stdcall GetLinkedFromID() = 0;
  virtual int __stdcall SetLinkedFromID(int new_id) = 0;
  virtual int __stdcall GetType() = 0;
  virtual const wchar_t *__stdcall GetTex() = 0;
  virtual void __stdcall SetText(const wchar_t *new_text) = 0;
  virtual int __stdcall IsEndDialog() = 0;
  virtual void __stdcall SetEndDialog(bool end_dialog) = 0;
  virtual const wchar_t *__stdcall GetQuestName() = 0;
  virtual int __stdcall SetQuestName(const wchar_t *quest_name) = 0;
  virtual int __stdcall GetNeedQuestValue() = 0;
  virtual int __stdcall SetNeedQuestValue(int quest_state) = 0;
  virtual int __stdcall GetResultQuestValue() = 0;
  virtual int __stdcall SetResultQuestValue(int quest_state) = 0;
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
