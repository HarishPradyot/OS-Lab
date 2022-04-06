#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx,bool hasbox=false)
{
	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	if(hasbox){box(local_win, 0, 0);}
	wrefresh(local_win); 
	return local_win;
}


void destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}

int main(int argc, char **argv)
{
	string file = "data.txt";
	vector<vector<string>> data;
	string line, word;
	ifstream fin(file, ios::in);

	if (fin.is_open())
	{
		while (getline(fin, line))
		{
			vector<string> split;
			stringstream str(line);
			while (getline(str, word, ',')){split.push_back(word);}
			data.push_back(split);
		}
	}
	else
	{
		cout << "Could not open file.";
		exit(1);
	}
	fin.close();

	initscr();
	start_color();
	cbreak();
	keypad(stdscr, TRUE);
	
	init_pair(1, COLOR_RED,COLOR_BLACK);
	init_pair(2, COLOR_GREEN,COLOR_BLACK);

	int tHeight, tWidth, startX, startY;
	tHeight = 15;
	tWidth = 72;
	
	refresh();
	
	WINDOW *table = create_newwin(tHeight + 2, tWidth + 2, 5, 5,true);

	if (!table)
	{
		endwin();
		cout << "Unable to allocate memory to table";
		return 1;
	}
	
	int tRows = 5, tColumns = 6;
	int mvH = tHeight / tRows, mvW = tWidth / tColumns;
	
	WINDOW *dataTable[tRows][tColumns];

	startY = 6;
	startX = 6;
	int rTotal = 0;

	for(int i=0;i<tRows;i++)
	{
		rTotal=0;
		for(int j=0;j<tColumns;j++)
		{
			dataTable[i][j]=create_newwin(mvH,mvW,startY,startX);
			if (!i ||!j)
			{
				if(j==tColumns-1)
				{
					wattron(dataTable[i][j],COLOR_PAIR(1));
					mvwprintw(dataTable[i][j],mvH/2,mvW/4,"%s","Total");
				}
				else
				{
					wattron(dataTable[i][j],COLOR_PAIR(1));
					mvwprintw(dataTable[i][j],mvH/2,mvW/4,"%s",data[i][j].c_str());
				}
			}
			else if(j!=tColumns-1)
			{
				rTotal+=stoi(data[i][j].c_str());
				mvwprintw(dataTable[i][j],mvH/2,mvW/2,"%s",data[i][j].c_str());
			}
			else
			{
				mvwprintw(dataTable[i][j],mvH/2,mvW/2-1,"%d",rTotal);
			}
			startX += mvW;
			wrefresh(dataTable[i][j]);
		}
		startY += mvH;
		startX = 6;
	}
	
	startY = 6 + mvH;
	startX = 6 + mvW;
	int curRow = 1, curColumn = 1;

	wattron(dataTable[curRow][curColumn],COLOR_PAIR(2));
	box(dataTable[curRow][curColumn], 124, 45);
	wrefresh(dataTable[curRow][curColumn]);
	int ch;
	
	while ((ch=getch())!=KEY_F(1))
	{
		switch (ch)
		{
			case KEY_LEFT:
			{
				startX -= mvW;
				curColumn--;
				if (startX < 6 + mvW)
				{
					startX += mvW;
					curColumn++;
				}
				break;
			}
			case KEY_RIGHT:
			{
				startX += mvW;
				curColumn++;
				if (startX >= 66)
				{
					startX -= mvW;
					curColumn--;
				}
				break;
			}
			case KEY_UP:
			{
				startY -= mvH;
				curRow--;
				if (startY < 6 + mvH)
				{
					startY += mvH;
					curRow++;
				}
				break;
			}
			case KEY_DOWN:
			{
				startY += mvH;
				curRow++;
				if (startY >= 20)
				{
					startY -= mvH;
					curRow--;
				}
				break;
			}
			case (int)'\n':
			{
				if(curColumn==tColumns){break;}

				move(startY+mvH/2,startX+mvW/2);
				mvwprintw(dataTable[curRow][curColumn],mvH/2,mvW/2," ");
				wrefresh(dataTable[curRow][curColumn]);

				char inp = getch();
				
				// Handling ESC
				if((int)inp==27){break;}
				if (inp>='0' && inp<='9')
				{
					mvwprintw(dataTable[curRow][curColumn],mvH/2,mvW/2,"%c",inp);
					wrefresh(dataTable[curRow][curColumn]);
					char inp2 = getch();
					if(inp2=='\n')
					{
						data[curRow][curColumn] = inp;
					}
				}
				
				break;
			}
		}

		table=create_newwin(tHeight + 2, tWidth + 2, 5, 5,true);
		int tempStartY = 6;
		int tempStartX = 6;
		int rSum = 0;

		for(int i=0;i<tRows;i++)
		{
			rSum = 0;
			for (int j = 0; j < tColumns; j++)
			{
				dataTable[i][j] = create_newwin(mvH, mvW, tempStartY, tempStartX);
				if (!i || !j)
				{
					if (j == tColumns - 1)
					{
						wattron(dataTable[i][j],COLOR_PAIR(1));
						mvwprintw(dataTable[i][j], mvH / 2, mvW / 4, "%s", "Total");
					}
					else
					{
						wattron(dataTable[i][j],COLOR_PAIR(1));
						mvwprintw(dataTable[i][j], mvH / 2, mvW / 4, "%s", data[i][j].c_str());
					}
				}
				else if (j != tColumns - 1)
				{
					rSum += stoi(data[i][j]);
					mvwprintw(dataTable[i][j], mvH / 2, mvW / 2, "%s", data[i][j].c_str());
				}
				else
				{
					mvwprintw(dataTable[i][j], mvH / 2, mvW / 2 - 1, "%d", rSum);
				}
				tempStartX += mvW;
				wrefresh(dataTable[i][j]);
			}
			tempStartY += mvH;
			tempStartX = 6;
		}

		ofstream fout(file, ios::out);

        if(fout.is_open()) 
		{
            for(int i=0;i<data.size();i++)
			{
                for(int j=0;j<data[i].size()-1;j++) 
				{
                    fout<<data[i][j]<<",";
                }
                fout<<data[i][data.size()-1]<<"\n";
            }
        }
		else 
		{
            cout << "Failed to update the table";
            exit(EXIT_FAILURE);
        }

        fout.close();

		wattron(dataTable[curRow][curColumn],COLOR_PAIR(2));
		box(dataTable[curRow][curColumn], 124, 45);
		wrefresh(dataTable[curRow][curColumn]);
		move(startY + mvH / 2, startX + mvW / 2);
		refresh();
	}
	
	endwin();
	return 0;
}