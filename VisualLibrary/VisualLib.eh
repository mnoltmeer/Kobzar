#include '.\Color.eh';

#class Form
{
  #public property Width = 0;
  #public property Height = 0;
  #public property FullScreen = 0;
}
//===========================================================;

#class Image
{
  #public property X = 0;
  #public property Y = 0;
  #public property Source = 'default.bmp';
}
//===========================================================;

#class Frame
{
  #public property X = 0;
  #public property Y = 0;
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
		_ImportFunc(&$this.LibraryHandle, 'eDrawImage', '_DrawImage', 'num pX,num pY,sym pFile');

        &$this.Initialised = 1;		
      }
    else
      {
        &$this.Initialised = 0;
		
		_throw('VisualScene: Error loading library!');
      } 
  }
//===========================================================;
   
  #public method VisualScene($width, $height, $fullscreen)
  {
    &$this.Init();
	
	if (!&$this.Initialised)
	   {_throw('VisualScene: not initialised!');}
	 else
	   {#return _CreateForm($width, $height, $fullscreen);}
  }
//===========================================================;
 
  #public method ~VisualScene()
  {
    _DestroyForm();
	_FreeLib(&$this.LibraryHandle);
  }
//===========================================================;

  #public method OpenForm()
  {
     if (!&$this.Initialised)
	   {_throw('VisualScene: not initialised!');}
	 else
	   {#return _ShowForm();}
  }
//===========================================================;

  #public method CloseForm()
  {
     if (!&$this.Initialised)
	   {_throw('VisualScene: not initialised!');}
	 else
	   {#return _HideForm();}
  }
//===========================================================;  
}