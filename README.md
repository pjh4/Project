# IT635 Project Deliverables

#Admin Login
- Add users and set individual permissions
```php 
php loginInterface.php -u admin -p password -c addNewUser -nu User2 -np password -l User
```
- Update a user's permissions
```php
php loginInterface.php -u admin -p password -c modifyPerm -tg User -m  <modifications>
# Permissions are plaintext and set by Groups (Admin, User, Guest)
```
- Add groups and associate users to groups
```php
php loginInterface.php -c modifyGroup -u admin -p password
```
#User Login
- submit a server that a user runs
```php
php loginInterface.php -c addServer -u User -p password -sn <Server Name> -IP <IP Address> -sd <Description>
```
- add a user profile
```php
php loginInterface.php -c addNewUser -u User -p password -nu User2 -np password -l User
```
- view other users
```php
php loginInterface.php -c viewUsers -u User -p password
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
1) Run permissions.sql to setup the database named "permissions" with the tables "groups","login", and "server". This will create the "user" account with the password "password"

2) Commands can be run using the format: 
```php
php loginInterface.php -u <username> -p <password> -c <command>
```
