syscall::open:entry
/gid==$1/
{
	printf("\ngroup id: %d\tExecutable:%s\nprocess id:%d\tuser id:%d\t%s:%s", gid, execname, pid, uid, probefunc, copyinstr(arg0));
	printf("\n---------------------\n");
}

syscall::access:entry
/gid==$1/
{
	printf("\nExecutable: %s\tprocess id: %d\nuser id: %d\t%s: %s",execname, pid, uid, probefunc, copyinstr(arg0));
	printf("\n---------------------\n");			
}

syscall::read:entry, syscall::write:entry
/gid==$1/
{
	ts[probefunc] = timestamp;
	printf("\nExecutable: %s", execname);
	printf("\n---------------------\n");
}

syscall::read:return, syscall::write:return
/gid == $1 && ts[probefunc] != 0/
{
	printf("%d ns", timestamp - ts[probefunc]);
	printf("\n--------------------\n");
}

syscall:::entry
/gid == $1/
{
	@syscalls[probefunc] = count();
}

syscall::openat:entry
/gid==$1/
{
	printf("\nopening at : %s %s", execname, copyinstr(arg1));
	printf("\n_____________________________________________");
}

proc:::exec
/gid==$1/
{
	printf("%s", stringof(args[0]));
	printf("\n-------------------\n");
}

proc:::exit
/gid == $1/
{
	printf("%d", args[0]);
	printf("\n------------------\n");
}
