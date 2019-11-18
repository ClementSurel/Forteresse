#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <ncurses.h>

#include "constants.h"
#include "game.h"
#include "animation.h"
#include "path_finder.h"

#include "LIFO.h"



void initGameElements (GameElements* gElts)
{
	// Variables
	int i, j;

	// Initiliaze the number of days
	gElts->nb_days = 0;

	// Initiliaze the map
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			gElts->gMap[i][j] = ' ';
		}
	}

	gElts->home.x = 9;
	gElts->home.y = 5;
	gElts->gMap[gElts->home.x][gElts->home.y] == '^';


	gElts->gMap[0][9] = '#';
	gElts->gMap[1][9] = '#';
	gElts->gMap[2][7] = '#';
	gElts->gMap[2][8] = '#';
	gElts->gMap[2][9] = '#';
	gElts->gMap[3][2] = '#';
	gElts->gMap[3][3] = '#';
	gElts->gMap[3][4] = '#';
	gElts->gMap[3][9] = '#';
	gElts->gMap[4][2] = '#';
	gElts->gMap[5][2] = '#';
	gElts->gMap[6][2] = '#';
	gElts->gMap[7][2] = '#';
	gElts->gMap[7][8] = '#';
	gElts->gMap[7][9] = '#';
	gElts->gMap[7][10] = '#';
	gElts->gMap[11][5] = '#';
	gElts->gMap[13][8] = '#';
	gElts->gMap[13][9] = '#';
	gElts->gMap[14][8] = '#';
	gElts->gMap[14][9] = '#';

/*
	if (MAP_WIDTH >= 10 && MAP_HEIGHT >= 10)
		gElts->gMap[9][9] = 'X';
		*/

	// Initialize the list of Personages
	gElts->nb_perso = 0;
	gElts->list_perso = NULL;
	addPersonage(gElts);

	// Initialize the list of money
	gElts->list_money = NULL;
	gElts->money_units = 0;
	gElts->money_visible = 0;

	// Initialize the amount of money
	gElts->amount_money = 500;
}

void freeGameElements (GameElements* gElts)
{
	int i;

	for (i = 0; i < gElts->nb_perso; i++)
	{
		LIFO_freeList(gElts->list_perso[i].target);
	}

	free(gElts->list_perso);
}

void addPersonage (GameElements* gElts)
{
	// Variables
	Personage* newPtr = NULL;
	int indice = 0;

	// Reallocation
	gElts->nb_perso++;
	newPtr = realloc(gElts->list_perso, gElts->nb_perso*sizeof(Personage));
	if (newPtr == NULL)
	{
		printw("The reallocation failed\n");
		refresh();
		endwin();
		free(gElts->list_perso);
		exit(EXIT_FAILURE);
	}
	gElts->list_perso = newPtr;

	// Initialize the new personage
	indice = gElts->nb_perso-1;
	gElts->list_perso[indice].pos.x = gElts->home.x;
	gElts->list_perso[indice].pos.y = gElts->home.y - 1;
	gElts->list_perso[indice].bag = NOTHING;
	gElts->list_perso[indice].state = WAITING;
	gElts->list_perso[indice].target = NULL;
}

void addMoney (GameElements* gElts)
{
	int x = rand() % MAP_WIDTH;
	int y = rand() % MAP_HEIGHT;
	if (gElts->gMap[x][y] == ' ')
	{
		gElts->gMap[x][y] = '$';
		gElts->money_units++;
		gElts->money_visible++;
	}	
}

void new_turn (GameElements* gElts)
{
	// Variables
	int i;
	int r;

	// may add money
	r = rand() % 5;
	if (r == 0)
		addMoney(gElts);

	for (i = 0; i < gElts->nb_perso; i++)
	{
		switch (gElts->list_perso[i].state)
		{
			case WAITING:
				lookingForMoney (gElts, i);
				break;
			case GETTING_MONEY:
				gettingMoney (gElts, i);
				break;
			case GOING_HOME:
				bringBackItem(gElts, i);
				break;
		}
	}
}

void lookingForMoney (GameElements* gElts, int indice)
{
	// Get the pointer on the Personage
	Personage* perso = &(gElts->list_perso[indice]);

	// Variables
	// Limits of the virtual map where to search
	int lim_x_inf = perso->pos.x;
	int lim_x_sup = perso->pos.x;
	int lim_y_inf = perso->pos.y;
	int lim_y_sup = perso->pos.y;
	// Other variables
	int i, j;
	int distance = 0;
	int found_money = 0; // Boolean
	int max_reached = 0; // Boolean

	gElts->gMap[perso->pos.x][perso->pos.y] = '*';

	if ( labelMap (gElts->gMap, perso, '$') )
	{
		gElts->money_visible--;
		perso->state = GETTING_MONEY;
		gettingMoney (gElts, indice);	
	}

}

void gettingMoney (GameElements* gElts, int indice)
{
	// Variables
	Personage *perso = &(gElts->list_perso[indice]);

	if ( reachTarget(gElts, indice) )
	{
		gElts->money_units--;
		perso->bag = MONEY;
		gElts->gMap[gElts->home.x][gElts->home.y] = '^';
		if ( ! labelMap(gElts->gMap, perso, '^'))
		{
			printw("Failed to find home\n");
			refresh();
			nodelay(stdscr, 0);
			getch();
			nodelay(stdscr, 1);
		}
		perso->state = GOING_HOME;
		//perso->target.x = gElts->home.x;
		//perso->target.y = gElts->home.y;
	}
}

void bringBackItem (GameElements* gElts, int indice)
{
	// Variables
	Personage *perso = &(gElts->list_perso[indice]);

	if (reachTarget(gElts, indice))
	{
		switch (perso->bag)
		{
			case MONEY:
				gElts->amount_money += 100;
				perso->bag = NOTHING;
				perso->state = WAITING;
				break;
		}
	}
}

int reachTarget (GameElements* gElts, int indice)
{
	Personage* perso = &(gElts->list_perso[indice]);
	Position nextCase = LIFO_getElement (perso->target);

	// Bernard is getting to the closest money (one step by calling)
	if (nextCase.x < perso->pos.x)
		perso->pos.x--; 
	else if (nextCase.x > perso->pos.x)
		perso->pos.x++;

	if (nextCase.y < perso->pos.y)
		perso->pos.y--; 
	else if (nextCase.y > perso->pos.y)
		perso->pos.y++;

	if (perso->target->nb_elms == 0)
		return 1;
	else
		return 0;
}

void readInput (GameElements *gElts)
{
	char input = getch();
	input = toupper(input);

	switch (input)
	{
		case 'N':
			addPersonage (gElts);
			break;
		case 'Q':
			gElts->nb_days = NB_DAYS;
			break;
		case 'P':
			printw("PAUSE\n");
			printw("Press a key to restart\n");
			nodelay(stdscr, 0);
			getch();
			nodelay(stdscr, 1);
			break;
	}
}
