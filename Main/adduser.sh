# Get the username of User
echo Enter the Username of the user :
read username

echo Type of User [faculty/students] :
read usertype

useradd -G$usertype $username
passwd $username

make setupuser
make setupuser_run