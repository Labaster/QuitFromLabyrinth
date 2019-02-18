//in FILE "input_labirint.txt" point of access to the labyrinth  indicate the digit  '2'
//digit '3' indicates quit from the labyrinth

#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <string.h>
#include  <iomanip>
using namespace std;


void identify_maze_size(int *col, int *row);
void create_maze(int **&maze, char **&res_maze, int col, int row);
void fill_maze(int col, int row, int *x0, int *y0, int *x1, int *y1, int **&maze, char **&res_maze);
bool find_path(int x0, int y0, int x1, int y1, int col, int row, int **&maze, char **&res_maze, int *len, int ax[], int ay[]);
void show_matr_char(char **&res_maze, int col, int row, int len, int ax[], int ay[], int x0, int y0, int x1, int y1);
void free_memory(int **&maze, char **&res_maze, int col);

int main(int argc, char *argv[]) {
	
	int row=0, col=2; 			// variables where will be stored the size of the labyrinth
	int x0=-1, y0=-1, x1=-1, y1=-1;	// coordinates of access and quit point of the labyrinth accordingly
	int len=0;					// storing a length of the passed path
	int **maze;					// pointer on 2-dimensional array(labyrinth)
	char **res_maze;			// pointer on 2-dimensional array(indicates quit from the labyrinth)
	char h;
	
	identify_maze_size(&col, &row);
	
	int ax[col*row];			// this arrays will be storing resulting 
	int ay[col*row];			// coordinates of the pathfinding
	
	create_maze(maze, res_maze, col, row);
	
	fill_maze(col, row, &x0, &y0, &x1, &y1, maze, res_maze);
	
	if(x0 == -1 || y0 == -1 || x1 == -1 || y1 == -1)									// checking if  coordinates are correct
	{ cout<<"Error! There is no available coordinates Start/Finish!!!"<<endl; 			// in "input_labirint.txt". 
	return -1;}																			// if not, the main program display error message and end work
	
	if(find_path(x0, y0, x1 , y1, col, row, maze, res_maze, &len, ax, ay) == true) 		// if function return true
	{
		do
		{
			show_matr_char(res_maze, col, row, len, ax, ay, x0, y0, x1, y1);				// calling of show_matr_char function
			cout<<endl<<"Repeat? y/n";
			do
			{
			h=getch();
			}
		while(h!='y' && h!='Y' && h!='n' && h!='N');
			if (h=='n' || h=='N')  {break;}
			if (h=='y' || h=='Y')  {system("cls");}
		}
		while(h == 'y' || h == 'Y');
	}
	
	free_memory(maze, res_maze, col);

	return 0;
}
//-----------------------------------------------------------------------------------
void identify_maze_size(int *col, int *row)	// identifying of labyrinth size function
{
	int c;
	int *ptr=&c;
	FILE *fp;
	
	fp = fopen("input_labirint.txt", "r"); 	// an opening of text File with the labyrinth
	
	while(*ptr!='\n')
	{
	fscanf(fp,"%c", ptr); 					// calculating rows in File	
	(*row)++;
	}
	*row=*row - (*row)/2;	

	while(fscanf(fp,"%c", ptr) != EOF)
	{
		if(*ptr == '\n') {(*col)++;} 		// calculating columns in File	
	}
	fclose ( fp );							// closing of File
}
//---------------------------------------------------------------------------------------
void create_maze(int **&maze, char **&res_maze, int col, int row)	// function for creating a dynamic 2-dimensional array
{
	int i;
	maze = new int* [col];
	for(i=0;i<col;i++)
	{
		maze[i] = new int[row];
	}
	//--------------------------//
	res_maze = new char* [col];
	for(i=0;i<col;i++)
	{
		res_maze[i] = new char[row];
	}
}
//---------------------------------------------------------------------------------------
void fill_maze(int col, int row, int *x0, int *y0, int *x1, int *y1, int **&maze, char **&res_maze)	// function for filling the array
{
	int i, j;
	FILE *fp;
	
	fp = fopen("input_labirint.txt", "r"); 		// an opening of text File with the labyrinth 
	
    for(i=0;i<col;i++)
	{
		for(j=0;j<row;j++)						// copying the labyrinth from File "input_labirint.txt"
		{
		fscanf(fp,"%i", &maze[i][j]);			// to array maze[col][row] 
		if(maze[i][j]==2) {(*y0)=i; (*x0)=j;}	// definition of the coordinates of access to the labyrinth
		if(maze[i][j]==3) {(*y1)=i; (*x1)=j;}	// definition of the coordinates of quit from the labyrinth
		}
	}
    fclose ( fp );								// closing of File
    
	for(i=0;i<col;i++)
	{
		for(j=0;j<row;j++)						
		{
		if (maze[i][j]==1) {maze[i][j]=-1;}										// replacing of all '1' to '-1'
		if (maze[i][j]==0 || maze[i][j]==2 || maze[i][j]==3) {maze[i][j]=-2;}	// replacing of all other digits to '-2'
		}
	}
	for(i=0;i<col;i++)
	{
		for(j=0;j<row;j++)						
		{
		res_maze[i][j] = maze[i][j];			// copying of maze to res_maze for storing beginning look of the labyrinth
		}
	}	
}
//-----------------------------------------------------------------------------------------
bool find_path(int x0, int y0, int x1, int y1, int col, int row, int **&maze, char **&res_maze, int *len, int ax[], int ay[])	// function of pathfinding
{
  	int dx[4] = {1, -1, 0, 0};   // for displacement to neighbors cells
 	int dy[4] = {0, 0, 1, -1};   // to the right, to the down, to the left, to the up
 	
	int d, x, y, k;				 // auxiliary variables
  	bool end;					
  	int ix, iy;
  	const int WALL   = -1;         // WALL
	const int EMPTY  = -2;         // free cell
	
  	if (maze[y0][x0] == WALL || maze[y1][x1] == WALL) return false;  // if cells [y0][x0] or [y1][x1] are WALLS return false
  	
  	//wave spread algorithm
  	d = 0;
  maze[y0][x0] = 0;            // writing '0' to beginning cell
  do {
    end = true;               
    for ( y = 0; y < col; y++ )
      for ( x = 0; x < row; x++ )
        if ( maze[y][x] == d )                         // cell maze[y][x] marking of digit 'd'
        {
          for ( k = 0; k < 4; k++ )                    // pass through all undefined neighbors
          {
             iy = y + dy[k]; 
			 ix = x + dx[k];
             if ( iy >= 0 && iy < col && ix >= 0 && ix < row && maze[iy][ix] == EMPTY )
             {
                end = false;              // if found undefined cells
                maze[iy][ix] = d + 1;     // increasing of step of the wave spread
             }
          }
        }
    d++;
	}	
	while ( !end && maze[y1][x1] == EMPTY );

  if (maze[y1][x1] == EMPTY) 
  {
  	cout<<endl<<"A path from starting position to quit is unreachable!";
  	return false;															// false, if path unreachable
  } 

  // retrieving of the shortest path
  
  *len = maze[y1][x1];            // a length of shortest path from [x0][y0] to (x1, y1)
  x = x1;
  y = y1;
  d = *len;
  while ( d > 0 )
  {
    ax[d] = x;					// writing an coordinate [x] to ax[d]
    ay[d] = y;                  // writing an coordinate [y] to ay[d]
    d--;
    for (k = 0; k < 4; k++)
    {
       	iy = y + dy[k];
		ix = x + dx[k];
       if ( iy >= 0 && iy < col && ix >= 0 && ix < row && maze[iy][ix] == d)
      {
          x = x + dx[k];
          y = y + dy[k];           // stepping to the cell which is closer to the beginning point on 1
          break;
      }
    }
  }
  ax[0] = x0;						// writing to the initial address of arrays
  ay[0] = y0;   					
  
  return true;
}
//---------------------------------------------------------
void show_matr_char(char **&res_maze, int col, int row, int len, int ax[], int ay[], int x0, int y0, int x1, int y1)	// A function of outputting array with a labyrinth
{
	int i, j, k=1, h, d=1;
	char c;
	for(i=0;i<col;i++)						
	{
		for(j=0;j<row;j++)
		{
		if (res_maze[i][j] == -1) {res_maze[i][j] = 'X';}	// Wall
		if (res_maze[i][j] == -2) {res_maze[i][j] = ' ';}	// Empty
		}
	}	
	
	// denoting of outputting array
	res_maze[ay[0]][ax[0]] = 'S';					// << S >> A Start point
	res_maze[ay[len]][ax[len]] = 'F';				// << F >> A Finish point
	
	cout<<"Start_position_x0="<<x0<<" Start_position_y0="<<y0<<endl<<"Finish_position_x1="<<x1<<" Finish_position_y1="<<y1<<endl;	// outputting of coordinates of Start and Finish accordingly
	cout<<"rows="<<col<<" columns="<<row<<endl;																						// outputting of a quantity of rows and columns accordingly
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);			// plugging of descriptor(using a color library)	
	SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 10 ));
	cout<<endl<<"We found the shortest path!"<<endl;
		
	SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 4 ));								                 
	cout<<endl<<"Do you want to output data  <<step by step mode>> or in <<automatic mode>>??"<<endl;
	cout<<"Enter your decision: s / a";
	
	do
	{
		c = getch();
	}
	while(c!='s' && c!='S' && c!='a' && c!='A');
	
	switch(c)
	{
		case 'a':
		case 'A':
			{
			while(k<=len)
	{
		system("cls");											// clear console screen function
		
		cout<<"Start_position_x0="<<x0<<" Start_position_y0="<<y0<<endl<<"Finish_position_x1="<<x1<<" Finish_position_y1="<<y1<<endl;	
		cout<<"rows="<<col<<" columns="<<row<<endl;																						
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);												            
		SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 9 ));
		  cout<<endl<<"Coordinates of exit"<<endl;
		  for(h=0;h<d;h++)
		  {
		  	cout<<ax[h]<<" "<<ay[h]<<"||"<<'\t';	// outputting coordinates of the shortest path
		  }
		  d++;
		  cout<<endl;
	
		for(i=0;i<col;i++)						
		{
			for(j=0;j<row;j++)
			{
				if(res_maze[i][j] == 'X')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 4 ));
					printf("%1c", res_maze[i][j]);
				}
				if(res_maze[i][j] == 'o')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 5 ));
					printf("%1c", res_maze[i][j]);
				}	
				if(res_maze[i][j] == ' ')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 15 ));
					printf("%1c", res_maze[i][j]);
				}
				if(res_maze[i][j] == 'S')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 9 ));
					printf("%1c", res_maze[i][j]);
				}	
				if(res_maze[i][j] == 'F')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 9 ));
					printf("%1c", res_maze[i][j]);
				}			
			}
			cout<<endl;
		}
		res_maze[ay[k]][ax[k]] = 'o';
		k++;
		Sleep(1000);												// time delay per 1 sec
	}
	break;	
			}
			
		case 's':
		case 'S':
			{
			while(k<=len)
	{
		system("cls");											// clear console screen function
		
		cout<<"Start_x0="<<x0<<" Start_y0="<<y0<<" Finish_x1="<<x1<<" Finish_y="<<y1<<endl;	// outputting of coordinates of Start and Finish accordingly
		cout<<"rows="<<col<<" columns="<<row<<endl;											// outputting of a quantity of rows and columns accordingly
		cout<<endl<<"We found the shortest path!"<<endl;								                 
	  
	  	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);			// plugging of descriptor(using a color library)	
		SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 9 ));
	  
		  cout<<endl<<"Coordinates of exit"<<endl;
		  for(h=0;h<d;h++)
		  {
		  	cout<<ax[h]<<" "<<ay[h]<<"||"<<'\t';	// outputting coordinates of the shortest path
		  }
		  d++;
		  cout<<endl;
	
		for(i=0;i<col;i++)						
		{
			for(j=0;j<row;j++)
			{
				if(res_maze[i][j] == 'X')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((15 << 4) | 4 ));
					printf("%1c", res_maze[i][j]);
				}
				if(res_maze[i][j] == 'o')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 5 ));
					printf("%1c", res_maze[i][j]);
				}	
				if(res_maze[i][j] == ' ')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 15 ));
					printf("%1c", res_maze[i][j]);
				}
				if(res_maze[i][j] == 'S')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 9 ));
					printf("%1c", res_maze[i][j]);
				}	
				if(res_maze[i][j] == 'F')
				{
					SetConsoleTextAttribute(hConsole, (WORD) ((7 << 4) | 9 ));
					printf("%1c", res_maze[i][j]);
				}			
			}
			cout<<endl;
		}
		res_maze[ay[k]][ax[k]] = 'o';
		k++;
		getch();												
	}	
			}
			break;
	}
for(i=1;i<len;i++)
{
		res_maze[ay[i]][ax[i]] = ' ';
}	
}
//---------------------------------------------------------
void free_memory(int **&maze, char **&res_maze, int col)			// cleaning of dynamic memory function
{
	int i;
	for(i=0;i<col;i++)
	{
		delete [] maze[i];
	}
	delete [] maze;
	//---------------------//
	for(i=0;i<col;i++)
	{
		delete [] res_maze[i];
	}
	delete [] res_maze;
}
//-----------------------------------------------------------
