#include <iostream>
#include <stdlib.h>
#include <ncurses.h>
#include <dirent.h>
#include <vector>
#include <fcntl.h>
#include <fstream>

using namespace std;
int readFile(vector<int> &col, string filename)
{
	FILE* file=fopen(&filename[0], "r");
	char c; int t=0;
	while((c=fgetc(file)) != EOF)
	{
		if(c=='\n')
		{
			printw("%d  ", t);
			//col.push_back(t);
			t=0;
			continue;
		}
		t=t*10+(c-'0');
	}
	fclose(file);
}
// A text-based user interface in the terminal using ncurses library 
int main(int argc,char *argv[])
{
	// Initializes the screen sets up memory and clears the terminal
	initscr();

	// Prints a string to the window
	printw("Hello World\n");

	DIR* dir = opendir(".");
    if (dir == NULL) {
        return 1;
    }

	string extension=".txt";
	
	// First line consists no of columns which is mapped to max number of entries of each .txt files 
	// Second line of config file consists of No of .txt files which can be mapped to number of rows in the table
	string configFile="./config";

	// No of Rows, No of Columns
	vector<int> tableSize(2, -1);
	vector<string> files;

	FILE* config=fopen(&configFile[0], "a+");
	int noOfLines=0;
	char c; int t=0;
	while(noOfLines<2 && (c=fgetc(config)) != EOF)
	{
		if(c=='\n')
		{
			tableSize[noOfLines]=t;
			t=0;
			noOfLines++;
			continue;
		}
		t=t*10+(c-'0');
	}
	if(tableSize[0]<=0)
	{
		printw("No of columns is not a positive number. Terminating");
		getch();
		return 1;
	}
	if(tableSize[1]>0)
	{
		files.reserve(tableSize[1]);
		string fname;
		while(fgets(&fname[0], 10, config) != NULL)
		{
			files.push_back(fname);
			printw(fname.c_str());
		}
	}	
	if(files.size()==0)
	{
		struct dirent* entity;
		entity = readdir(dir);
		while (entity != NULL) 
		{
			if(entity->d_type == DT_REG)
			{	
				string fname=entity->d_name;
				if (fname.find(extension, (fname.length() - extension.length())) != string::npos)
				{
					files.push_back("./"+fname+"\n");
					printw("%hhd %s\n", entity->d_type, entity->d_name);
				}
			}
			entity = readdir(dir);
		}
		closedir(dir);
	}
	
	// If there are no files in the directory then exit from program
	if(files.size()==0)
		return 1;

	if(tableSize[1]<=0)
	{
		tableSize[1]=files.size();
		fputs((to_string(files.size())+"\n").c_str(), config);
	}
	printw("%d\n", files.size());
	for(vector<string>::iterator it=files.begin(); it!=files.end(); it++)
	{
		fputs(&(*it)[0], config);
		printw("%s", &(*it)[0]);
	}
	fclose(config);

	printw("%d %d\n", tableSize[0], tableSize[1]);
	vector<vector<int>> table;
	table.reserve(tableSize[1]);
	for(int i=0;i<table.capacity();i++)
	{
		vector<int> column;
		column.reserve(tableSize[0]);
		//readFile(column, files[i]);
		FILE* file=fopen(&files[i][0], "r+");
		if(file!=NULL)
		{
			char c; int t=0;
			while((c=fgetc(file)) != EOF)
			{
				printw("%c  ", c);
				if(c=='\n')
				{
					printw("%d  ", t);
					//column.push_back(t);
					t=0;
					continue;
				}
				t=t*10+(c-'0');
			}
			fclose(file);
		}
		else 
		{
			printw("It was Null\n");
			printw(files[i].c_str());
		}
		table.push_back(column);
	}
	
	printw("Here\n");
	for(int i=0;i<table.size();i++)
	{
		vector<int> column=table[i];
		for(vector<int>::iterator it=column.begin(); it!=column.end(); it++)
			printw("%d  ", *it);
		printw("\n");
	}


	getch();

	// Deallocates memory and ends ncurses
	endwin();
	return 0;
}