DROP TABLE IF EXISTS `groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `groups` (
  `groupID` varchar(32) DEFAULT NULL,
  `userID` int (50) DEFAULT NULL,
  `permissionLevel` int(100) DEFAULT NULL,
  PRIMARY KEY (`groupID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `groups`
--

LOCK TABLES `groups` WRITE;
/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
INSERT INTO `groups` VALUES ('Ad1','1',100),('Ad2','2',100),('Us1','5',200),('Us2','15',200),('Gu1','16',300),('Gu2','24',300);
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `login`
--

DROP TABLE IF EXISTS `login`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `login` (
  `loginId` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(32) DEFAULT NULL,
  `password` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`loginId`)
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `login`
--

LOCK TABLES `login` WRITE;
/*!40000 ALTER TABLE `login` DISABLE KEYS */;
INSERT INTO `login` VALUES (1,'root','ce4b18ea16454fdd809a'),(2,'admin','ffce719cfe7991c8d0c0'),(5,'TK4210','ffce719cfe7991c8d0c0'),(15,'Pikachu','36383e25b89de1bc3c1b'),(16,'MegaMan','cdaa80dc4fecc0be9ed8'),(24,'User','c3b1f8d04f800f385a09');
/*!40000 ALTER TABLE `login` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `server`
--

DROP TABLE IF EXISTS `server`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server` (
  `serverID` int(11) NOT NULL AUTO_INCREMENT,
  `serverName` varchar(32) DEFAULT NULL,
  `IPaddress` varchar(15) DEFAULT NULL,
  `serverDesc` varchar(255) DEFAULT NULL,
  `OwnerID` int(11) DEFAULT NULL,
  PRIMARY KEY (`serverID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `server`
--

LOCK TABLES `server` WRITE;
/*!40000 ALTER TABLE `server` DISABLE KEYS */;
INSERT INTO `server` VALUES (1,'TK4210s server','173.50.12.11:10','This is my server','5'),(2,'Onett','127.0.0.1','The homeliest server','2'),(3,'Test','167.14.121.1','This is a test server','15'),(4,'Test','18.92.9.0','This is a test server','24');
/*!40000 ALTER TABLE `server` ENABLE KEYS */;
UNLOCK TABLES;
