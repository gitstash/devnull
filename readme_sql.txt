1. run following command in terminal

sudo apt-get install mysql-server

during the installation process it will ask for root password

2. to check whether the Mysql server is installed & running properly,run 

sudo netstat -tap | grep mysql

you sholud see now the following lines or something similar:

tcp        0      0 localhost.localdo:mysql *:*                     LISTEN      13059/mysqld 

3. If in case your server is not running properly you can restart it by running following command
sudo /etc/init.d/mysql restart

Using Mysql
4. when mysql is installed, you can start it by running:

mysql -u root -p

the command prompt will change now to mysql>

5. to create database use:
create database databasename;
6. to use database use:
use databasename;

from here on you can create table,insert into table and peroform other mysql actions

1. run following command in terminal

sudo apt-get install mysql-server

during the installation process it will ask for root password

2. to check whether the Mysql server is installed & running properly,run 

sudo netstat -tap | grep mysql

you sholud see now the following lines or something similar:

tcp        0      0 localhost.localdo:mysql *:*                     LISTEN      13059/mysqld 

3. If in case your server is not running properly you can restart it by running following command
sudo /etc/init.d/mysql restart

Using Mysql
4. when mysql is installed, you can start it by running:

mysql -u root -p

the command prompt will change now to mysql>

5. to create database use:
create database databasename;
6. to use database use:
use databasename;

SHOW DATABASES 
SHOW SCHEMAS 



sample:

ALTER TABLE Person MODIFY P_Id INT(11) NOT NULL;


to see column properties
mysql> show create table EMPLOYEES;

used following link for practice
http://www.sqlcourse2.com/
