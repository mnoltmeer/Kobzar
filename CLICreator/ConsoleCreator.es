#begin CLIStoryCreator;

//The script simulates work in the console;
//The entered commands will be interpreted in runtime mode;
//Any ELI constructs are acceptable;


#procedure RunShell()
{
  $res = string;

  while (_istreq($res, exit) != 1)
    {
      $line = '';
	  
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

_writeout('[Kobzar Console Story Creator copyright 2024 Maxim Noltmeer m.noltmeer@gmail.com]');
_writeout(#endl);
_writeout('Type "exit" to end work');
_writeout(#endl);
_writeout(#endl);

&Engine.Create(Kobzar, '');

:RunShell();

&Engine.Destroy();

#end;