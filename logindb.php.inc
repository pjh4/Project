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

   public function checkIfUserExists($username)
   {
	$username = $this->db->real_escape_string($username);
	$query = "select username from login where username = '$username';";
        $results = $this->db->query($query);
	$row = $results->fetch_assoc();
	if ($row['username'] === $username)
        {
	  return true;
	  echo "True";
	}
        return false;
   }
   
   public function checkIfServerExists($IPaddress)
   {
	$IPaddress = $this->db->real_escape_string($IPaddress);
	$query = "select IPaddress from server where IPaddress = '$IPaddress';";
        $results = $this->db->query($query);
	$row = $results->fetch_assoc();
	if ($row['IPaddress'] === $IPaddress)
        {
		return true;
	}
        return false;
   }   
   
   public function updateUserPassword($username,$oldPassword,$newPassword)
   {
   	if (!$this->validatePassword($username,$oldPassword))
        {
		echo "cannot update Password, validation failed".PHP_EOL;
		$response = array(
			"message"=>"cannot update password, validation failed",
			"result"=>"false");
		return $response;
        }
	$username = $this->db->real_escape_string($username);
	$password = $this->db->real_escape_string($this->saltPassword($newPassword));
	$query = "update login set password = '$password' where username ='$username');";
	$results = $this->db->query($query);
	if ($this->db->errno > 0)
        {
		echo "DB error: ".$this->db->error.PHP_EOL;
		return false;
        }
	return true;
   }

   public function validatePassword($username,$password)
   {
	$username = $this->db->real_escape_string($username);
	$password = $this->db->real_escape_string($this->saltPassword($password));
	$query = "select username,password from login where username = '$username';";
        $results = $this->db->query($query);
	$row = $results->fetch_assoc();
	if ($row['password'] === $password)
	{
		return true;
	}
	return false;
   }
   
   public function validateAdmin($username)
   {
	$username = $this->db->real_escape_string($username);
	$query = "select loginType from groups inner join login on login.loginId = groups.userID where login.username = '$username';";
        $results = $this->db->query($query);
	$row = $results->fetch_assoc();
	if ($row['loginType'] === "Admin")
	{
		return true;
	}
	else
	{
	  return false;
	}
   }   
   
   public function validateUser($username)
   {
	$username = $this->db->real_escape_string($username);
	$query = "select loginType from groups inner join login on login.loginId = groups.userID where login.username = '$username';";
        $results = $this->db->query($query);
	$row = $results->fetch_assoc();
	if ($row['loginType'] === "User")
	{
		return true;
	}
	else
	{
	  return false;
	}
   }   
   private function saltPassword($password)
   {
	$salt = $this->ini['loginDB']['salt'];
	return substr(sha1($password.$salt),10,-10);
   }

   public function addNewUser($username, $password, $newUser, $newPassword, $loginType)
   {
	if (!isset($username))
	{
	  echo "Error: No username specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -nu <new user> -np <new password> -l <group> -c addNewUser".PHP_EOL;
	  exit(0);
	}
	if (!isset($password))
	{
	  echo "Error: No password specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -nu <new user> -np <new password> -l <group> -c addNewUser".PHP_EOL;
	  exit(0);
	}
	if (!isset($newUser))
	{
	  echo "Error: No new user specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -nu <new user> -np <new password> -l <group> -c addNewUser".PHP_EOL;
	  exit(0);
	}    
	if (!isset($newPassword))
	{
	  echo "Error: No new password specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -nu <new user> -np <new password> -l <group> -c addNewUser".PHP_EOL;
	  exit(0);
	}    
	if (!isset($loginType))
	{
	  echo "Error: No group specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -nu <new user> -np <new password> -l <group> -c addNewUser".PHP_EOL;
	  exit(0);
	}    	
	if ($this->checkIfUserExists($newUser) == true)
        {
		echo "cannot create new user $newUser, this user already exists!".PHP_EOL;
		return;
	}
	$newPassword = $this->db->real_escape_string($this->saltPassword($newPassword));
	$loginType = $this->db->real_escape_string($loginType);
	//validate password
   	if (!$this->validatePassword($username,$password))
        {
	    echo "Error: Password incorrect".PHP_EOL;
	    $response = array(
		    "message"=>"Password incorrect",
		    "result"=>"false");
	    return $response;	
	}
	//validate if admin or user
	else if (!$this->validateAdmin($username) & !$this->validateUser($username))
	{
	  echo "Error: User ".$username." does not have access!".PHP_EOL;
	  $response = array(
		  "message"=>"Error: User ".$username." does not have access!",
		  "result"=>"false");
	  return $response;		      
	}
	//runs command
	else
	{
	  if ($this->validateAdmin($username))
	  {
	    $query = "insert into login (username,password) values('$newUser','$this->saltPassword$newPassword');";
	    $results = $this->db->query($query);
	    $query = "insert into groups (userID, loginType) values ((select loginId from login where username = '$newUser'),'$loginType');";
	    $results = $this->db->query($query);
	    echo "User $newUser has been added.".PHP_EOL;
	  }
	  else
	  {
	    //Prevent the User group from adding a new Admin
	    if ($loginType != "User" OR $loginType != "Guest")
	    {
	      echo "Error, invalid login type. Please choose either User or Guest as the login type.".PHP_EOL;
	    }
	    else
	    {
	      $query = "insert into login (username,password) values('$newUser','$this->saltPassword$newPassword');";
	      $results = $this->db->query($query);
	      $query = "insert into groups (userID, loginType) values ((select loginId from login where username = '$newUser'),'$loginType');";
	      $results = $this->db->query($query);
	      echo "User $newUser has been added.".PHP_EOL;
	    }
	  }
	}
   }
   
   public function deleteUser($username, $password, $targetUser)
   {
	if (!isset($username))
	{
	  echo "Error: No username specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -tu <target user> -c deleteUser".PHP_EOL;
	  exit(0);
	}
	if (!isset($password))
	{
	  echo "Error: No password specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -tu <target user> -c deleteUser".PHP_EOL;
	  exit(0);
	}
	if (!isset($targetUser))
	{
	  echo "Error: No target user specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -tu <target user> -c deleteUser".PHP_EOL;
	  exit(0);
	}   
	if ($this->checkIfUserExists($targetUser) == false)
        {
		echo "cannot delete user $targetUser, this user doesn't exists!".PHP_EOL;
		return;
	}
	$targetUser = $this->db->real_escape_string($targetUser);	
	//validate password
   	if (!$this->validatePassword($username,$password))
        {
	    echo "Error: Password incorrect".PHP_EOL;
	    $response = array(
		    "message"=>"Password incorrect",
		    "result"=>"false");
	    return $response;	
	}
	//validate if admin
	else if (!$this->validateAdmin($username))
	{
	  echo "Error: User ".$username." is not an admin!".PHP_EOL;
	  $response = array(
		  "message"=>"Error: User ".$username." is not an admin!",
		  "result"=>"false");
	  return $response;		      
	}
	//runs command
	else
	{
	  $query = "DELETE FROM login WHERE username = '$targetUser' LIMIT 1;";
	  $results = $this->db->query($query);
		  
	  if ($this->db->errno > 0)
	  {
		  echo "DB error: ".$this->db->error.PHP_EOL;
		  return;
	  }
	  echo "User $targetUser has been deleted.".PHP_EOL;
	}
   } 

   public function viewUsers($username,$password)
   {
	if (!isset($username))
	{
	  echo "Error: No username specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -c viewUsers".PHP_EOL;
	  exit(0);
	}
	if (!isset($password))
	{
	  echo "Error: No password specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -c viewUsers".PHP_EOL;
	  exit(0);
	}
	$query = "SELECT username FROM login;";
	$result = $this->db->query($query);
	//validate password
   	if (!$this->validatePassword($username,$password))
        {
	    echo "Error: Password incorrect".PHP_EOL;
	    $response = array(
		    "message"=>"Password incorrect",
		    "result"=>"false");
	    return $response;	
	}
	//validate if admin or user
	else if (!$this->validateAdmin($username) & !$this->validateUser($username))
	{
	  echo "Error: User ".$username." does not have access!".PHP_EOL;
	  $response = array(
		  "message"=>"Error: User ".$username." does not have access!",
		  "result"=>"false");
	  return $response;		      
	}
	//runs command
	else
	{
	  if ($result->num_rows > 0) 
	  {
	      while($row = $result->fetch_assoc()) 
	      {
		  echo " - Username: " . $row["username"].PHP_EOL;
	      }
	  } 
	  else 
	  {
	      echo "0 results";    
	  }
	}
   }
   
   public function modifyPerm($username,$password,$targetUser,$targetGroup)
   {
	if (!isset($username))
	{
	  echo "Error: No username specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -tu <target user> -tg <target group> -c modifyPerm".PHP_EOL;
	  exit(0);
	}
	if (!isset($password))
	{
	  echo "Error: No password specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -tu <target user> -tg <target group> -c modifyPerm".PHP_EOL;
	  exit(0);
	}
	if (!isset($targetGroup))
	{
	  echo "Error: No target group specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -tu <target user> -tg <target group> -c modifyPerm".PHP_EOL;
	  exit(0);
	}
	if (!isset($targetUser))
	{
	  echo "Error: No target user specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -tu <target user> -tg <target group> -c modifyPerm".PHP_EOL;
	  exit(0);
	}	
	//validate password
   	if (!$this->validatePassword($username,$password))
        {
	    echo "Error: Password incorrect".PHP_EOL;
	    $response = array(
		    "message"=>"Password incorrect",
		    "result"=>"false");
	    return $response;	
	}
	//validate if admin
	else if (!$this->validateAdmin($username))
	{
	  echo "Error: User ".$username." is not an admin!".PHP_EOL;
	  $response = array(
		  "message"=>"Error: User ".$username." is not an admin!",
		  "result"=>"false");
	  return $response;		      
	}
	//runs command
	else
	{
	  $query = "update groups inner join login on groups.userID = login.loginId set groups.loginType='$targetGroup' where username='$targetUser';";
	  $results = $this->db->query($query);
	  echo "The user ".$targetUser."'s permissions group has been changed to: ".$targetGroup.PHP_EOL;
	}   
   }
   public function addServer($username,$password,$serverName,$IPaddress,$serverDesc,$targetUser)
   {
	if (!isset($username))
	{
	  echo "Error: No username specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -sn <server name> -IP <IP Address> -sd <server description> -c addServer".PHP_EOL;
	  exit(0);
	}
	if (!isset($password))
	{
	  echo "Error: No password specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -sn <server name> -IP <IP Address> -sd <server description> -c addServer".PHP_EOL;
	  exit(0);
	}
	if (!isset($serverName))
	{
	  echo "Error: No server name specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -sn <server name> -IP <IP Address> -sd <server description> -c addServer".PHP_EOL;
	  exit(0);
	}
	if (!isset($IPaddress))
	{
	  echo "Error: No IP Address specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -sn <server name> -IP <IP Address> -sd <server description> -c addServer".PHP_EOL;
	  exit(0);
	}  
	if (!isset($serverDesc))
	{
	  echo "Error: No server description specified".PHP_EOL;
	  echo "Usage: loginInterface.php -u <username> -p <password> -sn <server name> -IP <IP Address> -sd <server description> -c addServer".PHP_EOL;
	  exit(0);
	}  	
	//check if server address exists
	if ($this->checkIfServerExists($serverName) == true)
        {
		echo "Cannot add new server $serverName, this IP Address $IPaddress is already in use!".PHP_EOL;
		return;
	}
	//validate password
   	if (!$this->validatePassword($username,$password))
        {
	    echo "Error: Password incorrect".PHP_EOL;
	    $response = array(
		    "message"=>"Password incorrect",
		    "result"=>"false");
	    return $response;	
	}
	//validate if admin
	else if (!$this->validateAdmin($username))
	{
	  echo "Error: User ".$username." is not an admin!".PHP_EOL;
	  $response = array(
		  "message"=>"Error: User ".$username." is not an admin!",
		  "result"=>"false");
	  return $response;		      
	}
	//runs command
	else
	{
	  $query = "INSERT INTO server (serverName,IPaddress,serverDesc,OwnerID) 
	  values ('$serverName','$IPaddress','$serverDesc', (SELECT login.loginId FROM login where login.username = '$targetUser'))";
	  $results = $this->db->query($query);
	  if ($this->db->errno > 0)
	  {
		  echo "DB error: ".$this->db->error.PHP_EOL;
		  return;
	  }
	}
        echo "Server $serverName has been added.".PHP_EOL;  
   }   
   public function searchServers($serverName)
   {
	/*
	if (!isset($serverName))
	{
	  echo "Error: No server name specified".PHP_EOL;
	  echo "Usage: loginInterface.php -sn <server name> -c searchServers".PHP_EOL;
	  exit(0);
	}   
	$serverName = $this->db->real_escape_string($serverName);	   
	$query = "SELECT * FROM server where serverName = '$serverName';";
	$result = $this->db->query($query);
	if ($result->num_rows > 0) 
	{
	    while($row = $result->fetch_assoc()) 
	    {
		echo " - Name: " . $row["serverName"]. " - IP Address: " . $row["IPaddress"]. " - Info: " . $row["serverDesc"].PHP_EOL;
	    }
	} 
	else 
	{
	    echo "0 results".PHP_EOL;    
	}
	*/
	
	try 
	{
	  $uri = "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions";
	  $m = new MongoClient($uri);
	  echo  "\n" . "Database connected sucessfully" . "\n".PHP_EOL;
	  $collection = $m->permissions->server;
	  echo "Collection connected sucessfully" . "\n".PHP_EOL;	  

	  $Query = array('serverName' => $serverName);

	  $cursor = $collection->find($Query);
	  
	  foreach ($cursor as $id) 
	  {
	    echo "Display results for Server Name: " . "$serverName" . "\n".PHP_EOL;
	    echo "Server Name: " . $id["serverName"]. " - IP Address: " . $id["IPaddress"]. " - Info: " 
	    . $id["serverDesc"]. " - Owner ID: " .$id["OwnerID"] . "\n" .PHP_EOL;	    
	  }    
	}
	
	catch (MongoConnectionException $e) 
	{
	  die('Error connecting to MongoDB server');
	} catch (MongoConnectionException $e) 
	{ 
	  die('Error: ' . $e->getMessage());
	}	
	
   }
   public function viewServers()
   {
	try 
	{
	  $uri = "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions";
	  $m = new MongoClient($uri);
	  echo  "\n" . "Database connected sucessfully" . "\n".PHP_EOL;
	  $collection = $m->permissions->server;
	  echo "Collection connected sucessfully" . "\n".PHP_EOL;	  
	  
	  $cursor = $collection->find();
	  foreach ($cursor as $id => $value) 
	  {
	    echo "Display results for all servers: " . "\n".PHP_EOL;
	    echo "Server Name: " . $value["serverName"]. " - IP Address: " . $value["IPaddress"]. " - Info: " 
	    . $value["serverDesc"]. " - Owner ID: " .$value["OwnerID"] . "\n" .PHP_EOL;		    
	  }
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
   
   public function sendMongo() 
   {
   try 
    {
      //Connect to Mongo 
      $connection = new MongoClient( "mongodb://pjh4:pjh4njit@ds011902.mlab.com:11902/permissions");
      $mongodb = $connection->selectDB('test');
      echo "collection Login selected sucessfully" . "\n".PHP_EOL;
      //Connect to MySQL
      $con=mysqli_connect("localhost","root","toor","permissions");
      //Convert table to PHP Array
      $query = "select * from server;";
      $result = mysqli_query($con,$query);
      $rows = array();
      
      //Encode to JSON
      while($r = mysqli_fetch_array($result)) 
      {
	$rows[] = $r;
      }
      echo json_encode($rows).PHP_EOL;
      $file = file_get_contents('data.json');
      $data = json_decode($file);
      unset($file);
      file_put_contents('data.json', json_encode($rows));
      unset($rows);
      exec("mongorestore -h ds011902.mlab.com:11902 -d permissions -c test 
	  -u pjh4 -p pjh4njit --authenticationDatabase admin --out mongodb --jsonArray --file data.json");
      mysqli_close($con);
      
      /*
      while ($row = mysqli_fetch_array($fetch, MYSQL_ASSOC)) 
      {
	  $row_array['serverID'] = $row['serverID'];
	  $row_array['serverName'] = $row['serverName'];
	  $row_array['IPaddress'] = $row['IPaddress'];
	  $row_array['serverDesc'] = $row['serverDesc'];
	  $row_array['OwnerID'] = $row['OwnerID'];

	  array_push($return_arr,$row_array);
      }
      echo json_encode($return_arr);
      */
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

  public function vUser($username, $password)
  {
    $db=$this->client->selectDB("permissions");
    
    $collections = $db->login;
    $query=array('username'=> $username, 'password'=> $password);	
    $count= $collection->findOne($query);
    if ($count->count() > 0) 
    {
      echo "You are successully loggedIn";
    }
  }  

}

?>
