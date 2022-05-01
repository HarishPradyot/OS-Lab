#! /bin/sh
# username
echo Enter the Username of the user :
read username

# usergroup
echo Type of User [F/S] :
read usertype

#homedirectory
homedirectory = /home/OS-Demo

pw useradd -n$username -c$username -d$homedirectory -g$usertype
passwd $username

make setupuser
make setupuser_run