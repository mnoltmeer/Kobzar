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

#pragma hdrstop

#include "QuestLibrary.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

QuestRecord *TQuest::GetRec(int ind)
{
  QuestRecord *res;

  if (ind < recs.size())
	res = &recs[ind];
  else
	{
	  res = NULL;
	  throw Exception("TQuest::Records: Out of bounds!");
	}

  return res;
}
//---------------------------------------------------------------------------

void TQuest::SetRec(int ind, QuestRecord *val)
{
  if (ind < recs.size())
	recs[ind] = *val;
  else
	throw Exception("TQuest::Records: Out of bounds!");
}
//---------------------------------------------------------------------------

TStringList *TQuest::GetRecStr()
{
  TStringList *res;

  try
	 {
	   recstrs->Clear();

	   for (int i = 0; i < recs.size(); i++)
		  {
			recstrs->Add(recs[i].State + "=" + recs[i].JournalRecord);
		  }

	   res = recstrs;
	 }
  catch (Exception &e)
	 {
	   res = NULL;
	   throw Exception("TQuest::RecordStrings: " + e.ToString());
	 }

  return res;
}
//---------------------------------------------------------------------------

void TQuest::AddRecord(const String &state, const String &journal_entry)
{
  try
	 {
	   recs.push_back(QuestRecord(state, journal_entry));
	 }
  catch (Exception &e)
	 {
	   throw Exception("TQuest::AddRecord: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void TQuest::RemoveRecord(const String &state)
{
  try
	 {
	   int ind = IndexOf(state);

	   if (ind > -1)
		 recs.erase(recs.begin() + ind);
	   else
		 throw Exception("Unknown Record state");
	 }
  catch (Exception &e)
	 {
	   throw Exception("TQuest::RemoveRecord: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

int TQuest::IndexOf(const String &state)
{
  int ind = -1;

  for (int i = 0; i < recs.size(); i++)
	 {
	   if (recs[i].State == state)
		 {
		   ind = i;
		   break;
		 }
	 }

  return ind;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
TQuest *TQuestLibrary::GetItem(int ind)
{
  TQuest *res;

  if (ind < itms.size())
	res = itms[ind];
  else
	{
	  res = NULL;
	  throw Exception("TQuestLibrary::Items: Out of bounds!");
	}

  return res;
}
//---------------------------------------------------------------------------

void TQuestLibrary::SetItem(int ind, TQuest *val)
{
  if (ind < itms.size())
	itms[ind] = val;
  else
	throw Exception("TQuestLibrary::Items: Out of bounds!");
}
//---------------------------------------------------------------------------

int TQuestLibrary::IndexOf(const String &id)
{
  int ind = -1;

  for (int i = 0; i < itms.size(); i++)
	 {
	   if (itms[i]->ID == id)
		 {
		   ind = i;
		   break;
		 }
	 }

  return ind;
}
//---------------------------------------------------------------------------

void TQuestLibrary::AddQuest(TQuest *quest)
{
  itms.push_back(quest);
}
//---------------------------------------------------------------------------

void TQuestLibrary::RemoveQuest(const String &id)
{
  int ind = IndexOf(id);

  if (ind > -1)
	{
	  delete itms[ind];
      itms.erase(itms.begin() + ind);
	}
  else
	throw Exception("TQuestLibrary::RemoveQuest: Unknown ID");
}
//---------------------------------------------------------------------------

void TQuestLibrary::Clear()
{
  for (int i = 0; i < itms.size(); i++)
	 delete itms[i];

  itms.clear();
}
//---------------------------------------------------------------------------
