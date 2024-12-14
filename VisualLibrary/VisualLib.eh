#class VisualLibrary
{
  #property LibraryHandle = 0;
   
  #public property Initialised = 0;
//===========================================================;
 
  #method Init()
  {
    &$this.LibraryHandle = _ConnectLib(.\Visualisation.dll);

	if (&$this.LibraryHandle != -1)
	  {
	    _ImportFunc(&$this.LibraryHandle, 'eCreateForm', '_CreateForm', '');
	    _ImportFunc(&$this.LibraryHandle, 'eDeleteForm', '_DeleteForm', 'num pFormID');

        &$this.Initialised = 1;		
      }
    else
      {
        &$this.Initialised = 0;
		
		_throw('VisualLibrary: Error loading library!');
      } 
  }
//===========================================================;
   
  #public method VisualLibrary(){&$this.Init();}
//===========================================================;
 
  #public method ~VisualLibrary(){_FreeLib(&$this.LibraryHandle);}
//===========================================================;

  #public method CreateForm()
  {
     if (!&$this.Initialised)
	   {_throw('VisualLibrary: not initialised!');}
	 else
	   {#return _CreateForm();}
  }
//===========================================================;

  #public method DeleteForm($id)
  {
     if (!&$this.Initialised)
	   {_throw('VisualLibrary: not initialised!');}
	 else
	   {#return _DeleteForm($id);}
  }
//===========================================================;  
}