#include ".\Color.eh";

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
	    _ImportFunc(&$this.LibraryHandle, "eLoadFont", "_LoadFont", "sym pFile");
		_ImportFunc(&$this.LibraryHandle, "eRemoveFont", "_RemoveFont", "sym pFile");
		
		_ImportFunc(&$this.LibraryHandle, "eCreateForm", "_CreateForm", "num pWidth,num pHeight,num pFullscreen");
		_ImportFunc(&$this.LibraryHandle, "eDestroyForm", "_DestroyForm", "");
		_ImportFunc(&$this.LibraryHandle, "eClearForm", "_ClearForm", "");
		_ImportFunc(&$this.LibraryHandle, "eDrawImage", "_DrawImage", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawFrame", "_DrawFrame", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawText", "_DrawText", "sym pObjectName");

        &$this.Initialised = 1;		
      }
    else
      {
        &$this.Initialised = 0;
		
		_throw("VisualLibrary: Error loading library!");
      } 
  }
//===========================================================;
   
  #public method VisualLibrary()
  {
    &$this.Init();
	
	if (!&$this.Initialised)
	   {_throw("VisualLibrary: not initialised!");}
  }
//===========================================================;
 
  #public method ~VisualLibrary(){#protect {_FreeLib(&$this.LibraryHandle);}}
}
//===========================================================;

#class RuntimeFont
{
  #property File = "";
  
  #public method RuntimeFont($file)
  {
    #protect
	{
	  $res = _LoadFont($file);
	  
	  if ($res)
	    {&$this.File = $file;}
	}
  }
//===========================================================;
 
  #public method ~RuntimeFont()
  {
    #protect
	{
	  if (_streq(&$this.File, "") == 0)
	    {_RemoveFont(&$this.File);}
	}
  }
}
//===========================================================;

#class Font
{
  #public property Name = "Arial";
  #public property Style = r; //r  (Regular) = 0
							  //b  (Bold) = 1,
							  //i  (Italic) = 2,
							  //bi (BoldItalic) = 3,
							  //u  (Underline) = 4,
							  //s  (Strikeout) = 8;
  #public property Size = 10;
  #public property Color = #class Color(255, 0, 0, 0);  
}
//===========================================================;

#class Border
{
  #public property Size = 1; //can't be less than 1;
  #public property Color = #class Color(255, 0, 0, 0);  
}
//===========================================================;

#class Object
{
  #public property Type = "Object";
  
  #public property Left = 0;
  #public property Top = 0;
  #public property Width = 0;
  #public property Height = 0;
  
  #public method Draw()
  {
    if (_istreq(&$this.Type, "Image"))
	  {_DrawImage(&$this.GetName());}
	else if (_istreq(&$this.Type, "Text"))
	  {_DrawText(&$this.GetName());}
	else if (_istreq(&$this.Type, "Frame"))
	  {_DrawFrame(&$this.GetName());}
	else
	  {_throw("VisualLibrary: invalid object type");}
  }
}
//===========================================================;

#class Image : Object
{  
  #public property Type = "Image";
  
  #public property Source = ".\default.png";
  
  #public method Image($file){&$this.Source = $file;}
}
//===========================================================;

#class Frame : Object
{  
  #public property Type = "Frame";
  
  #public property Color = #class Color(255, 0, 0, 0);
  #public property Border = #class Border;
  #public property Corner = 0; //set more than 0 to set corner radius;

  #public method Frame($a, $r, $g, $b)
  {
    &$this.Color.Set($a, $r, $g, $b);
	&$this.Border.Color.Set($a, $r, $g, $b); //no border by default;
  }  
}
//===========================================================;

#class Baloon : Frame
{  
  #public property Type = "Baloon";
  
  #public property Style = 0;
  #public property SpeechPos = 0;
  
}
//===========================================================;

#class Text : Object
{  
  #public property Type = "Text";
  
  #public property Font = #class Font;
  #public property Align = "justify"; //left, right, center, justify;
  #public property WordWrap = 0;
  #public property CenterVerticaly = 0;
  #public property Data = "";
}
//===========================================================;

#class VisualScene
{
  #public method VisualScene($width, $height, $fullscreen){_CreateForm($width, $height, $fullscreen);}
//===========================================================;
 
  #public method ~VisualScene(){#protect {_DestroyForm();}}
//===========================================================;

  #public method Clear(){#return _ClearForm();}
//===========================================================;  
}