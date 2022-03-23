#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <dirent.h>
#include <vector>
#include <fcntl.h>
#include <fstream>

using namespace std;
// A text-based user interface in the terminal using ncurses library 
int Terminate(int err_code)
{
	getch();
	// Deallocates memory and ends ncurses
	endwin();
	return err_code;
}
int main(int argc,char *argv[])
{
	// Initializes the screen sets up memory and clears the terminal
	initscr();

	DIR* dir = opendir(".");
	bool writeFiles=false, writeFileCount=false;
    if (dir == NULL) {
        return Terminate(1);
    }

	string extension=".txt";
	
	// First line consists of Max number of entries of each .txt files which is mapped to no of columns in the table
	// Second line of config file consists of No of .txt files which can be mapped to number of rows in the table
	string configFile="./config";

	// No of Rows, No of Columns
	vector<int> tableSize(2, -1);
	vector<string> files;

	FILE* config=fopen(&configFile[0], "a+");
	if(config==NULL)
	{
		printw("Config file does not exist. Terminating\n");
		return Terminate(1);
	}
	int noOfLines=0;
	char c; int t=0;
	while(noOfLines<2 && (c=fgetc(config)) != EOF)
	{
		if(c>='0' && c<='9')
			t=t*10+(c-'0');
		else if(c=='\n')
		{
			tableSize[noOfLines]=t;
			t=0;
			noOfLines++;
			continue;
		}
		else
		{
			printw("Error in config file. Terminating\n");
			return Terminate(1);
		}
	}
	if(tableSize[0]<=0)
	{
		printw("No of columns is not a positive number. Error in config file. Terminating\n");
		return Terminate(1);
	}
	if(tableSize[1]>0)
	{
		files.reserve(tableSize[1]);
		char c; string fname;
		while((c=fgetc(config)) != EOF)
		{
			if(c=='\n')
			{
				files.push_back(fname);	
				fname.clear();
				continue;
			}
			fname+=c;
		}
	}
	else
		writeFileCount=true;

	if(files.size()==0)
	{
		writeFiles=true;
		struct dirent* entity;
		entity = readdir(dir);
		while (entity != NULL) 
		{
			if(entity->d_type == DT_REG)
			{	
				string fname=entity->d_name;
				if (fname.find(extension, (fname.length() - extension.length())) != string::npos)
					files.push_back("./"+fname);
			}
			entity = readdir(dir);
		}
		closedir(dir);
	}
	
	// If there are no files in the directory then exit from program
	if(files.size()==0)
	{
		printw("No files in the directory. Error in config file. Terminating\n");
		return Terminate(1);
	}
	
	// Writing if config file does not contains appropriate details
	if(writeFileCount)
	{
		tableSize[1]=files.size();
		fputs(&(to_string(files.size())+"\n")[0], config);
		if(writeFiles)
			for(vector<string>::iterator it=files.begin(); it!=files.end(); it++)
				fputs(&((*it)+"\n")[0], config);
	}
	fclose(config);

	vector<vector<int>> table;
	table.reserve(tableSize[1]);
	for(int i=0;i<table.capacity();i++)
	{
		vector<int> column;
		column.reserve(tableSize[0]);
		FILE* file=fopen(&files[i][0], "r+");
		if(file!=NULL)
		{
			char c; int t=0;
			while((c=fgetc(file)) != EOF)
			{
				if(c=='\n')
				{
					column.push_back(t);
					t=0;
					continue;
				}
				t=t*10+(c-'0');
			}
			fclose(file);
		}
		else
		{
			printw("File does not exist. Error in config file. Terminating\n");
			return Terminate(1);
		}
		table.push_back(column);
	}
	
	for(int i=0;i<table.size();i++)
	{
		vector<int> column=table[i];
		for(vector<int>::iterator it=column.begin(); it!=column.end(); it++)
			printw("%d ", *it);
		printw("\n");
	}

	// Display the data as a table

	return Terminate(0);
}