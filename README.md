# IT635 Project Deliverables

#Admin Login
- Add users and set individual permissions
```php 
php loginInterface.php -u <User> -p <Password> -c addNewUser -nu <New User> -np <New Password> -l <Admin, User, Guest>
```
- Modify a user's permission group
```php
php loginInterface.php -u <User> -p <Password> -c modifyPerm -tu <Target User> -tg <Admin, User, Guest>

#User Login
- submit a server that a user runs
```php 
php loginInterface.php -c addServer -u <User> -p <Password> -sn <Server Name> -tu <Target User> -IP <IP Address> -sd <Description>
```
- add a user profile
```php
php loginInterface.php -c addNewUser -u <User> -p <Password> -nu <New User> -np <Password> -l User
```
- view other users
```php
php loginInterface.php -c viewUsers -u <User> -p <Password>
```

#Guest Login
- Search for servers
```php
php loginInterface.php -c searchServers -sn <ServerName>
```
- view server information
```php
php loginInterface.php -c viewServers
```

# Instructions
1) Run permissions.sql to setup the database named "permissions" with the tables "groups","login", and "server". This will create the "user" account with the <Password> "<Password>"

2) Commands can be run using the format: 
```php
php loginInterface.php -u <username> -p <<Password>> -c <command>
```
