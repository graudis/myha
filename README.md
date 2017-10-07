-- myHA requirement --

 1. apache-log4cxx-0.10.0
 2. boost 1.65.0
 3. RHEL / CENTOS 7

Steps before compiling myHA source

Adding Extended Repository RPM Files for Installation

" epel-release-7-10.noarch.rpm " or higher

# rpm -Uhv epel-release-7-10.noarch.rpm
# yum clean all
# yum repolist

-- step 1 --

apache-log4cxx library Install

# yum install log4cxx*

-- step 2 --

apache-log4cxx library Install

# yum install boost*

Install all boost dependency libraries


-- build --

root@localhost ]# make 

root@localhost ]# make install 

-- myHA Configuration --

[ notice ]
The MySQL DB server must be configured to replicate between the master DB server and the slave DB server


myHA Confiiguration file is " config.ini "

# cd /usr/local/mysqlha/bin

# vi config.ini

dbvip = 192.168.xxx.xxx

dbvip_nic = eth0

master_ip = 192.168.xxx.xxx

slave_ip = 192.168.xxx.xxx


dbvip = 192.168.xxx.xxx

: DB Server service IP setting

dbvip_nic = eth0

: Setting the network device name used for the DB server

ex) RHEL 6 / CentOS 6 = ethX , RHEL 7 / CentOS 7 = enpXXX

master_ip = 192.168.xxx.xxx

: Master DB server IP setting

slave_ip = 192.168.xxx.xxx

: Slave DB server IP setting


Save the configuration file and exit editing

After all the work is done, run myHA

# ./myha
