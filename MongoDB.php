<?php
// connect
$uri = "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions";


try {
	$m = new MongoClient($uri);	
	echo "Collection Selected sucessfully".PHP_EOL;
}
catch (MongoConnectionException $e) {
	die('Error connecting to MongoDB server'.PHP_EOL);
} 

// select a database
$db = $m->permissions;

// select a collection
$collection = $db->login;

$Username = 'Pikachu';

$Query = array('username' => $Username);

$cursor = $collection->find($Query);

foreach ($cursor as $id) 
{
  echo $id["username"] . "\n";
}


//close connection
$m->close();
?>