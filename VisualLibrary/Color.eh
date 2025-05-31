#class Color
{
  #method InRange($val)
  {
    $res = 1;
	
	if ($val < 0)
	  {$res = 0;}
	else if ($val > 255)
	  {$res = 0;}
	else
	  {$res = 1;}
	  
	#return $res;
  }
  
  #public property Red = 0;
  #public property Green = 0;
  #public property Blue = 0;
  #public property Alpha = 255;
  
  #public method Set($a, $r, $g, $b)
  {
    if (&$this.InRange($r))
	  {&$this.Red = $r;}
	else
	  {&$this.Red = 0;}
	  
	if (&$this.InRange($g))
	  {&$this.Green = $g;}
	else
	  {&$this.Green = 0;}
	  
	if (&$this.InRange($b))
	  {&$this.Blue = $b;}
	else
	  {&$this.Blue = 0;}

	if (&$this.InRange($a))
	  {&$this.Alpha = $a;}
	else
	  {&$this.Blue = 0;}
  }
  
  #public method Color($a, $r, $g, $b){&$this.Set($a, $r, $g, $b);}
}
