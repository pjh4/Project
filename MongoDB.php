<?php
  include "logindb.php.inc";
  try 
  {
  $query = "SELECT * FROM server;";
  $result = $this->db->query($query);
  if ($result->num_rows > 0) 
  {
      while($row = $result->fetch_assoc()) 
      {
	$jsonO=file_get_contents($fetch_pInfo);
	var_dump($jsonO);

	$connection = new MongoClient( "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions");
	$mongodb = $connection->selectDB('permissions');
	$collection = new MongoCollection($mongodb,'server');

	$row=array();
		$row["serverID"] = $serverID;
		$row["info"] = $jsonO;
		$collection->insert($row);			
	  //echo " - Name: " . $row["serverName"]. " - IP Address: " . $row["IPaddress"]. " - Info: " . $row["serverDesc"].PHP_EOL;
      }
  } 		

  }
  
  catch (MongoConnectionException $e) 
  {
    die('Error connecting to MongoDB server');
  } catch (MongoConnectionException $e) 
  { 
    die('Error: ' . $e->getMessage());
  }
?>
