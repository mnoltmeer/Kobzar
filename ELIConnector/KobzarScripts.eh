#class Kobzar
{
   #property NewInstance = 0;
   #property LibraryHandle = 0;
   #property Initialised = 0;
   
   #method Init()
   {
     &$this.LibraryHandle = _ConnectLib(.\lib\eliKobzar.dll);

	 if (&$this.LibraryHandle != -1)
	   {
         _ImportFunc(&$this.LibraryHandle, 'eGetEngineHandle', '_GetEngineHandle', '');
	     _ImportFunc(&$this.LibraryHandle, 'eCreateEngineHandle', '_CreateEngineHandle', '');
	     _ImportFunc(&$this.LibraryHandle, 'eFreeEngineHandle', '_FreeEngineHandle', '');
	 
	     _ImportFunc(&$this.LibraryHandle, 'eGetLastError', '_GetLastError', '');
	     _ImportFunc(&$this.LibraryHandle, 'eCreateStory', '_CreateStory', 'sym pFile');
		 _ImportFunc(&$this.LibraryHandle, 'eLoadStory', '_LoadStory', 'sym pFile');
	     _ImportFunc(&$this.LibraryHandle, 'eSaveStory', '_SaveStory', '');
	     _ImportFunc(&$this.LibraryHandle, 'eCloseStory', '_CloseStory', '');
	     _ImportFunc(&$this.LibraryHandle, 'eClearStory', '_ClearStory', '');
	     _ImportFunc(&$this.LibraryHandle, 'eAddScene', '_AddScene', '');
	     _ImportFunc(&$this.LibraryHandle, 'eAddAnswer', '_AddAnswer', '');
	     _ImportFunc(&$this.LibraryHandle, 'eAddScript', '_AddScript', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSelect', '_Select', 'num pID');
	     _ImportFunc(&$this.LibraryHandle, 'eRemove', '_Remove', 'num pID');
	     _ImportFunc(&$this.LibraryHandle, 'eLink', '_Link', 'num pFrID,num pToID');
	     _ImportFunc(&$this.LibraryHandle, 'eUnlink', '_Unlink', 'num pFrID,num pToID');
	     _ImportFunc(&$this.LibraryHandle, 'eGetID', '_GetID', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetID', '_SetID', 'num pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eGetDialog', '_GetDialog', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetDialog', '_SetDialog', 'num pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eGetNextDialog', '_GetNextDialog', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetNextDialog', '_SetNextDialog', 'num pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eGetPrevID', '_GetPrevID', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetPrevID', '_SetPrevID', 'num pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eGetNextID', '_GetNextID', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetNextID', '_SetNextID', 'num pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eGetType', '_GetType', '');
	     _ImportFunc(&$this.LibraryHandle, 'eGetText', '_GetText', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetText', '_SetText', 'sym pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eIsEndDialog', '_IsEndDialog', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetEndDialog', '_SetEndDialog', 'num pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eGetParams', '_GetParams', '');
	     _ImportFunc(&$this.LibraryHandle, 'eSetParams', '_SetParams', 'sym pValue');
	     _ImportFunc(&$this.LibraryHandle, 'eRun', '_Run', '');
	     _ImportFunc(&$this.LibraryHandle, 'eGetResult', '_GetResult', '');
	
	     _ImportFunc(&$this.LibraryHandle, 'eTellStory', '_TellStory', 'sym pFile');
	     _ImportFunc(&$this.LibraryHandle, 'eLoadDialog', '_LoadDialog', 'num pID');
	     _ImportFunc(&$this.LibraryHandle, 'eGetScene', '_GetScene', '');
	     _ImportFunc(&$this.LibraryHandle, 'eGetAnswerCount', '_GetAnswerCount', '');
	     _ImportFunc(&$this.LibraryHandle, 'eGetAnswerCount', '_GetAnswer', 'num pIndex');
	     _ImportFunc(&$this.LibraryHandle, 'eSelectAnswer', '_SelectAnswer', 'num pIndex');
		 
		 &$this.Initialised = 1;
       }
     else
       {
         &$this.Initialised = 0;
		 
		 _throw('KobzarScripts: Error loading connector library!');
       } 
   }
 //===========================================================;
 
   #public method UseCurrrentInstance()
   {
     if (!_GetEngineHandle())
	   {_throw('KobzarScripts: Error getting engine instance!');}
   }
//===========================================================;
   
   #public method CreateNewInstance()
   {
     if (!_CreateEngineHandle())
	   {_throw('KobzarScripts: Error creating engine instance!');}
   }
//===========================================================;
   
   #public method Kobzar(){&$this.Init();}
//===========================================================;
 
   #public method ~Kobzar()
   {
     if (NewInstance)
	   {_FreeEngineHandle();}
	   
	 _FreeLib(&$this.LibraryHandle);
   }
//===========================================================;

   #public method GetLastError()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetLastError();
   }
//===========================================================;

   #public method CreateStory($File)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _CreateStory($File);
   }
//===========================================================;

   #public method LoadStory($File)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _LoadStory($File);
   }
//===========================================================;

   #public method SaveStory()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _SaveStory();
   }
//===========================================================;

   #public method CloseStory()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _CloseStory();
   }
//===========================================================;

   #public method ClearStory()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _ClearStory();
   }
//===========================================================;

   #public method AddScene()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _AddScene();
   }
//===========================================================;

   #public method AddAnswer()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _AddAnswer();
   }
//===========================================================;

   #public method AddScript()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _AddScript();
   }
//===========================================================;

   #public method Select($ID)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _Select($pID);
   }
//===========================================================;

   #public method Remove($ID)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _Remove($pID);
   }
//===========================================================;

   #public method Link($frID, $toID)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _Link($frID, $toID);
   }
//===========================================================;

   #public method Unlink($frID, $toID)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return Unlink($frID, $toID);
   }
//===========================================================;

   #public method GetID()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetID();
   }
//===========================================================;

   #public method SetID($val)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _SetID($val);
   }
//===========================================================;

   #public method GetDialog()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetDialog();
   }
//===========================================================;

   #public method SetDialog($val)
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _SetDialog($val);
   }
//===========================================================;

   #public method GetLastError()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetLastError();
   }
//===========================================================;

   #public method GetLastError()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetLastError();
   }
//===========================================================;

   #public method GetLastError()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetLastError();
   }
//===========================================================;

   #public method GetLastError()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetLastError();
   }
//===========================================================;

   #public method GetLastError()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetLastError();
   }
//===========================================================;

   #public method GetLastError()
   {
     if (!&$this.Initialised)
	   {_throw('KobzarScripts: Engine not initialised!');}
	 else
	   #return _GetLastError();
   }   
}