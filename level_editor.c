#include <stdio.h>
#include <ctype.h>

#include <ncurses.h>

#include "constants.h"

#include "level_editor.h"



void createLevel ()
{
	// VARIABLES
	char eMap[MAP_WIDTH][MAP_HEIGHT];
	int input;
	int i, j;
	int row, col;
	int continue_editing = 1; //Boolean

	clear();

	// Make an empty map
	for (j = 0; j < MAP_HEIGHT; j++)
	{
		for (i = 0; i < MAP_WIDTH; i++)
		{
			eMap[i][j] = ' ';
		}
	}

	printw("\t\t\t=== LEVEL EDITOR ===\n\n");

	printw("+");
	for (i = 0; i < 2*MAP_WIDTH; i++)
		printw("-");
	printw("+\n");

	for (j = 0; j < MAP_HEIGHT; j++)
	{
		printw("|");
		for (i = 0; i < MAP_WIDTH; i++)
		{
			printw("  ");
		}
		printw("|\n");
	}

	printw("+");
	for (i = 0; i < 2*MAP_WIDTH; i++)
		printw("-");
	printw("+");

	printw("\n\nW : Wall\t");
	printw("A : entry point for the ennemies (only on the edge)\n");
	printw("S: Save level\n");

	// print the home
	row = 3+MAP_HEIGHT/2;
	col = 1+2*MAP_WIDTH/2;
	move(row, col);
	printw("^");
	eMap[MAP_WIDTH/2][MAP_HEIGHT/2] = '^';

	row = 3;
	col = 1;
	move(row, col);

	refresh();

	while (continue_editing)
	{
		input = getch();
		input = toupper(input);
		switch (input)
		{
			case KEY_RIGHT:
				if (col+2 < 1+2*MAP_WIDTH)
				{
					col += 2;
					move (row, col);
				}
				break;
			case KEY_LEFT:
				if (col-2 >= 0)
				{
					col -= 2;
					move (row, col);
				}
				break;
			case KEY_UP:
				if (row-1 >= 3)
				{
					row--;
					move (row, col);
				}
				break;
			case KEY_DOWN:
				if (row+1 < 3+MAP_HEIGHT)
				{
					row++;
					move (row, col);
				}
				break;
			case 'Q':
			case 27:
				continue_editing = 0;
				break;
			case 'W':
				if ( (row != 3+MAP_HEIGHT/2 && row != 3+MAP_HEIGHT/2-1) || col != 1+2*MAP_WIDTH/2)
				{
					printw("# ");
					eMap[(col-1)/2][row-3] = '#';
					col += 2;
				}
				if (col >= 1+2*MAP_WIDTH)
				{
					col -= 2;
					move (row, col);
				}
				break;
			case 'A':
				if ( row == 3 || row == 3+MAP_HEIGHT-1 || col == 1 || col == 1+2*MAP_WIDTH-2 )
				{
					printw("A ");
					eMap[(col-1)/2][row-3] = 'A';
					col += 2;
				}
				if (col >= 1+2*MAP_WIDTH)
				{
					col -= 2;
					move (row, col);
				}				
				break;
			case 'S':
				if ( saveLevel(eMap) )
				{
					move (3+MAP_HEIGHT+6, 0);
					printw("Level Saved!\n");
					move(row, col);
				}
				break;
			default:
				break;
		}
		refresh();
	}

	//return;
}


int saveLevel (char eMap[MAP_WIDTH][MAP_HEIGHT])
{
	// VARIABLES	
	int i, j;

	FILE *file = fopen("map.lvl", "w");
	if (file == NULL)
		return 0;

	for (j = 0; j < MAP_HEIGHT; j++)
	{
		for (i = 0; i < MAP_WIDTH; i++)
		{
			fputc(eMap[i][j], file);
		}
		fputc('\n', file);
	}

	fclose(file);

	return 1;
}

