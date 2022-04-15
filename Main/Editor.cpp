#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <grp.h>
#include <unistd.h>
#include <cstring>
using namespace std;

/*
initscr() - initializes the screen a blank screen
cbreak() - disables the line buffer and echoing
refresh() - dumps the content of window buffer onto the screen
keypad() - activates the function keys and arrows
endwin()
create_win() - creates a window
move() - moves the cursor to a point
wrefresh() - refreshes the window
box() - draws a box around the window

mvwprintw(w,y, x, string)
Move to (y, x) relative to window
co-ordinates and then print
*/

WINDOW *create_newwin(int height, int width, int startY, int startX);
void destroy_win(WINDOW *local_win);

vector<vector<string>> createUI(vector<vector<string>> data, bool total)
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);

	// Table Dimensions.
	int tHeight, tWidth, startX, startY;
	int tRows = data.size(), tColumns = data[0].size() + total;

	// TODO: set the width and height dynamically
	tHeight = 3 * tRows;
	tWidth = 15 * tColumns;

	refresh();

	// Creating the main window
	WINDOW *table = create_newwin(tHeight + 2, tWidth + 2, 5, 5);

	if (!table)
	{
		endwin();
		cout << "Unable to allocate memory to table";
		exit(1);
	}

	// The individual windows(cells) in the main window
	WINDOW *dataTable[tRows][tColumns];
	int mvH = tHeight / tRows, mvW = tWidth / tColumns;

	int rTotal = 0;
	startY = 6;
	startX = 6;

	for (int i = 0; i < tRows; i++)
	{
		rTotal = 0;

		for (int j = 0; j < tColumns; j++)
		{
			dataTable[i][j] = create_newwin(mvH, mvW, startY, startX);

			if (!i || !j)
			{
				if (total && (j == tColumns - 1))
				{
					mvwprintw(dataTable[i][j], mvH / 2, mvW / 4, "%s", "Total");
				}
				else
				{
					mvwprintw(dataTable[i][j], mvH / 2, mvW / 4, "%s", data[i][j].c_str());
				}
			}
			else if (!total || j != tColumns - 1)
			{
				rTotal += stoi(data[i][j].c_str());
				mvwprintw(dataTable[i][j], mvH / 2, mvW / 2, "%s", data[i][j].c_str());
			}
			else if (total)
			{
				mvwprintw(dataTable[i][j], mvH / 2, mvW / 2 - 1, "%d", rTotal);
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
	box(dataTable[curRow][curColumn], 124, 45);
	wrefresh(dataTable[curRow][curColumn]);

	int ch;
	while ((ch = getch()) != KEY_F(1))
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
			if (startX > (tColumns - total) * mvW)
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
			if (startY > mvH * (tRows + 1))
			{
				startY -= mvH;
				curRow--;
			}
			break;
		}
		case (int)'\n':
		{
			// TODO: If student then don't allow this.

			move(startY + mvH / 2, startX + mvW / 2);

			mvwprintw(dataTable[curRow][curColumn], mvH / 2,
					  mvW / 2, " ");
			wrefresh(dataTable[curRow][curColumn]);

			char inp = getch();

			mvwprintw(dataTable[curRow][curColumn], mvH / 2,
					  mvW / 2, "%c", inp);
			wrefresh(dataTable[curRow][curColumn]);

			char inp2 = getch();
			if (inp2 == '\n')
			{
				if (inp >= '0' && inp <= '9')
				{
					data[curRow][curColumn] = inp;
				}
			}
			break;
		}
		}
		// Re-draw the window
		table = create_newwin(tHeight + 2, tWidth + 2, 5, 5);

		int tempStartY = 6;
		int tempStartX = 6;
		int rSum = 0;
		for (int i = 0; i < tRows; i++)
		{
			rSum = 0;

			for (int j = 0; j < tColumns; j++)
			{
				dataTable[i][j] = create_newwin(mvH, mvW, tempStartY, tempStartX);

				if (!i || !j)
				{
					if (total && j == tColumns - 1)
					{
						mvwprintw(dataTable[i][j], mvH / 2, mvW / 4, "%s", "Total");
					}
					else
					{
						mvwprintw(dataTable[i][j], mvH / 2, mvW / 4, "%s", data[i][j].c_str());
					}
				}
				else if (!total || j != tColumns - 1)
				{
					rSum += stoi(data[i][j]);
					mvwprintw(dataTable[i][j], mvH / 2, mvW / 2, "%s", data[i][j].c_str());
				}
				else if (total)
				{
					mvwprintw(dataTable[i][j], mvH / 2, mvW / 2 - 1, "%d", rSum);
				}
				tempStartX += mvW;
				wrefresh(dataTable[i][j]);
			}
			tempStartY += mvH;
			tempStartX = 6;
		}
		box(dataTable[curRow][curColumn], 45, 45);
		wrefresh(dataTable[curRow][curColumn]);
		move(startY + mvH / 2, startX + mvW / 2);
		refresh();
	}
	endwin();
	return data;
}

