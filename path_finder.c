#include <ncurses.h>

#include "path_finder.h"

#include "LIFO.h"

int labelMap (char gMap[MAP_WIDTH][MAP_HEIGHT], Personage *perso, char target)
{
	// Variables
	int i, j;
	// The LIFO lists
	LIFO_List *toSee = LIFO_createList();
	LIFO_List *seen = LIFO_createList();
	LIFO_List *toSeeNext = LIFO_createList();
	// Limits of the map
	int leftIsOut = 0;
	int upIsOut = 0;
	int downIsOut = 0;
	int rightIsOut = 0;
	// Cases
	Position actualCase = {0, 0};
	Position nextCase = {0, 0};
	Position money_found = {-1, -1};
	// label
	char label = 'A';
	// Boolean
	int found_money = 0;

	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			if (gMap[i][j] >= 'A' && gMap[i][j] <= 'Z')
				gMap[i][j] = ' ';
		}
	}

	gMap[perso->pos.x][perso->pos.y] = ' ';

	// Add Bernard's case in the caseToSee
	LIFO_addElement (toSeeNext, perso->pos);

	while (toSeeNext->nb_elms > 0 && ! found_money)
	{
		while (toSeeNext->nb_elms)
		{
			actualCase = LIFO_pop (toSeeNext);
			LIFO_addElement (toSee, actualCase);
		}

		while (toSee->nb_elms > 0 && ! found_money)
		{
			actualCase = LIFO_pop (toSee);

			if (gMap[actualCase.x][actualCase.y] == ' ' || gMap[actualCase.x][actualCase.y] == '*')
			{
				gMap[actualCase.x][actualCase.y] = label;
			}
			else if (gMap[actualCase.x][actualCase.y] == target)
			{
				gMap[actualCase.x][actualCase.y] = label;
				found_money = 1;
				money_found.x = actualCase.x;
				money_found.y = actualCase.y;
				break;
			}

			LIFO_addElement (seen, actualCase);

			leftIsOut = (actualCase.x-1 >= 0) ? 0 : 1;
			rightIsOut = (actualCase.x+1 < MAP_WIDTH) ? 0 : 1;
			downIsOut = (actualCase.y+1 < MAP_HEIGHT) ? 0 : 1;
			upIsOut = (actualCase.y-1 >= 0) ? 0 : 1;

			// Put the neighbor cases in the list caseToSee
			/*
			if ( ! leftIsOut && ! upIsOut )
			{
				nextCase.x = actualCase.x - 1;
				nextCase.y = actualCase.y - 1;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			*/
			if ( ! upIsOut )
			{
				nextCase.x = actualCase.x;
				nextCase.y = actualCase.y - 1;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			/*
			if ( ! rightIsOut && ! upIsOut)
			{
				nextCase.x = actualCase.x + 1;
				nextCase.y = actualCase.y - 1;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			*/
			if ( ! rightIsOut )
			{
				nextCase.x = actualCase.x + 1;
				nextCase.y = actualCase.y;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			/*
			if ( ! rightIsOut && ! downIsOut )
			{
				nextCase.x = actualCase.x + 1;
				nextCase.y = actualCase.y + 1;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			*/
			if ( ! downIsOut )
			{
				nextCase.x = actualCase.x;
				nextCase.y = actualCase.y + 1;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			/*
			if ( ! leftIsOut && ! downIsOut )
			{
				nextCase.x = actualCase.x - 1;
				nextCase.y = actualCase.y + 1;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			*/
			if ( ! leftIsOut )
			{
				nextCase.x = actualCase.x - 1;
				nextCase.y = actualCase.y;
				if ( (gMap[nextCase.x][nextCase.y] == ' ' || gMap[nextCase.x][nextCase.y] == target)
				 && ! LIFO_exists(toSee, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
		}

		label++;
		if (label == 'T')
			label++;
		else if (label > 'Z')
			label = 'Z';
	}

	LIFO_freeList (toSee);
	LIFO_freeList (seen);
	LIFO_freeList (toSeeNext);

	if (found_money)
	{
		getTargetPath (gMap, perso, money_found);
		return 1;
	}
	else
	{
		return 0;
	}
}

void getTargetPath (char gMap[MAP_WIDTH][MAP_HEIGHT], Personage* perso, Position money_found)
{
	// Variables
	char label = gMap[money_found.x][money_found.y];
	int leftIsOut = 0;
	int upIsOut = 0;
	int downIsOut = 0;
	int rightIsOut = 0;
	Position actualCase;
	Position nextCase;
	LIFO_List* targetPath = LIFO_createList();

	// The first case is money_found
	actualCase.x = money_found.x;
	actualCase.y = money_found.y;
	gMap[actualCase.x][actualCase.y] = '%';

	int countdown = 20;

	while (label != 'A' && countdown > 0)
	{
		countdown--;
		label--;

		LIFO_addElement (targetPath, actualCase);

		leftIsOut = (actualCase.x-1 >= 0) ? 0 : 1;
		rightIsOut = (actualCase.x+1 < MAP_WIDTH) ? 0 : 1;
		downIsOut = (actualCase.y+1 < MAP_HEIGHT) ? 0 : 1;
		upIsOut = (actualCase.y-1 >= 0) ? 0 : 1;

		if ( ! leftIsOut && ! upIsOut )
		{
			nextCase.x = actualCase.x - 1;
			nextCase.y = actualCase.y - 1;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! upIsOut )
		{
			nextCase.x = actualCase.x;
			nextCase.y = actualCase.y - 1;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! rightIsOut && ! upIsOut)
		{
			nextCase.x = actualCase.x + 1;
			nextCase.y = actualCase.y - 1;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! rightIsOut )
		{
			nextCase.x = actualCase.x + 1;
			nextCase.y = actualCase.y;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! rightIsOut && ! downIsOut )
		{
			nextCase.x = actualCase.x + 1;
			nextCase.y = actualCase.y + 1;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! downIsOut )
		{
			nextCase.x = actualCase.x;
			nextCase.y = actualCase.y + 1;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! leftIsOut && ! downIsOut )
		{
			nextCase.x = actualCase.x - 1;
			nextCase.y = actualCase.y + 1;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! leftIsOut )
		{
			nextCase.x = actualCase.x - 1;
			nextCase.y = actualCase.y;
			if ( gMap[nextCase.x][nextCase.y] == label)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
	}

	LIFO_freeList (perso->target);
	perso->target = targetPath;
}
