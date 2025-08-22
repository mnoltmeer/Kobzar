#include ".\Color.eh";
#include ".\Point.eh";

#class VisualLibrary
{
  #property LibraryHandle = 0;
   
  #public property Initialised = 0;
 
  #method Init()
  {
    &$this.LibraryHandle = _ConnectLib(.\VisualLib.dll);

	if (&$this.LibraryHandle != -1)
	  {
	    _ImportFunc(&$this.LibraryHandle, "eLoadFont", "_LoadFont", "sym pObjectName");		
		_ImportFunc(&$this.LibraryHandle, "eCreateForm", "_CreateForm", "num pWidth,num pHeight,num pFullscreen");
		_ImportFunc(&$this.LibraryHandle, "eDestroyForm", "_DestroyForm", "");
		_ImportFunc(&$this.LibraryHandle, "eClearForm", "_ClearForm", "");
		_ImportFunc(&$this.LibraryHandle, "eCalcTextArea", "_CalcTextArea", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawLine", "_DrawLine", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawArc", "_DrawArc", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawPoly", "_DrawPoly", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawEllipse", "_DrawEllipse", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawRect", "_DrawRect", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawImage", "_DrawImage", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawFrame", "_DrawFrame", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawBubbleRect", "_DrawBubble", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawBlast", "_DrawBlast", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawText", "_DrawText", "sym pObjectName");

        &$this.Initialised = 1;		
      }
    else
      {
        &$this.Initialised = 0;
		
		_throw("VisualLibrary: Error loading library!");
      } 
  }
   
  #public method VisualLibrary()
  {
    &$this.Init();
	
	if (!&$this.Initialised)
	  {_throw("VisualLibrary: not initialised!");}
  }
 
  #public method ~VisualLibrary(){#protect {_FreeLib(&$this.LibraryHandle);}}
}
//===========================================================;

#class UserFont
{
  #public property Source = "";
  #public property Name = "Arial";  
  
  #public method UserFont($file)
  {
    &$this.Source = $file;
	_LoadFont(&$this.GetName());
  }
}
//===========================================================;

#class Font
{
  #public property UserFont = 0; //set 1 if assigned to UserFont;
  #public property Name = "Arial";
  #public property Style = r; //r  (Regular) = 0
							  //b  (Bold) = 1,
							  //i  (Italic) = 2,
							  //bi (BoldItalic) = 3,
							  //u  (Underline) = 4,
							  //s  (Strikeout) = 8;
  #public property Size = 10;
  #public property Color = #class Color(255, 0, 0, 0);
  
  #public method Assign($userfont)
  {
    if (&$userfont.Exist())
	  {
		&$this.UserFont = 1;
	    &$this.Name = &$userfont.Name;
	  }
	else
	  {_throw("UserFont object [" $userfont "] doesn't exists!");}
  }
  
  #public method Select($name)
  {
    &$this.UserFont = 0;
	&$this.Name = $name;
  }
  
  #public method Font($name){&$this.Select($name);}
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
  #public property Left = 0;
  #public property Top = 0;
  #public property Width = 0;
  #public property Height = 0;
  #public property StartAngle = 0;
  #public property SweepAngle = 0;
  #public property Size = 1;
  #public property Color = #class Color(255, 0, 0, 0);

  #public method Draw(){_DrawArc(&$this.GetName());}
  
  #public method Arc($left, $top, $width, $height, $stangle, $swangle, $size)
  {
    &$this.Left = $left;
	&$this.Top = $top;
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
  #public property Left = 0;
  #public property Top = 0;
  #public property Width = 0;
  #public property Height = 0;
  #public property Color = #class Color(255, 255, 255, 255);
  #public property Border = #class Border;
  #public property Shadow = 0;
   
  #public method Draw(){_DrawEllipse(&$this.GetName());}
  
  #public method Ellipse($left, $top, $width, $height)
  {
    &$this.Left = $left;
	&$this.Top = $top;
	&$this.Width = $width;
	&$this.Height = $height;
  }
}
//===========================================================;

#class Rect
{     
  #public property Left = 0;
  #public property Top = 0;
  #public property Width = 0;
  #public property Height = 0;
  #public property Color = #class Color(255, 255, 255, 255);
  #public property Border = #class Border;
  #public property Shadow = 0;
   
  #public method Draw(){_DrawRect(&$this.GetName());}
  
  #public method Rect($left, $top, $width, $height)
  {
    &$this.Left = $left;
	&$this.Top = $top;
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
	else if (_istreq(&$this.Type, "CharLine"))
	  {
	    $txtlen = _strlen(&$this.Text);
		
		if ($txtlen != 0) {&$this.AdjustToText();}
		
		_DrawFrame(&$this.GetName());		
		
		if ($txtlen != 0) {_DrawText(&$this.Text.GetName());}
	  }
	else
	  {_throw("VisualLibrary: invalid object type");}
  }
  
  #public method SetPos($left, $top){&$this.Left = $left; &$this.Top = $top;}
  #public method SetSize($width, $height){&$this.Width = $width; &$this.Height = $height;}
  
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

#class Text : Object
{  
  #public property Type = "Text";
  
  #public property Font = #class Font;
  #public property Align = "justify"; //left, right, center, justify;
  #public property WordWrap = 0;
  #public property CenterVerticaly = 0;
  #public property Data = "";
  
  #public method CalcAreaSize(){_CalcTextArea(&$this.GetName());}
  
  #public method Text($left, $top, $width, $height) 
  {
    &$this.Object($left, $top, $width, $height); //set $width or $height to 0 for auto calculate;
  }
}
//===========================================================;

#class CharLine : Object
{
  #public property Color = #class Color(255, 255, 255, 255);
  #public property Border = #class Border;
  #public property Shadow = 0;
  #public property BeforeTextInterval = 5;
  #public property AfterTextInterval = 5;
  #public property Text = #class Text;  
  
  #public method AdjustToText()
  {
	&$this.Text.Left = &$this.Left + &$this.BeforeTextInterval;
	&$this.Text.Top = &$this.Top + &$this.AfterTextInterval;
	&$this.Text.Width = &$this.Width - &$this.BeforeTextInterval - &$this.AfterTextInterval;
	&$this.Text.Height = 0;
		
	&$this.Text.CalcAreaSize();
	
	&$this.Width = &$this.Text.Width + &$this.BeforeTextInterval + &$this.AfterTextInterval;
	&$this.Height = &$this.Text.Height + &$this.BeforeTextInterval + &$this.AfterTextInterval;
  }
  
  #public method CharLine($left, $top, $width, $height) {&$this.Object($left, $top, $width, $height);}
}
//===========================================================;

#class Plate : CharLine
{  
  #public property Type = "CharLine";  
  
  #public property Corner = 0; //set more than 0 to set corner radius;    
  
  #public method Plate($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
}
//===========================================================;

#class Bubble : Plate
{  
  #public property Type = "CharLine";
  
  #public property Tail = #class Point(0, 0);
  
  #public method Bubble($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
}
//===========================================================;

#class Blast : CharLine
{  
  #public property Type = "CharLine";
  
  #public property MinRayHeight = 10;
  #public property MaxRayHeight = 50; //max ray height or ray height if DynamicRays is 0;
  #public property DynamicRays = 0; //set more than 0 to randomising ray height;
   
  #public method Blast($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
}
//===========================================================;

#class VisualScene
{
  #public method Clear(){#return _ClearForm();}
  
  #public method VisualScene($width, $height, $fullscreen){_CreateForm($width, $height, $fullscreen);}
 
  #public method ~VisualScene(){#protect {_DestroyForm();}}  
}
//===========================================================;