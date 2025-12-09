#class Kobzar
{
  #property NewInstance = 0;
  #property LibraryHandle = 0;
   
  #public property Initialised = 0;

  #method Init()
  {
    &$this.LibraryHandle = _ConnectLib("C:\Users\mnolt\OneDrive\GIT\Kobzar\ELIConnector\Win32\Release\eliKobzar.dll");

    if (&$this.LibraryHandle != -1)
      {
        _ImportFunc(&$this.LibraryHandle, "eGetEngineHandle", "_GetEngineHandle", "");
        _ImportFunc(&$this.LibraryHandle, "eCreateEngineHandle", "_CreateEngineHandle", "sym pFile");
        _ImportFunc(&$this.LibraryHandle, "eFreeEngineHandle", "_FreeEngineHandle", "");
	 
        _ImportFunc(&$this.LibraryHandle, "eGetLastError", "_GetLastError", "");
        _ImportFunc(&$this.LibraryHandle, "eCreateStory", "_CreateStory", "sym pFile");
        _ImportFunc(&$this.LibraryHandle, "eLoadStory", "_LoadStory", "sym pFile");
        _ImportFunc(&$this.LibraryHandle, "eSaveStory", "_SaveStory", "");
        _ImportFunc(&$this.LibraryHandle, "eCloseStory", "_CloseStory", "");
        _ImportFunc(&$this.LibraryHandle, "eClearStory", "_ClearStory", "");
        _ImportFunc(&$this.LibraryHandle, "eAddScene", "_AddScene", "");
        _ImportFunc(&$this.LibraryHandle, "eAddAnswer", "_AddAnswer", "");
        _ImportFunc(&$this.LibraryHandle, "eAddScript", "_AddScript", "");
        _ImportFunc(&$this.LibraryHandle, "eSelect", "_Select", "num pID");
        _ImportFunc(&$this.LibraryHandle, "eRemove", "_Remove", "num pID");
        _ImportFunc(&$this.LibraryHandle, "eLink", "_Link", "num pFrID,num pToID");
        _ImportFunc(&$this.LibraryHandle, "eUnlink", "_Unlink", "num pFrID,num pToID");
        _ImportFunc(&$this.LibraryHandle, "eGetID", "_GetID", "");
        _ImportFunc(&$this.LibraryHandle, "eSetID", "_SetID", "num pValue");
        _ImportFunc(&$this.LibraryHandle, "eGetDialog", "_GetDialog", "");
        _ImportFunc(&$this.LibraryHandle, "eSetDialog", "_SetDialog", "num pValue");
        _ImportFunc(&$this.LibraryHandle, "eGetNextDialog", "_GetNextDialog", "");
        _ImportFunc(&$this.LibraryHandle, "eSetNextDialog", "_SetNextDialog", "num pValue");
        _ImportFunc(&$this.LibraryHandle, "eGetPrevID", "_GetPrevID", "");
        _ImportFunc(&$this.LibraryHandle, "eSetPrevID", "_SetPrevID", "num pValue");
        _ImportFunc(&$this.LibraryHandle, "eGetNextID", "_GetNextID", "");
        _ImportFunc(&$this.LibraryHandle, "eSetNextID", "_SetNextID", "num pValue");
        _ImportFunc(&$this.LibraryHandle, "eGetType", "_GetType", "");
        _ImportFunc(&$this.LibraryHandle, "eGetText", "_GetText", "");
        _ImportFunc(&$this.LibraryHandle, "eSetText", "_SetText", "sym pValue");
        _ImportFunc(&$this.LibraryHandle, "eIsEndDialog", "_IsEndDialog", "");
        _ImportFunc(&$this.LibraryHandle, "eSetEndDialog", "_SetEndDialog", "num pValue");
        _ImportFunc(&$this.LibraryHandle, "eGetParams", "_GetParams", "");
        _ImportFunc(&$this.LibraryHandle, "eSetParams", "_SetParams", "sym pValue");
        _ImportFunc(&$this.LibraryHandle, "eExecute", "_Execute", "");
        _ImportFunc(&$this.LibraryHandle, "eGetResult", "_GetResult", "");
	
        _ImportFunc(&$this.LibraryHandle, "eTellStory", "_TellStory", "sym pFile");
        _ImportFunc(&$this.LibraryHandle, "eLoadDialog", "_LoadDialog", "num pID");
        _ImportFunc(&$this.LibraryHandle, "eGetScene", "_GetScene", "");
        _ImportFunc(&$this.LibraryHandle, "eGetAnswerCount", "_GetAnswerCount", "");
        _ImportFunc(&$this.LibraryHandle, "eGetAnswerCount", "_GetAnswer", "num pIndex");
        _ImportFunc(&$this.LibraryHandle, "eSelectAnswer", "_SelectAnswer", "num pIndex");
      }
    else
      {_throw("KobzarScripts: Error loading connector library!");}
  }
 //===========================================================;
 
  #public method UseCurrrentInstance()
  {
    if (_GetEngineHandle())
      {&$this.Initialised = 1;}
    else
      {
        &$this.Initialised = 0;
        _throw("KobzarScripts: Error getting engine instance!");
      }
  }
//===========================================================;
   
  #public method CreateNewInstance($file)
  {
    if (_CreateEngineHandle($file))
      {
        &$this.Initialised = 1;
        &$this.NewInstance = 1;
      }
    else
      {
        &$this.Initialised = 0;
        _throw("KobzarScripts: Error creating engine instance!");
      }
  }
//===========================================================;
   
  #public method Kobzar(){&$this.Init();}
//===========================================================;
 
  #public method ~Kobzar()
  {
    if (&$this.NewInstance)
      {_FreeEngineHandle();}
	   
    _FreeLib(&$this.LibraryHandle);
  }
//===========================================================;

  #public method GetLastError()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetLastError();}
  }
