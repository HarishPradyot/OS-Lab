#! /bin/sh
make refreshdata

echo How do you want to modify the database [reset-0 or refresh-1]:
read operation

if [ $operation -eq 0 ]
then
	make resetdata_run
elif [ $operation -eq 1 ]
then
	make refreshdata_run
fi
