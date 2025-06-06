﻿#begin CLIStoryCreator;

//The script emulates work in the console;
//The entered commands will be interpreted in runtime mode;
//Any ELI constructs are acceptable;

#include ".\KobzarScripts.eh";
#include ".\VisualLib.eh";

#procedure RunShell()
{
  $res = "";

  while (!_istreq($res, exit))
    {
      $line = "";
	  
      _writeout(#>);
      _readin($line);
		
      if (_istreq($line, exit) != 1)
        {  
          if (_run($line) == 0)
            {
			   _writeout(_lasterror());
			   _writeout(#endl);
			   $res = exit;}
        }
      else
        {$res = $line;}
    }
}
//===========================================================;

_writeout("[Kobzar Console Story Creator copyright 2024 Maxim Noltmeer m.noltmeer@gmail.com]");
_writeout(#endl);
_writeout(#endl);

&Engine.Create(Kobzar, "");
&Engine.CreateNewInstance(".\KobzarEngine.dll");

if (&Engine.Initialised)
  {
    _writeout("Engine initialised...");
    _writeout(#endl);
	
	&VLib.Create(VisualLibrary, "");
	
	if (&VLib.Initialised)
	  {
	    _writeout("Visualisation Library connected...");
        _writeout(#endl);
	  }
	else
	  {
	    _writeout("Error loading Visualisation Library");
        _writeout(#endl);
	  }
	  
	_writeout("Console started...");
    _writeout(#endl);
	
    _writeout("Type 'exit' to end work");
    _writeout(#endl);
    _writeout(#endl);	

	:RunShell();
	
	_writeout(#endl);
	_writeout("Exit console...");
	_writeout(#endl);
  }
else
  {
    _writeout("Engine not initialised. Exit console...");
    _writeout(#endl);
  }

&VLib.Destroy();
&Engine.Destroy();

#end;