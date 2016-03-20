<?php

class classesDB
{
   private $db;
   public function __construct()
   {
	$this->db = new mysqli("localhost","root","toor","permissions");
	if ($this->db->connect_errno > 0)
	{
		echo __FILE__.":".__LINE__.": failed to connect to db, re: ".$this->db->connect_error.PHP_EOL;
		exit(0);
	}
   }

   public function __destruct()
   {
	$this->db->close();
   }

}


?>
