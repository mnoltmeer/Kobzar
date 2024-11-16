$LibraryHandle = _ConnectLib(.\lib\eliKobzar.dll);

if ($LibraryHandle != -1)
  {
    _ImportFunc(&$this.Hinstance, 'eGetEngineHandle', '_GetEngineHandle', '');
	_ImportFunc(&$this.Hinstance, 'eCreateEngineHandle', '_CreateEngineHandle', '');
	_ImportFunc(&$this.Hinstance, 'eFreeEngineHandle', '_FreeEngineHandle', '');
	 
	_ImportFunc(&$this.Hinstance, 'eGetLastError', '_GetLastError', '');
	_ImportFunc(&$this.Hinstance, 'eCreateStory', '_CreateStory', 'sym pFile');
	_ImportFunc(&$this.Hinstance, 'eSaveStory', '_SaveStory', '');
	_ImportFunc(&$this.Hinstance, 'eCloseStory', '_CloseStory', '');
	_ImportFunc(&$this.Hinstance, 'eClearStory', '_ClearStory', '');
	_ImportFunc(&$this.Hinstance, 'eAddScene', '_AddScene', '');
	_ImportFunc(&$this.Hinstance, 'eAddAnswer', '_AddAnswer', '');
	_ImportFunc(&$this.Hinstance, 'eAddScript', '_AddScript', '');
	_ImportFunc(&$this.Hinstance, 'eSelect', '_Select', 'num pID');
	_ImportFunc(&$this.Hinstance, 'eRemove', '_Remove', 'num pID');
	_ImportFunc(&$this.Hinstance, 'eLink', '_Link', 'num pFrID,num pToID');
	_ImportFunc(&$this.Hinstance, 'eUnlink', '_Unlink', 'num pFrID,num pToID');
	_ImportFunc(&$this.Hinstance, 'eGetID', '_GetID', '');
	_ImportFunc(&$this.Hinstance, 'eSetID', '_SetID', 'num pValue');
	_ImportFunc(&$this.Hinstance, 'eGetDialog', '_GetDialog', '');
	_ImportFunc(&$this.Hinstance, 'eSetDialog', '_SetDialog', 'num pValue');
	_ImportFunc(&$this.Hinstance, 'eGetNextDialog', '_GetNextDialog', '');
	_ImportFunc(&$this.Hinstance, 'eSetNextDialog', '_SetNextDialog', 'num pValue');
	_ImportFunc(&$this.Hinstance, 'eGetPrevID', '_GetPrevID', '');
	_ImportFunc(&$this.Hinstance, 'eSetPrevID', '_SetPrevID', 'num pValue');
	_ImportFunc(&$this.Hinstance, 'eGetNextID', '_GetNextID', '');
	_ImportFunc(&$this.Hinstance, 'eSetNextID', '_SetNextID', 'num pValue');
	_ImportFunc(&$this.Hinstance, 'eGetType', '_GetType', '');
	_ImportFunc(&$this.Hinstance, 'eGetText', '_GetText', '');
	_ImportFunc(&$this.Hinstance, 'eSetText', '_SetText', 'sym pValue');
	_ImportFunc(&$this.Hinstance, 'eIsEndDialog', '_IsEndDialog', '');
	_ImportFunc(&$this.Hinstance, 'eSetEndDialog', '_SetEndDialog', 'num pValue');
	_ImportFunc(&$this.Hinstance, 'eGetParams', '_GetParams', '');
	_ImportFunc(&$this.Hinstance, 'eSetParams', '_SetParams', 'sym pValue');
	_ImportFunc(&$this.Hinstance, 'eRun', '_Run', '');
	_ImportFunc(&$this.Hinstance, 'eGetResult', '_GetResult', '');
	
	_ImportFunc(&$this.Hinstance, 'eTellStory', '_TellStory', 'sym pFile');
	_ImportFunc(&$this.Hinstance, 'eLoadDialog', '_LoadDialog', 'num pID');
	_ImportFunc(&$this.Hinstance, 'eGetScene', '_GetScene', '');
	_ImportFunc(&$this.Hinstance, 'eGetAnswerCount', '_GetAnswerCount', '');
	_ImportFunc(&$this.Hinstance, 'eGetAnswerCount', '_GetAnswer', 'num pIndex');
	_ImportFunc(&$this.Hinstance, 'eSelectAnswer', '_SelectAnswer', 'num pIndex');
  }
else
  {
    _throw('KobzarScripts: Error loading connector library!');
  }
//===========================================================;
  
#class Kobzar
{
   #property NewInstance = 0;
   
   
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
   
   #public method Kobzar()
   {
     
   }
//===========================================================;
 
   #public method ~Kobzar()
   {
     if (NewInstance)
	   {_FreeEngineHandle();}
	   
	 _FreeLib(&$this.Handle);
   }
//===========================================================;
}