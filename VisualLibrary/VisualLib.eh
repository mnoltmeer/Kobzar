#include ".\Color.eh";
#include ".\Point.eh";

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
		_ImportFunc(&$this.LibraryHandle, "eDrawLine", "_DrawLine", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawArc", "_DrawArc", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawPoly", "_DrawPoly", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawEllipse", "_DrawEllipse", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawImage", "_DrawImage", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawFrame", "_DrawFrame", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawBubbleRect", "_DrawBubbleRect", "sym pObjectName");
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
  #public property Size = 0;
  #public property Color = #class Color(255, 0, 0, 0);  
}
//===========================================================;

#class Line
{
  #public property X1 = 0;
  #public property Y1 = 0;
  #public property X2 = 0;
  #public property Y2 = 0;
  #public property Size = 1;
  #public property Color = #class Color(255, 0, 0, 0);

  #public method Draw(){_DrawLine(&$this.GetName());}
  
  #public method Line($x1, $y1, $x2, $y2, $size)
  {
    &$this.X1 = $x1;
	&$this.Y1 = $y1;
	&$this.X2 = $x2;
	&$this.Y2 = $y2;
	&$this.Size = $size;
  }  
}
//===========================================================;

#class Arc
{
  #public property X = 0;
  #public property Y = 0;
  #public property Width = 0;
  #public property Height = 0;
  #public property StartAngle = 0;
  #public property SweepAngle = 0;
  #public property Size = 1;
  #public property Color = #class Color(255, 0, 0, 0);

  #public method Draw(){_DrawArc(&$this.GetName());}
  
  #public method Arc($x, $y, $width, $height, $stangle, $swangle, $size)
  {
    &$this.X = $x;
	&$this.Y = $y;
	&$this.Width = $width;
	&$this.Height = $height;
	&$this.StartAngle = $stangle;
	&$this.SweepAngle = $swangle;
	&$this.Size = $size;
  }  
}
//===========================================================;

#class Poly
{   
  #public property Count = 0;  
  #public property Color = #class Color(255, 255, 255, 255);
  #public property Border = #class Border;
  #public property Shadow = 0;
  
  #public method AddPoint($x, $y)
  {
    $name = _int(&$this.Count);
    &$this.Add($name, #class Point("$x,$y"));
	&$this.Count = ++1;
  }
  
  #public method Draw(){_DrawPoly(&$this.GetName());}
}
//===========================================================;

#class Ellipse
{     
  #public property X = 0;
  #public property Y = 0;
  #public property Width = 0;
  #public property Height = 0;
  #public property Color = #class Color(255, 255, 255, 255);
  #public property Border = #class Border;
  #public property Shadow = 0;
   
  #public method Draw(){_DrawEllipse(&$this.GetName());}
  
  #public method Ellipse($x, $y, $width, $height)
  {
    &$this.X = $x;
	&$this.Y = $y;
	&$this.Width = $width;
	&$this.Height = $height;
  }
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
	else if (_istreq(&$this.Type, "Bubble"))
	  {_DrawBubbleRect(&$this.GetName());}
	else
	  {_throw("VisualLibrary: invalid object type");}
  }
  
  #public method Object($left, $top, $width, $height)
  {
    &$this.Left = $left;
	&$this.Top = $top;
	&$this.Width = $width;
	&$this.Height = $height;
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
  
  #public property Color = #class Color(255, 255, 255, 255);
  #public property Border = #class Border;
  #public property Corner = 0; //set more than 0 to set corner radius;
  #public property Shadow = 0;

  #public method Frame($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
}
//===========================================================;

#class Bubble : Frame
{  
  #public property Type = "Bubble";
  
  #public property Tail = #class Point(0, 0);
  
  #public method Bubble($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
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
  
  #public method Text($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}
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