//===========================================================;

  #public method CreateStory($file)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _CreateStory($file);}
  }
//===========================================================;

  #public method LoadStory($file)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _LoadStory($file);}
  }
//===========================================================;

  #public method SaveStory()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SaveStory();}
  }
//===========================================================;

  #public method CloseStory()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _CloseStory();}
  }
//===========================================================;

  #public method ClearStory()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _ClearStory();}
  }
//===========================================================;

  #public method AddScene()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _AddScene();}
  }
//===========================================================;

  #public method AddAnswer()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _AddAnswer();}
  }
//===========================================================;

  #public method AddScript()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _AddScript();}
  }
//===========================================================;

  #public method Select($id)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _Select($id);}
  }
//===========================================================;

  #public method Remove($id)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _Remove($id);}
  }
//===========================================================;

  #public method Link($frID, $toID)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _Link($frID, $toID);}
  }
//===========================================================;

  #public method Unlink($frID, $toID)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return Unlink($frID, $toID);}
  }
//===========================================================;

  #public method GetID()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetID();}
  }
//===========================================================;

  #public method SetID($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetID($val);}
  }
//===========================================================;

  #public method GetDialog()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetDialog();}
  }
//===========================================================;

  #public method SetDialog($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetDialog($val);}
  }
//===========================================================;

  #public method GetNextDialog()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetNextDialog();}
  }
//===========================================================;

  #public method SetNextDialog($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetNextDialog($val);}
  }
//===========================================================;

  #public method GetPrevID()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetPrevID();}
  }
//===========================================================;

  #public method SetPrevID($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetPrevID($val);}
  }
//===========================================================;

  #public method GetNextID()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetNextID();}
  }
//===========================================================;

  #public method SetNextID($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetNextID($val);}
  }
//===========================================================;

  #public method GetType()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetType();}
  }
//===========================================================;

  #public method GetText()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetText();}
  }
//===========================================================;

  #public method SetText($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetText($val);}
  }
//===========================================================;

  #public method IsEndDialog()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _IsEndDialog();}
  }
//===========================================================;

  #public method SetEndDialog($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetEndDialog($val);}
  }
//===========================================================;

  #public method GetParams()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetParams();}
  }
//===========================================================;

  #public method SetParams($val)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SetParams($val);}
  }
//===========================================================;

  #public method Execute()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _Execute();}
  }
//===========================================================;

  #public method GetResult()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetResult();}
  }
//===========================================================;

  #public method TellStory($file)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _TellStory($file);}
  }
//===========================================================;

  #public method LoadDialog($id)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _LoadDialog($id);}
  }
//===========================================================;

  #public method GetScene()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetScene();}
  }
//===========================================================;

  #public method GetAnswerCount()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetAnswerCount();}
  }
//===========================================================;

  #public method GetAnswer()
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _GetAnswer();}
  }
//===========================================================;

  #public method SelectAnswer($ind)
  {
    if (!&$this.Initialised)
      {_throw("KobzarScripts: Engine not initialised!");}
    else
      {#return _SelectAnswer($ind);}
  }
//===========================================================;   
}