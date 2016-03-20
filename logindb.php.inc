<?php

class loginDB
{
   private $db;
   private $ini;
//   private $sql;
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
	}
        return false;
   }
   
   public function updateUserPassword($username,$oldPassword,$newPassword)
   {
   	if (!$this->validateUser($username,$oldPassword))
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

   public function validateUser($username,$password)
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
	$query = "select loginType from login where username = '$username';";
        $results = $this->db->query($query);
	$row = $results->fetch_assoc();
	if ($row['loginType'] === 'Admin')
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
	if ($this->checkIfUserExists($newUser) == true)
        {
		echo "cannot create new user $newUser, this user already exists!".PHP_EOL;
		return;
	}
	$username = $this->db->real_escape_string($username);
	$password = $this->db->real_escape_string($this->saltPassword($password));
	$loginType = $this->db->real_escape_string($loginType);
	$query = "insert into login (username,password,loginType) values('$newUser','$this->saltPassword$password','$loginType');";
	$results = $this->db->query($query);
	if ($this->db->errno > 0)
        {
		echo "DB error: ".$this->db->error.PHP_EOL;
		return;
        }
        echo "User $newUser has been added.".PHP_EOL;
   }
   public function deleteUser($username, $password, $targetUser)
   {
	if ($this->checkIfUserExists($targetUser) == false)
        {
		echo "cannot delete user $targetUser, this user doesn't exists!".PHP_EOL;
		return;
	}
	$targetUser = $this->db->real_escape_string($targetUser);	
	
   	if (!$this->validateUser($username,$password))
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

   public function modifyGroup($username, $password, $targetUser, $loginType)
   {
	$username = $this->db->real_escape_string($username);
	$password = $this->db->real_escape_string($this->saltPassword($password));
	$loginType = $this->db->real_escape_string($loginType);	
	//validate password
   	if (!$this->validateUser($username,$password))
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
	  $query = "update login set loginType = '$loginType' where username = '$targetUser';";
	  $results = $this->db->query($query);
	  echo "User ".$targetUser."'s login type changed to: ".$loginType.PHP_EOL;
	}
   }
   public function viewUsers()
   {
	$query = "SELECT username, loginType FROM login;";
	$result = $this->db->query($query);
	if ($result->num_rows > 0) 
	{
	    while($row = $result->fetch_assoc()) 
	    {
		echo " - Username: " . $row["username"]. " - Group: " . $row["loginType"].PHP_EOL;
	    }
	} 
	else 
	{
	    echo "0 results";    
	}
   }
   public function modifyPerm($username,$password,$targetGroup,$modification)
   {
   	if (!$this->validateUser($username,$password))
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
	  $query = "update groups set permissions = '$modification' where loginType = '$targetGroup';";
	  $results = $this->db->query($query);
	  echo "Group ".$targetGroup."'s permissions changed to: ".$modification.PHP_EOL;
	}   
   }
   public function addServer($serverName)
   {
   }   
   public function searchServers($serverName)
   {
   }
   public function viewServerInfo($serverName)
   {
   }  
}

?>

