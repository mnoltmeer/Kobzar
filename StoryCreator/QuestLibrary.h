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

#ifndef QuestLibraryH
#define QuestLibraryH

#include <System.Classes.hpp>
#include <vector>
//---------------------------------------------------------------------------

struct QuestRecord
{
  String State;
  String JournalRecord;

  QuestRecord(const String &state,
			  const String &jrn_rec)
  {
	State = state;
    JournalRecord = jrn_rec;
  }
};
//---------------------------------------------------------------------------

class TQuest
{
  private:
	String id;
	String name;
	std::vector<QuestRecord> recs;
	TStringList *recstrs;

	String GetID(){return id;}
	void SetID(const String &val){id = val;}

	String GetName(){return name;}
	void SetName(const String &val){name = val;}

	QuestRecord *GetRec(int ind);
	void SetRec(int ind, QuestRecord *val);

	int GetCount(){return recs.size();}

	TStringList *GetRecStr();

  public:
	TQuest(const String &id, const String &name)
	{
	  this->id = id;
	  this->name = name;
	  recstrs = new TStringList();
	}

	virtual ~TQuest(){recs.clear(); delete recstrs;}

	void AddRecord(const String &state, const String &journal_entry);
	void RemoveRecord(const String &state);
	int IndexOf(const String &state);

	__property String ID = {read = GetID, write = SetID};
	__property String Name = {read = GetName, write = SetName};
	__property QuestRecord *Records[int ind] = {read = GetRec, write = SetRec};
	__property int Count = {read = GetCount};
	__property TStringList *RecordStrings = {read = GetRecStr};
};
//---------------------------------------------------------------------------

class TQuestLibrary
{
  private:
	std::vector<TQuest*> itms;

	TQuest *GetItem(int ind);
	void SetItem(int ind, TQuest *val);

	int GetCount(){return itms.size();}

  public:
	TQuestLibrary(){};
	virtual ~TQuestLibrary(){Clear();}


	int IndexOf(const String &id);
	void AddQuest(TQuest *quest);
	void RemoveQuest(const String &id);
	void Clear();

	__property TQuest *Items[int ind] = {read = GetItem, write = SetItem};
	__property int Count = {read = GetCount};
};
//---------------------------------------------------------------------------

#endif
