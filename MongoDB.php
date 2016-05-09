<?php
class loginDB
{
   private $db;
   private $ini;
   public function __construct($iniFile)
   {
	$this->ini = parse_ini_file($iniFile,true);
	$host = $this->ini['loginDB']['host'];
	$user = $this->ini['loginDB']['user'];
	$password = $this->ini['loginDB']['password'];
	$database = $this->ini['loginDB']['database'];
	$this->db = new mysqli($host,$user,$password,$database);
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

   public function MongoSend() 
   {
   try 
    {
      $uri = "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions";
      $m = new MongoClient($uri);
      echo "Sucessfull";
      
      echo "Collection Selected sucessfully";

      $connection = new MongoClient( "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions");
      $mongodb = $connection->selectDB('test');
      echo "collection Login selected sucessfully";

      $return_arr = array();
      
      $fetch = mysql_query("SELECT * FROM server"); 

      while ($row = mysql_fetch_array($fetch, MYSQL_ASSOC)) 
      {
	  $row_array['serverID'] = $row['serverID'];
	  $row_array['serverName'] = $row['serverName'];
	  $row_array['IPaddress'] = $row['IPaddress'];
	  $row_array['serverDesc'] = $row['serverDesc'];
	  $row_array['OwnerID'] = $row['OwnerID'];

	  array_push($return_arr,$row_array);
      }
      echo json_encode($return_arr);

    }
    catch (MongoConnectionException $e) 
    {
      die('Error connecting to MongoDB server');
    } 

    catch (MongoConnectionException $e) 
    { 
      die('Error: ' . $e->getMessage());
    }
  }
}  
?>