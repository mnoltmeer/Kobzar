#include ".\Color.eh";
#include ".\Point.eh";

#class VisualLibrary
{
  #property LibraryHandle = 0;
   
  #public property Initialised = 0;
 
  #method Init()
  {
    &$this.LibraryHandle = _ConnectLib(C:\Users\mnolt\OneDrive\GIT\Kobzar\VisualLibrary\Win32\Release\VisualLib.dll);

	if (&$this.LibraryHandle != -1)
	  {
	    _ImportFunc(&$this.LibraryHandle, "eLoadFont", "_LoadFont", "sym pObjectName");		
		_ImportFunc(&$this.LibraryHandle, "eCreateForm", "_CreateForm", "num pWidth,num pHeight,num pFullscreen");
		_ImportFunc(&$this.LibraryHandle, "eDestroyForm", "_DestroyForm", "");
		_ImportFunc(&$this.LibraryHandle, "eClearForm", "_ClearForm", "");
		_ImportFunc(&$this.LibraryHandle, "eCalcTextArea", "_CalcTextArea", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eDrawLine", "_DrawLine", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawArc", "_DrawArc", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawPoly", "_DrawPoly", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawEllipse", "_DrawEllipse", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawRect", "_DrawRect", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawImage", "_DrawImage", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawPlate", "_DrawPlate", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawBubble", "_DrawBubble", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawBlast", "_DrawBlast", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawBalloon", "_DrawBalloon", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawCloud", "_DrawCloud", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eDrawText", "_DrawText", "sym pObjectName,num pHandle");
		_ImportFunc(&$this.LibraryHandle, "eCreateControl", "_CreateControl", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eMoveControl", "_MoveControl", "sym pObjectName");
		_ImportFunc(&$this.LibraryHandle, "eUpdateControl", "_UpdateControl", "sym pObjectName");

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

#class Figure
{
  #public property Left = 0;
  #public property Top = 0;
  #public property Width = 0;
  #public property Height = 0;
  #public property Color = #class Color(255, 0, 0, 0);
  
  #public method Figure($left, $top, $width, $height)
  {
    &$this.Left = $left;
	&$this.Top = $top;
	&$this.Width = $width;
	&$this.Height = $height;
  }  
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

  #public method Draw($handle){_DrawLine(&$this.GetName(), $handle);}
  
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

#class Arc : Figure
{
  #public property StartAngle = 0;
  #public property SweepAngle = 0;
  #public property Size = 1;

  #public method Draw(){_DrawArc(&$this.GetName(), &$this.Handle);}
  
  #public method Arc($left, $top, $width, $height, $stangle, $swangle, $size)
  {
    &$this.Figure($left, $top, $width, $height);

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
  
  #public method Draw($handle){_DrawPoly(&$this.GetName(), $handle);}
}
//===========================================================;

#class Ellipse : Figure
{     
  #public property Border = #class Border;
  #public property Shadow = 0;
   
  #public method Draw($handle){_DrawEllipse(&$this.GetName(), $handle);}
  
  #public method Ellipse($left, $top, $width, $height){&$this.Figure($left, $top, $width, $height);}
}
//===========================================================;

#class Rect : Figure
{     
  #public property Border = #class Border;
  #public property Shadow = 0;
   
  #public method Draw($handle){_DrawRect(&$this.GetName(), $handle);}
  
  #public method Rect($left, $top, $width, $height){&$this.Figure($left, $top, $width, $height);}
}
//===========================================================;

#class Object
{
  #public property Left = 0;
  #public property Top = 0;
  #public property Width = 0;
  #public property Height = 0;  
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
  #public property Source = ".\default.png";
  
  #public method Draw($handle){_DrawImage(&$this.GetName(), $handle);}
  
  #public method Image($file){&$this.Source = $file;}
}
//===========================================================;

#class Text : Object
{  
  #public property Font = #class Font;
  #public property Align = "justify"; //left, right, center, justify;
  #public property WordWrap = 0;
  #public property CenterVerticaly = 0;
  #public property Data = "";
  
  #public method CalcAreaSize(){_CalcTextArea(&$this.GetName());}
  
  #public method Draw($handle){_DrawText(&$this.GetName(), $handle);}
  
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
  #public property Corner = 0; //set more than 0 to set corner radius;

  #public method Draw($handle)
  {
	$txtlen = _strlen(&$this.Text.Data);
		
	if ($txtlen != 0) {&$this.AdjustToText();}
		
	_DrawPlate(&$this.GetName(), $handle);		
		
	if ($txtlen != 0) {_DrawText(&$this.Text.GetName(), $handle);}
  }  
  
  #public method Plate($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
}
//===========================================================;

#class Bubble : Plate
{  
  #public property Tail = #class Point(0, 0);
  #public property TailWidth = 15;
  
  #public method Draw($handle)
  {
	$txtlen = _strlen(&$this.Text.Data);
		
	if ($txtlen != 0) {&$this.AdjustToText();}
		
	_DrawBubble(&$this.GetName(), $handle);		
		
	if ($txtlen != 0) {_DrawText(&$this.Text.GetName(), $handle);}
  }
  
  #public method Bubble($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
}
//===========================================================;

#class Blast : CharLine
{  
  #public property MinRayHeight = 10;
  #public property MaxRayHeight = 50; //max ray height or ray height if DynamicRays is 0;
  #public property DynamicRays = 0; //set more than 0 to randomising ray height;
  
  #public method Draw($handle)
  {
	$txtlen = _strlen(&$this.Text.Data);
		
	if ($txtlen != 0) {&$this.AdjustToText();}
		
	_DrawBlast(&$this.GetName(), $handlee);		
		
	if ($txtlen != 0) {_DrawText(&$this.Text.GetName(), $handle);}
  }
   
  #public method Blast($left, $top, $width, $height){&$this.Object($left, $top, $width, $height);}  
}
//===========================================================;

#class Balloon : Bubble
{
  #public method Draw($handle)
  {
	$txtlen = _strlen(&$this.Text.Data);
		
	if ($txtlen != 0) {&$this.AdjustToText();}
		
	_DrawBalloon(&$this.GetName(), $handle);		
		
	if ($txtlen != 0) {_DrawText(&$this.Text.GetName(), $handle);}
  }
  
  #public method Balloon($left, $top, $width, $height)
  {
    &$this.Object($left, $top, $width, $height);
	&$this.BeforeTextInterval = 20;
	&$this.AfterTextInterval = 20;
  }
}
//===========================================================;

#class Cloud : Bubble
{
  #public method Draw($handle)
  {
	$txtlen = _strlen(&$this.Text.Data);
		
	if ($txtlen != 0) {&$this.AdjustToText();}
		
	_DrawCloud(&$this.GetName(), $handle);		
		
	if ($txtlen != 0) {_DrawText(&$this.Text.GetName(), $handle);}
  }
  
  #public method Cloud($left, $top, $width, $height)
  {
    &$this.Object($left, $top, $width, $height);
	&$this.BeforeTextInterval = 15;
	&$this.AfterTextInterval = 15;
  }
}
//===========================================================;

#class Control : Object
{ 
  #public property Handle = 0; //handle of WinAPI window;
  
  #public method SetPos($left, $top)
  {
    &$this.Left = $left;
	&$this.Top = $top;
	_MoveControl(&$this.GetName());
  } 
  
  #public method Update(){_UpdateControl(&$this.GetName());} //redraws control visual elements;
  
  #public method Control($left, $top, $width, $height)
  {
    &$this.Object($left, $top, $width, $height);
	
	&$this.Handle = _CreateControl(&$this.GetName());
	
	if (!&$this.Handle)
	  {_throw("Error creating Control [" &$this.GetName() "]";}
  }
}
//===========================================================;

#modify class Control
{ 
  #drop method SetSize;
}
//===========================================================;

#class VisualScene
{
  #public property Handle = 0; //handle of WinAPI window;
  
  #public method Clear(){#return _ClearForm();}
  
  #public method VisualScene($width, $height, $fullscreen){&$this.Handle = _CreateForm($width, $height, $fullscreen);}
 
  #public method ~VisualScene(){#protect {_DestroyForm();}}  
}
//===========================================================;