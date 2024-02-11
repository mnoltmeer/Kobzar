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

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Xml.adomxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.Win.msxmldom.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.BaseImageCollection.hpp>
#include <Vcl.ImageCollection.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.AppEvnts.hpp>

//---------------------------------------------------------------------------
class TStoryCreator : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenSchema;
	TSaveDialog *SaveSchema;
	TMainMenu *MainMenu;
	TMenuItem *MenuView;
	TMenuItem *MenuSave;
	TMenuItem *MenuLoad;
	TMenuItem *MenuExportXML;
	TMenuItem *MenuDel;
	TMenuItem *MenuEdit;
	TPopupMenu *MainPopupMenu;
	TMenuItem *MPPScrTxt;
	TMenuItem *MPPAnsw;
	TPopupMenu *ElementPopupMenu;
	TMenuItem *EPPEdit;
	TMenuItem *EPPDel;
	TMenuItem *EPPUnsel;
	TMenuItem *MenuSaveAs;
	TMenuItem *MenuNew;
	TMenuItem *MenuShowItems;
	TMenuItem *MenuInstruments;
	TMenuItem *MenuFile;
	TMenuItem *MenuImportXML;
	TMenuItem *MenuExportText;
	TMenuItem *MenuActs;
	TMenuItem *MenuToolPalette;
	TMenuItem *MenuSettings;
	TMenuItem *MenuAbout;
	TImageCollection *Images;
	TMenuItem *MenuPlaceText;
	TMenuItem *MenuPlaceAnswer;
	TMenuItem *MenuPlaceScript;
	TMenuItem *MenuRecent;
	TMenuItem *MPPScript;
	TPanel *Palette;
	TBitBtn *BtUnselect;
	TBitBtn *BtScreenText;
	TBitBtn *BtAnswer;
	TBitBtn *BtScript;
	TPanel *QuickButtonsPanel;
	TMenuItem *MenuButtonPanel;
	TBitBtn *QBtNew;
	TBitBtn *QBtOpen;
	TBitBtn *QBtSave;
	TBitBtn *QBtImport;
	TBitBtn *QBtExport;
	TMenuItem *EPPStandAlone;
	TPanel *PropsPanel;
	TListBox *ItemList;
	TStringGrid *PropList;
	TApplicationEvents *ApplicationEvents;
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ScrTextClick(TObject *Sender);
	void __fastcall AnswerClick(TObject *Sender);
	void __fastcall MenuDelClick(TObject *Sender);
	void __fastcall SimpleAnswerClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MenuExportXMLClick(TObject *Sender);
	void __fastcall PlaceFreeClick(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall MenuSaveClick(TObject *Sender);
	void __fastcall MenuLoadClick(TObject *Sender);
	void __fastcall MenuEditClick(TObject *Sender);
	void __fastcall MPPScrTxtClick(TObject *Sender);
	void __fastcall MPPAnswClick(TObject *Sender);
	void __fastcall EPPEditClick(TObject *Sender);
	void __fastcall EPPDelClick(TObject *Sender);
	void __fastcall EPPUnselClick(TObject *Sender);
	void __fastcall MenuSaveAsClick(TObject *Sender);
	void __fastcall MenuNewClick(TObject *Sender);
	void __fastcall MenuShowItemsClick(TObject *Sender);
	void __fastcall MenuImportXMLClick(TObject *Sender);
	void __fastcall MenuExportTextClick(TObject *Sender);
	void __fastcall MenuToolPaletteClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall MenuSettingsClick(TObject *Sender);
	void __fastcall MenuAboutClick(TObject *Sender);
	void __fastcall MenuFileClick(TObject *Sender);
	void __fastcall BtUnselectClick(TObject *Sender);
	void __fastcall BtScreenTextClick(TObject *Sender);
	void __fastcall BtAnswerClick(TObject *Sender);
	void __fastcall BtScriptClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall MenuPlaceTextClick(TObject *Sender);
	void __fastcall MenuPlaceAnswerClick(TObject *Sender);
	void __fastcall MenuPlaceScriptClick(TObject *Sender);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall MPPScriptClick(TObject *Sender);
	void __fastcall MenuButtonPanelClick(TObject *Sender);
	void __fastcall QBtNewClick(TObject *Sender);
	void __fastcall QBtOpenClick(TObject *Sender);
	void __fastcall QBtSaveClick(TObject *Sender);
	void __fastcall QBtImportClick(TObject *Sender);
	void __fastcall QBtExportClick(TObject *Sender);
	void __fastcall EPPStandAloneClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ItemListClick(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);



private:	// User declarations
	void __fastcall PreparePalette();

    void __fastcall WriteLastFilesList();
	void __fastcall ReadLastFilesList();
	void __fastcall WriteSettings();
	void __fastcall ReadSettings();
	void __fastcall ShowLastFiles();
	void __fastcall CreateMenuLastFiles();
	void __fastcall SaveCurrentProject(bool confirm);
	void __fastcall AddLastFile(String file);
	void __fastcall PPLastFileClick(TObject *Sender);

public:		// User declarations
	__fastcall TStoryCreator(TComponent* Owner);
	void __fastcall PropsDrawCell(TObject *Sender, int ACol, int ARow,
								  const TRect &Rect, TGridDrawState State);

    void __fastcall ContainerKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);

	void __fastcall ContainerMouseDown(TObject *Sender,
									   TMouseButton Button,
									   TShiftState Shift,
									   int X, int Y);
	void __fastcall ContainerMouseUp(TObject *Sender,
									 TMouseButton Button,
									 TShiftState Shift,
									 int X, int Y);
	void __fastcall ContainerMouseMove(TObject *Sender,
									   TShiftState Shift,
									   int X, int Y);

	void __fastcall ChangeElement(); //используется после изменения свойств елемента
									 //проверяет и перестраивает связи
	void __fastcall ChangeID(int new_val);
	void __fastcall ChangeCardOfDialog(int new_val);
	void __fastcall ChangeNextCardOfDialog(int new_val);
	void __fastcall ChangeLinkedID(int new_val);
	void __fastcall ChangeLinkedFromID(int new_val);
	void __fastcall VisualiseElements();
};
//---------------------------------------------------------------------------
extern PACKAGE TStoryCreator *StoryCreator;
//---------------------------------------------------------------------------

String CreateProjName(String proj_path);
void UpdateItemsList(TListBox *list);

#endif
