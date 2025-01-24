#class VisualScene
{
  #property LibraryHandle = 0;
   
  #public property Initialised = 0;
//===========================================================;
 
  #method Init()
  {
    &$this.LibraryHandle = _ConnectLib(.\VisualLib.dll);

	if (&$this.LibraryHandle != -1)
	  {
	    _ImportFunc(&$this.LibraryHandle, 'eOpenForm', '_OpenForm', '');
	    _ImportFunc(&$this.LibraryHandle, 'eCloseForm', '_CloseForm', '');

        &$this.Initialised = 1;		
      }
    else
      {
        &$this.Initialised = 0;
		
		_throw('VisualScene: Error loading library!');
      } 
  }
//===========================================================;
   
  #public method VisualScene(){&$this.Init();}
//===========================================================;
 
  #public method ~VisualScene(){_FreeLib(&$this.LibraryHandle);}
//===========================================================;

  #public method OpenForm()
  {
     if (!&$this.Initialised)
	   {_throw('VisualScene: not initialised!');}
	 else
	   {#return _OpenForm();}
  }
//===========================================================;

  #public method CloseForm()
  {
     if (!&$this.Initialised)
	   {_throw('VisualScene: not initialised!');}
	 else
	   {#return _CloseForm();}
  }
//===========================================================;  
}