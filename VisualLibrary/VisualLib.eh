#include '.\Color.eh';

#class VisualLibrary
{
  #property LibraryHandle = 0;
   
  #public property Initialised = 0;
//===========================================================;
 
  #method Init()
  {
    &$this.LibraryHandle = _ConnectLib(.\VisualLib.dll);

	if (&$this.LibraryHandle != -1)
	  {
	    _ImportFunc(&$this.LibraryHandle, 'eLoadFont', '_LoadFont', 'sym pFile');
		_ImportFunc(&$this.LibraryHandle, 'eRemoveFont', '_RemoveFont', 'sym pFile');
		
		_ImportFunc(&$this.LibraryHandle, 'eCreateForm', '_CreateForm', 'num pWidth,num pHeight,num pFullscreen');
		_ImportFunc(&$this.LibraryHandle, 'eDestroyForm', '_DestroyForm', '');
		_ImportFunc(&$this.LibraryHandle, 'eShowForm', '_ShowForm', '');
	    _ImportFunc(&$this.LibraryHandle, 'eHideForm', '_HideForm', '');
		_ImportFunc(&$this.LibraryHandle, 'eClearForm', '_ClearForm', '');
		_ImportFunc(&$this.LibraryHandle, 'eDrawImage', '_DrawImage', 'num pX,num pY,sym pFile');
		

        &$this.Initialised = 1;		
      }
    else
      {
        &$this.Initialised = 0;
		
		_throw('VisualLibrary: Error loading library!');
      } 
  }
//===========================================================;
   
  #public method VisualLibrary()
  {
    &$this.Init();
	
	if (!&$this.Initialised)
	   {_throw('VisualLibrary: not initialised!');}
  }
//===========================================================;
 
  #public method ~VisualLibrary(){_FreeLib(&$this.LibraryHandle);}
//===========================================================;
}

#class Object
{
  #public property X = 0;
  #public property Y = 0;
}
//===========================================================;

#class Image : Object
{
  #public property Source = 'default.bmp';
}
//===========================================================;

#class Frame : Object
{
  #public property Width = 0;
  #public property Height = 0;
  #public property Color = #class Color(0, 0, 0);
  #public property BorderSize = 0;
}
//===========================================================;

#class Baloon : Frame
{
  #public property Style = 0;
  #public property SpeechPos = 0;
}
//===========================================================;

#class Font
{
  #public property FontName = 'Arial';
  #public property Style = d; //d - default, i - italic, b - bold
  #public property Size = 10;  
}
//===========================================================;

#class Text
{
  #public property X = 0;
  #public property Y = 0;
  #public property Font = #class Font;
  #public property Text = '';
}
//===========================================================;

#class RuntimeFont
{
  #property File = '';
  
  #public method RuntimeFont($file)
  {
    #protect
	{
	  _LoadFont($file);
	  
	  if ($res)
	    {&$this.File = $file;}
	}
  }
//===========================================================;
 
  #public method ~RuntimeFont()
  {
    #protect
	{
	  if (&$this.File) {_RemoveFont(&$this.File);}
	}
  }
}
//===========================================================;

#class VisualScene
{
  #public method VisualScene($width, $height, $fullscreen){_CreateForm($width, $height, $fullscreen);}
//===========================================================;
 
  #public method ~VisualScene(){_DestroyForm();}
//===========================================================;

  #public method ShowForm(){#return _ShowForm();}
//===========================================================;

  #public method HideForm(){#return _HideForm();}
//===========================================================;

  #public method Clear(){#return _ClearForm();}
//===========================================================;

  #public method DrawImage($x, $y, $file){#return _DrawImage($x, $y, $file);}

//===========================================================;  
}