vector<vector<string>> retrieve_marks_data(int utype, uid_t uid)
{
	ifstream fp("data.db");
	string line;
	vector<vector<string>> data;
	while (getline(fp, line))
	{
		stringstream ss(line);
		string word;
		vector<string> temp;
		while (ss >> word)
		{
			temp.push_back(word);
		}
		data.push_back(temp);
	}
	fp.close();
	int rows = data.size();
	int cols = data[0].size();
	// modify the first column data
	// modify the first row 
	vector<vector<string>> extracted_data;
	if(utype == 1){
		for(int i=0;i<rows;i++){
			if(!i) extracted_data.push_back(data[i]);
			else{
				if(stoi(data[i][0]) == (int)uid){
					extracted_data.push_back(data[i]);
					break;
				}
			}
		}
	}
	if(utype == 2){
		int f_col = 0;
		for(int i=1;i<cols;i++){
			if(stoi(data[0][i]) == (int)uid){
				f_col = i;
				break;
			}
		}
		for(int i=0;i<rows;i++){
			extracted_data.push_back({data[i][0], data[i][f_col]});
		}
	}
	else return data;
	
	return extracted_data;
}

gid_t getGroupIdByName(const char *name)
{
	struct group *grp = getgrnam(name); /* don't free, see getgrnam() for details */
	if (grp == NULL)
	{
		throw runtime_error(string("Failed to get groupId from groupname : ") + name);
	}
	return grp->gr_gid;
}

void write_back(vector<vector<string>> final_data, int utype, uid_t uid)
{
	vector<vector<string>> database = retrieve_marks_data(-1, -1); 
	
	if(utype==2){  // faculty changes could have been made
		int col = -1;
		for(int i=0; i<database[0].size(); i++){
			if(stoi(database[0][i])==(int)uid){
				col = i;
				break;
			}
		}

		for(int i=1; i<database.size(); i++){
			database[i][col] = final_data[i][1];
		}
	}
	else if(utype==1){
		int row = -1;
		for(int i=1; i<database.size(); i++){
			if(stoi(database[i][0])==(int)uid){
				row = i;
			}
		}

		for(int i=1; i<database[0].size(); i++){
			database[row][i] = final_data[1][i];
		} 
	}

	ofstream write_fp;
	write_fp.open("data.db");
	for (int i = 0; i < database.size(); i++)
	{
		for (int j = 0; j < database[i].size(); j++)
		{
			write_fp << database[i][j] << " ";
		}
		write_fp << "\n";
	}
	write_fp.close();
}

int main(int argc, char **argv)
{
	char username[100];
	cout << "Enter your username :";
	cin >> username;
	// int gid = getGroupIdByName(username);
	// can use these for the same
	gid_t gid = getgid();
	uid_t uid = getuid();
	char *grpname = getgrgid(gid)->gr_name;
	int utype = -1;
	if(strlen(grpname) > 0){
		if(grpname[0] == 'S')  utype = 1;
		else if(grpname[0] == 'F') utype =2;
		else utype = 3;
	}  
	else {
		printf("No Associated Group Found");	
		exit(1);
	}
	vector<vector<string>> initial_data = retrieve_marks_data(utype, uid);
	// For student set the second parameter to true (for total)
	vector<vector<string>> final_data = createUI(initial_data, true);
	write_back(final_data, utype, uid);
	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0, 0); /* 0, 0 gives default characters
						   * for the vertical and horizontal
						   * lines
						   * 			*/
	wrefresh(local_win);  /* Show that box 		*/

	return local_win;
}
