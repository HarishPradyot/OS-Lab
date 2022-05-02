#include <iostream>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdio.h>
#include <errno.h>
#define user pair<uid_t, string>
using namespace std;

bool comp(user a, user b)
{
	if(a.first<b.first)
		return true;
	else
		return false;
}
user getLastEntry(FILE* fp)
{
	if(ftell(fp)==0)
		return make_pair(0, "");
		
	fseek(fp, -2, SEEK_END);
	char c;
	while(true)
	{
		if(ftell(fp)==0)
			break;
		c=fgetc(fp);
		if(c=='\n')
			break;
		else
			fseek(fp, -2, SEEK_CUR);
	}

	user lastEntry;
	lastEntry.first=0;
	while((c=fgetc(fp)) != EOF)
	{
		if(c==' ')
			break;
		lastEntry.first=lastEntry.first*10+(c-'0');
	}
	while((c=fgetc(fp)) != EOF)
	{
		if(c=='\n')
			break;
		lastEntry.second=lastEntry.second+c;
	}
	return lastEntry;
}
int setup_database(string user_type)
{
	vector<user> users;
	group grp;
	group* grpptr=&grp;
	group* tempGrpPtr;
	passwd* tempuser;
	char grpbuffer[200];
	int  grplinelen = sizeof(grpbuffer);

	if ((getgrnam_r(&user_type[0],grpptr,grpbuffer,grplinelen,&tempGrpPtr))!=0)
		perror("getgrnam_r() error.");
	else
	{
		printf("\nThe group name is\t: %s\n", grp.gr_name);
		printf("The gid is\t\t: %u\n", grp.gr_gid);
		for(; *(grp.gr_mem)!=NULL; (grp.gr_mem)++)
		{
			tempuser=getpwnam(*(grp.gr_mem));
			user temp=make_pair(tempuser->pw_uid, *(grp.gr_mem));
			users.push_back(temp);
		}
	}
	if(users.size()==0)
		return 1;
	//Sort users based on uid_t
	sort(users.begin(), users.end(), &comp);

	//Write back to database
	string fname="./"+user_type+".db";
	FILE* file=fopen(&fname[0], "a+");
	user lastEntry=getLastEntry(file);
	cout<<lastEntry.first<<"   "<<lastEntry.second<<endl;
	vector<user>::iterator start=upper_bound(users.begin(), users.end(), lastEntry);
	//cout<<users.size()<<endl;
	//for(int i=0;i<users.size();i++)
	//	cout<<users[i].first<<"  "<<users[i].second<<endl;
	for(;start!=users.end();start++)
		fputs(&(to_string(start->first)+" "+start->second+"\n")[0], file);
	fclose(file);
	return 0;
}
int main()
{
	setup_database("S");
	setup_database("F");
	return 0;
}
