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
void write_extra_fac(FILE* file, vector<user>::iterator start, vector<user> fac, bool zeroes=false)
{
	fseek(file, -1, SEEK_CUR);
	if(!zeroes)
	{
		for(; start!=fac.end(); start++)
			fputs(&(to_string(start->first)+" ")[0], file);
		fseek(file, -1, SEEK_CUR);
		fputc('\n', file);
	}
	else
	{
		for(int i=0; i<fac.end()-start; i++)
			fputs("0 ", file);
		fseek(file, -1, SEEK_CUR);
		fputc('\n', file);
	}
}
void write_extra_stu(FILE* file, vector<user>::iterator start, vector<user> stu, int facs)
{
	fseek(file, -1, SEEK_CUR);
	for(; start!=stu.end(); start++)
	{
		fputs(&(to_string(start->first)+" ")[0], file);
		for(int i=0; i<facs; i++)
			fputs("0 ", file);
	}
	fseek(file, -1, SEEK_CUR);
	fputc('\n', file);
}
vector<user> get_users(FILE* fp)
{
    vector<user> users;
    user temp;
	while(true)
	{
		char c;
		temp.first=0;
		temp.second.clear();
		while((c=fgetc(fp)) != EOF)
		{
			if(c==' ')
				break;
			temp.first=temp.first*10+(c-'0');
		}
		while((c=fgetc(fp)) != EOF)
		{
			if(c=='\n')
			{
				users.push_back(temp);
				break;
			}
			temp.second=temp.second+c;
		}
		if(c==EOF)
			break;
	}
	sort(users.begin(), users.end(), &comp);
	return users;
}
int refresh_database(string file_name, bool reset=true)
{	
	// Reset or modify back to database
	string fname="./"+file_name+".db";
	FILE* file;
	if(reset)
	{
		file=fopen(&fname[0], "w");
		FILE* S=fopen("./S.db", "r");
		vector<user> stu=get_users(S);
		fclose(S);

		FILE* F=fopen("./F.db", "r");
		vector<user> fac=get_users(F);
		fclose(F);

		// Heading
		fputs("S/F ", file);
		for(vector<user>::iterator start=fac.begin(); start!=fac.end(); start++)
			fputs(&(to_string(start->first)+" ")[0], file);
		fseek(file, -1, SEEK_CUR);
		fputc('\n', file);

		// Record for each student
		for(vector<user>::iterator start=stu.begin(); start!=stu.end(); start++)
		{
			fputs(&(to_string(start->first)+" ")[0], file);
			for(int i=0;i<fac.size();i++)
				fputs("0 ", file);
			fseek(file, -1, SEEK_CUR);
			fputc('\n', file);
		}
	}	
	else
	{
		file=fopen(&fname[0], "a+");
		string x="";
		FILE* S=fopen("./S.db", "r");
		vector<user> stu=get_users(S);
		fclose(S);

		FILE* F=fopen("./F.db", "r");
		vector<user> fac=get_users(F);
		fclose(F);

		// Processing first line to get number of faculty already present
		char c;
		uid_t temp=0;
		while(c=fgetc(file) != '\n')
		{
			cout<<c<<" this ";
			if(c==' ')
			{
				temp=0;
				continue;
			}
			temp=temp*10+(c-'0');
		}
		cout<<temp<<endl;
		vector<user>::iterator start=upper_bound(fac.begin(), fac.end(), make_pair(temp, x));
		cout<<"Here";
		cout<<start->first<<"    "<<start->second<<endl;
		write_extra_fac(file, start, fac);

		temp=0;
		while(true)
		{
			temp=0;
			while(c=fgetc(file) != EOF)
			{
				if(c==' ')
					break;
				temp=temp*10+(c-'0');
			}
			while(c=fgetc(file) != EOF)
			{
				if(c=='\n')
					write_extra_fac(file, start, fac, true);
			}
			if(c==EOF)
				break;
		}
		start=upper_bound(stu.begin(), stu.end(), make_pair(temp, x));
		cout<<start->first<<"    "<<start->second<<endl;
		write_extra_stu(file, start, stu, fac.size());
	}	
	fclose(file);
}
int main(int argc, char* argv[])
{
	if(argc==1)
		refresh_database("data");
	
	else if(argc==2)
	{
		if(argv[1][0] == '0')
			refresh_database("data");
		else if(argv[1][0] == '1')
			refresh_database("data", false);
	}
}