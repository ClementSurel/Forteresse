#include <ncurses.h>

#include "path_finder.h"

#include "LIFO.h"

int labelMap (Case* gMap[MAP_WIDTH][MAP_HEIGHT], Personage *perso, Item target)
{
	// VARIABLES
	int i, j;
	// The LIFO lists
	LIFO_List *casesSeen = LIFO_createList();
	LIFO_List *toSeeNext = LIFO_createList();
	// Limits of the map
	int leftIsOut = 0;
	int upIsOut = 0;
	int downIsOut = 0;
	int rightIsOut = 0;
	// Case positions
	Position actualCase = {0, 0};
	Position nextCase = {0, 0};
	Position money_found = {-1, -1};
	// label
	int actualDist = 0;
	// Boolean
	int found_money = 0;

	// Erase the map
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			gMap[i][j]->distance = -1;
		}
	}

	// Add Bernard's case in the caseToSee
	LIFO_addElement (toSeeNext, perso->pos);

	while (toSeeNext->nb_elms > 0 && ! found_money)
	{
		while (toSeeNext->nb_elms > 0)
		{
			actualCase = LIFO_pop (toSeeNext);
			LIFO_addElement (casesSeen, actualCase);
		}

		while (casesSeen->nb_elms > 0 && ! found_money)
		{
			actualCase = LIFO_pop (casesSeen);

			if (gMap[actualCase.x][actualCase.y]->item == NOTHING && gMap[actualCase.x][actualCase.y]->distance == -1)
			{
				gMap[actualCase.x][actualCase.y]->distance = actualDist;
			}
			else if (gMap[actualCase.x][actualCase.y]->item == target && gMap[actualCase.x][actualCase.y]->targeted == 0)
			{
				gMap[actualCase.x][actualCase.y]->distance = actualDist;
				found_money = 1;
				money_found.x = actualCase.x;
				money_found.y = actualCase.y;
				break;
			}

			leftIsOut = (actualCase.x-1 >= 0) ? 0 : 1;
			rightIsOut = (actualCase.x+1 < MAP_WIDTH) ? 0 : 1;
			downIsOut = (actualCase.y+1 < MAP_HEIGHT) ? 0 : 1;
			upIsOut = (actualCase.y-1 >= 0) ? 0 : 1;

			// Put the neighbor cases in the list caseToSee
			if ( ! leftIsOut && ! upIsOut )
			{
				nextCase.x = actualCase.x - 1;
				nextCase.y = actualCase.y - 1;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			if ( ! rightIsOut && ! upIsOut)
			{
				nextCase.x = actualCase.x + 1;
				nextCase.y = actualCase.y - 1;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			if ( ! rightIsOut && ! downIsOut )
			{
				nextCase.x = actualCase.x + 1;
				nextCase.y = actualCase.y + 1;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			if ( ! leftIsOut && ! downIsOut )
			{
				nextCase.x = actualCase.x - 1;
				nextCase.y = actualCase.y + 1;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			if ( ! upIsOut )
			{
				nextCase.x = actualCase.x;
				nextCase.y = actualCase.y - 1;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			if ( ! rightIsOut )
			{
				nextCase.x = actualCase.x + 1;
				nextCase.y = actualCase.y;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			if ( ! downIsOut )
			{
				nextCase.x = actualCase.x;
				nextCase.y = actualCase.y + 1;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
			if ( ! leftIsOut )
			{
				nextCase.x = actualCase.x - 1;
				nextCase.y = actualCase.y;
				if ( (gMap[nextCase.x][nextCase.y]->item == NOTHING || gMap[nextCase.x][nextCase.y]->item == target) && gMap[nextCase.x][nextCase.y]->distance == -1
					&& ! LIFO_exists(casesSeen, nextCase) && ! LIFO_exists(toSeeNext, nextCase) )
					LIFO_addElement (toSeeNext, nextCase);
			}
		}

		actualDist++;
	}

	LIFO_freeList (casesSeen);
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

void getTargetPath (Case* gMap[MAP_WIDTH][MAP_HEIGHT], Personage* perso, Position money_found)
{
	// Variables
	int actualDist = gMap[money_found.x][money_found.y]->distance;
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
	if (gMap[actualCase.x][actualCase.y]->item != HOME)
		gMap[actualCase.x][actualCase.y]->targeted = 1;

	while (actualDist > 0)
	{
		actualDist--;

		LIFO_addElement (targetPath, actualCase);

		leftIsOut = (actualCase.x-1 >= 0) ? 0 : 1;
		rightIsOut = (actualCase.x+1 < MAP_WIDTH) ? 0 : 1;
		downIsOut = (actualCase.y+1 < MAP_HEIGHT) ? 0 : 1;
		upIsOut = (actualCase.y-1 >= 0) ? 0 : 1;

		if ( ! upIsOut )
		{
			nextCase.x = actualCase.x;
			nextCase.y = actualCase.y - 1;
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist)
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
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist)
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
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist)
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
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist )
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				continue;
			} 
		}
		if ( ! leftIsOut && ! upIsOut )
		{
			nextCase.x = actualCase.x - 1;
			nextCase.y = actualCase.y - 1;
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist )
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
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist )
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
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist )
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
			if ( gMap[nextCase.x][nextCase.y]->distance == actualDist )
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
