#include <stdio.h>

#include <ncurses.h>

#include "constants.h"
#include "animation.h"

void printGame (GameElements* gElts)
{
	// Variables
	int i, j;
	int n;
	Item gMapCpy[MAP_WIDTH][MAP_HEIGHT];


	// Copy the game map
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			//if (gElts->gMap[i][j]->explored)
				gMapCpy[i][j] = gElts->gMap[i][j]->item;
//			else
//				gMapCpy[i][j] = INEXPLORED;
		}
	}
	// Put the personages in the map copy
	for (i = 0; i < gElts->nb_perso; i++)
	{
		gMapCpy[gElts->list_perso[i].pos.x][gElts->list_perso[i].pos.y] = PERSO;
		gElts->gMap[gElts->list_perso[i].pos.x][gElts->list_perso[i].pos.y]->explored = 1;
	}
	gMapCpy[gElts->home.x][gElts->home.y] = HOME;


	// DISPLAY
	// Refresh the screen (i.s. clear the previous screen)
	clear();

	// Print stats
	printw("Day : %d\n", gElts->nb_days);
	printw("Amount of money : %d\n", gElts->amount_money);
	printw("Number of bernards : %d\n", gElts->nb_perso);
	printw("\n");

	// PRINT THE MAP
	for (i = 0; i < 2*MAP_WIDTH+2; i++)
		printw("-");
	printw("\n");

	for (j = 0; j < MAP_HEIGHT; j++)
	{
		printw("|");
		for (i = 0; i < MAP_WIDTH; i++)
		{
			switch (gMapCpy[i][j])
			{
				case INEXPLORED:
					break;
				case WALL:
					attron(COLOR_PAIR(1));
					printw("# ");
					attroff(COLOR_PAIR(1));
					break;					
				case MONEY:
					// Switch the color pair 1 ON
					attron(COLOR_PAIR(2));
					printw("$ ");
					// Switch the color pair 1 OFF
					attroff(COLOR_PAIR(2));
					break;
				case PERSO:
					attron(COLOR_PAIR(3));
					printw("* ");
					attroff(COLOR_PAIR(3));
					break;
				case NOTHING:
					attron(COLOR_PAIR(4));
					printw("  ", gMapCpy[i][j]);
					attroff(COLOR_PAIR(4));
					break;
				case HOME:
					attron(COLOR_PAIR(1));
					printw("^ ");
					attroff(COLOR_PAIR(1));
					break;
				default:
					attron(COLOR_PAIR(1));
					printw("  ");
					attroff(COLOR_PAIR(1));
					break;
			}
		}
		printw("|\n");
	}

	for (i = 0; i < 2*MAP_WIDTH+2; i++)
		printw("-");
	printw("\n\n");

	refresh();
}

