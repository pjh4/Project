<?php
  try 
  {
    $uri = "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions";
    $m = new MongoClient($uri);
    echo "Database connected sucessfully".PHP_EOL;
    $collection = $m->permissions->server;
    echo "Collection connected sucessfully".PHP_EOL;
    
    $serverID = '1';

    $Query = array('serverID' => $serverID);

    $cursor = $collection->find($Query);

    foreach ($cursor as $id) 
    {
      echo $id["serverID"] . "\n" . $id["serverName"] . "\n" . $id["IPaddress"]
      . "\n" . $id["serverDesc"] . "\n" . $id["OwnerID"] . "\n".PHP_EOL;
    }    
    
    /* Show All Users
    $cursor = $collection->find();
    foreach ($cursor as $id => $value) 
    {
      echo "$id: ";
      var_dump( $value );	
    }
    */
  }
  
  catch (MongoConnectionException $e) 
  {
    die('Error connecting to MongoDB server');
  } catch (MongoConnectionException $e) 
  { 
    die('Error: ' . $e->getMessage());
  }
?>

