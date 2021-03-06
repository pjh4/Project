#!/usr/bin/php
<?php

require_once("logindb.php.inc");

function print_help()
{
  echo "usage: ".PHP_EOL;
  echo "loginInterface.php -u <username> -p <password> -l <Login Type> -c <command arguments...>".PHP_EOL;
}

if ($argc < 2)
{
   print_help();
   exit(0);
}
$cArgs = array();
for ($i = 0; $i < $argc;$i++)
{
  if (($argv[$i] === "-h") ||
      ($argv[$i] === "--help"))
  {
      print_help();
      exit(0);
  }
  if ($argv[$i] === '-u')
  {
    $username = $argv[$i + 1];
    $i++;
    continue;
  }
  if ($argv[$i] === '-p')
  {
    $password = $argv[$i + 1];
    $i++;
    continue;
  }
  if ($argv[$i] === '-l')
  {
    $permissionLevel = $argv[$i + 1];
    $i++;
    continue;
  }
  if ($argv[$i] === '-tU')
  {
    $targetUser = $argv[$i + 1];
    $i++;
    continue;
  }
  if ($argv[$i] === '-tg')
  {
    $targetGroup = $argv[$i + 1];
    $i++;
    continue;
  }  
  if ($argv[$i] === '-nu')
  {
    $newUser = $argv[$i + 1];
    $i++;
    continue;
  }
  if ($argv[$i] === '-np')
  {
    $newPassword = $argv[$i + 1];
    $i++;
    continue;
  }  
  if ($argv[$i] === '-sn')
  {
    $serverName = $argv[$i + 1];
    $i++;
    continue;
  }  
  if ($argv[$i] === '-sd')
  {
    $serverDesc = $argv[$i + 1];
    $i++;
    continue;
  }  
  if ($argv[$i] === '-IP')
  {
    $IPaddress = $argv[$i + 1];
    $i++;
    continue;
  }    
  if ($argv[$i] === '-c')
  {
    $command = $argv[$i + 1];
    $i++;
    continue;
  }
  $cArgs[] = $argv[$i];  
}

//command requirements

if (!isset($command))
{
   echo "no command specified".PHP_EOL;
   print_help();
   exit(0);
}

//Command Functions
switch ($command)
{
  case 'updatePW':
   $login = new loginDB("logindb.ini");
   $login->updateUserPassword($username,$password,$cArgs[0]);
   break;
  case 'validatePW':
   $login = new loginDB("logindb.ini");
   if ($login->validatePassword($username,$password))
   {
      echo "password validated!!!".PHP_EOL;
   }
   else
   {
      echo "password does not validate!".PHP_EOL;
   }
   break;
  //Admin
  case 'addNewUser':
  $login = new loginDB("logindb.ini");
  $login->addNewUser($username,$password,$newUser,$newPassword,$permissionLevel,$cArgs[0]);
  break;
  case 'deleteUser':
  $login = new loginDB("logindb.ini");
  $login->deleteUser($username,$password,$targetUser,$cArgs[0]);
  break;
  case 'modifyPerm':
  $login = new loginDB("logindb.ini");
  $login->modifyPerm($username,$password,$targetUser,$targetGroup,$cArgs[0]);
  break;
  //User
  case 'addServer':
  $login = new loginDB("logindb.ini");
  $login->addServer($username,$password,$serverName,$IPaddress,$serverDesc,$targetUser,$cArgs[0]);
  break;
  case 'viewUsers':
  $login = new loginDB("logindb.ini");
  $login->viewUsers($username,$password,$cArgs[0]);
  break;
  //Guest
  case 'searchServers':
  $login = new loginDB("logindb.ini");
  $login->searchServers($serverName,$cArgs[0]);
  break;
  case 'viewServers':
  $login = new loginDB("logindb.ini");
  $login->viewServers($cArgs[0]);
  break;
  case 'checkIfUserExists':
  $login = new loginDB("logindb.ini");
  $login->checkIfUserExists($username);
  break;
  case 'sendMongo':
  $login = new loginDB("logindb.ini");
  $login->sendMongo($cArgs[0]);
  break;  
  case 'vUser':
  $login = new loginDB("logindb.ini");
  $login->vUser($username, $password, $cArgs[0]);
  break;
}

?>
