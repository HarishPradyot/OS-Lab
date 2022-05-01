make refreshdata

echo How do you want to modify the database:
read operation

if [$operation == "reset"]
then
	make resethdata_run
elif [$operation == "refresh"]
then
	make refreshdata_run
fi