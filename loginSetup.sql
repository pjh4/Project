drop database if exists permissions;
create database permissions;
use permissions;
drop table if exists login;
create table login
(
   loginId INT(11) primary key auto_increment,
   username VARCHAR(32),
   password VARCHAR(255),
   loginType ENUM ('Admin','User','Guest')
);

INSERT INTO login VALUES (1,'root','ce4b18ea16454fdd809a','Admin'),(2,'admin','ffce719cfe7991c8d0c0','Admin'),(3,'TK4210','28c628bb80948b3f6637','User');

create table server
(
  serverID INT(11) primary key auto_increment,
  serverName VARCHAR(32),
  IPaddress VARCHAR(15),
  serverDesc VARCHAR(255)
);
insert into server values (' ','TK4210s server', '173.50.12.11:10050', 'This is my server');

create table groups
(
  loginType ENUM ('Admin','User','Guest'),
  permissions VARCHAR (255)
);

insert into groups values ('Admin','*');
insert into groups values ('User','Kick, Ban, Mute, Gamemode, Viewplayers, Serverinfo');
insert into groups values ('Guest','Calladmin, Viewplayers, Serverinfo');