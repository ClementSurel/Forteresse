#include <stdio.h>

#include <ncurses.h>

#include "constants.h"
#include "animation.h"

void printGame (GameElements* gElts)
{
	// Variables
	int i, j;
	int n;
	char gMapCpy[MAP_WIDTH][MAP_HEIGHT];

	// Refresh the screen (i.s. clear the previous screen)
	clear();

	// Print stats
	printw("Day : %d\n", gElts->nb_days);
	printw("Amount of money : %d\n", gElts->amount_money);
	printw("Number of bernards : %d\n", gElts->nb_perso);
	printw("\n");

	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			gMapCpy[i][j] = gElts->gMap[i][j];
		}
	}

	for (i = 0; i < gElts->nb_perso; i++)
	{
		gMapCpy[gElts->list_perso[i].pos.x][gElts->list_perso[i].pos.y] = '*';
	}

	gMapCpy[gElts->home.x][gElts->home.y] = '^';



	for (i = 0; i < MAP_WIDTH+2; i++)
		printw("-");
	printw("\n");


	for (j = 0; j < MAP_HEIGHT; j++)
	{
		printw("|");
		for (i = 0; i < MAP_WIDTH; i++)
		{
			switch (gMapCpy[i][j])
			{
				case '%':
				case '$':
					// Switch the color pair 1 ON
					attron(COLOR_PAIR(2));
					printw("$");
					// Switch the color pair 1 OFF
					attroff(COLOR_PAIR(2));
					break;
				case '*':
					attron(COLOR_PAIR(3));
					printw("*");
					attroff(COLOR_PAIR(3));
					break;
				case 'A':
				case 'C':
				case 'E':
				case 'G':
				case 'I':
				case 'K':
				case 'M':
				case 'O':
				case 'Q':
				case 'S':
				case 'U':
				case 'W':
				case 'Y':
				case 'B':
				case 'D':
				case 'F':
				case 'H':
				case 'J':
				case 'L':
				case 'N':
				case 'P':
				case 'R':
				case 'V':
				case 'X':
				case 'Z':
				case '.':
					gElts->gMap[i][j] = ' ';
					// Switch the color pair 1 ON
					attron(COLOR_PAIR(4));
					printw(" ", gMapCpy[i][j]);
					// Switch the color pair 1 OFF
					attroff(COLOR_PAIR(4));
					break;
				default:
					attron(COLOR_PAIR(1));
					printw("%c", gMapCpy[i][j]);
					attroff(COLOR_PAIR(1));
					break;
			}
		}
		printw("|\n");
	}

	for (i = 0; i < MAP_WIDTH+2; i++)
		printw("-");
	printw("\n\n");

	refresh();
}

