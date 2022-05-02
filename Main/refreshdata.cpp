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
void write_extra_fac(FILE* file, vector<user>::iterator start, vector<user> &fac, bool zeroes=false)
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
void write_extra_stu(FILE* file, vector<user>::iterator start, vector<user> &stu, int facs)
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
		// Unable to write back at current file pointer position
		// file=fopen(&fname[0], "a+");
		// string x="";
		// FILE* S=fopen("./S.db", "r");
		// vector<user> stu=get_users(S);
		// fclose(S);

		// FILE* F=fopen("./F.db", "r");
		// vector<user> fac=get_users(F);
		// fclose(F);

		// // Processing first line to get number of faculty already present
		// char c;
		// uid_t temp=0;
		// while((c=fgetc(file)) != '\n')
		// {
		// 	if(c==' ')
		// 	{
		// 		temp=0;
		// 		continue;
		// 	}
		// 	temp=temp*10+(c-'0');
		// }
		// vector<user>::iterator start=upper_bound(fac.begin(), fac.end(), make_pair(temp, x));
		// if(start->first == temp)
		// 	start++;
		// if(start!=fac.end())
		// 	write_extra_fac(file, start, fac);

		// temp=0;
		// while(true)
		// {
		// 	temp=0;
		// 	while((c=fgetc(file)) != EOF)
		// 	{
		// 		if(c==' ')
		// 			break;
		// 		temp=temp*10+(c-'0');
		// 	}
		// 	while((c=fgetc(file)) != EOF)
		// 	{
		// 		if(c=='\n')
		// 		{
		// 			if(start!=fac.end())
		// 				write_extra_fac(file, start, fac, true);
		// 			break;
		// 		}
		// 	}
		// 	if(c==EOF)
		// 		break;
		// }
		// start=upper_bound(stu.begin(), stu.end(), make_pair(temp, x));
		// if(start->first == temp)
		// 	start++;
		// write_extra_stu(file, start, stu, fac.size());

		vector<vector<string>> data;
		file=fopen(&fname[0], "r");
		
		string x="";
		FILE* S=fopen("./S.db", "r");
		vector<user> stu=get_users(S);
		fclose(S);

		FILE* F=fopen("./F.db", "r");
		vector<user> fac=get_users(F);
		fclose(F);

		char c;
		string temp;
		vector<string> row;
		while((c=fgetc(file)) != EOF)
		{
			if(c==' ')
			{
				row.push_back(temp);
				temp.clear();
				continue;
			}
			else if(c=='\n')
			{
				row.push_back(temp);
				temp.clear();
				data.push_back(row);
				row.clear();
				continue;
			}
			temp=temp+c;
		}
		fclose(file);
		
		vector<vector<string>>::iterator start=data.begin();
		vector<vector<string>>::iterator end=data.end()-1;
		vector<user>::iterator extra=upper_bound(fac.begin(), fac.end(), make_pair((uid_t)stoi(*(start->end()-1)), x));
		if(extra->first == (uid_t)stoi(*(start->end()-1)))
			extra++;

		if(extra != fac.end())
		{
			for(vector<user>::iterator temp=extra; temp!=fac.end(); temp++)
				start->push_back(to_string(temp->first));
			for(vector<vector<string>>::iterator temp=start+1; temp!=data.end(); temp++)
			{
				for(int i=0; i<fac.end()-extra; i++)
					temp->push_back("0");
			}
		}
		extra=upper_bound(stu.begin(), stu.end(), make_pair((uid_t)stoi(*(end->begin())), x));
		if(extra->first == (uid_t)stoi(*(end->begin())))
			extra++;
		
		if(extra != stu.end())
		{
			x="0";
			vector<string> val(fac.size()+1, x);
			for(vector<user>::iterator temp=extra; temp!=stu.end(); temp++)
			{
				val[0]=to_string(extra->first);
				data.push_back(val);
			}
		}

		file=fopen(&fname[0], "w");		
		start=data.begin();
		for(;start!=data.end();start++)
		{
			for(vector<string>::iterator temp=start->begin(); temp!=start->end(); temp++)
				fputs(&(*temp+" ")[0], file);
			fseek(file, -1, SEEK_CUR);
			fputc('\n', file);
		}
	}	
	fclose(file);
	return 0;
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
	return 0;
}
