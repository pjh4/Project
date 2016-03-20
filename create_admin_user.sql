drop user 'root'@'localhost';
create user 'root'@'localhost' identified by "toor";
grant all privileges on *.* to 'root'@'localhost';
flush privileges;
