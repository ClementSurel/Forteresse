#include <stdlib.h>

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
	// Case positions
	Position actualCase = {0, 0};
	Position nextCase = {0, 0};
	Position money_found = {-1, -1};
	// label
	int actualDist = 0;
	// Boolean
	int found_money = 0;


	// Label all the cases as non-visited
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			gMap[i][j]->visited = 0;
			gMap[i][j]->distance = -1;
		}
	}


	// Add Bernard's case in the caseToSee
	LIFO_addElement (toSeeNext, perso->pos);

	// Label the map until reach the target
	while (toSeeNext->nb_elms > 0 && ! found_money)
	{
		// Transfer the cases from toSeeNext to casesSeen
		while (toSeeNext->nb_elms > 0)
		{
			actualCase = LIFO_pop (toSeeNext);
			gMap[actualCase.x][actualCase.y]->visited = 1;
			LIFO_addElement (casesSeen, actualCase);
		}

		// Check the cases one by one
		while (casesSeen->nb_elms > 0 && ! found_money)
		{
			actualCase = LIFO_pop (casesSeen);

			gMap[actualCase.x][actualCase.y]->distance = actualDist;

			// Check whether the target has been found
			if ( ( (gMap[actualCase.x][actualCase.y]->item == target && perso->job != DEFENDER)
				|| ( perso->job == DEFENDER && gMap[actualCase.x][actualCase.y]->ennemy) )
				&& ! gMap[actualCase.x][actualCase.y]->targeted )
			{
				found_money = 1;
				money_found.x = actualCase.x;
				money_found.y = actualCase.y;
				break;
			}

			// Put the neighbour cases in the list toSeeNext
			for (i = actualCase.x-1; i <= actualCase.x+1; i++)
			{
				for (j = actualCase.y-1; j <= actualCase.y+1; j++)
				{
					if ( INSIDE_MAP(i,j)
						&& ! gMap[i][j]->visited
						&& (gMap[i][j]->explored || perso->job == ENNEMY)
						&& (gMap[i][j]->crossable || gMap[i][j]->item == target) )
					{
						nextCase.x = i;
						nextCase.y = j;
						if ( ! LIFO_exists(toSeeNext, nextCase) )
							LIFO_addElement (toSeeNext, nextCase);
					}
				}
			}

		} // end : Check the cases one by one

		actualDist++;

	} // end : Label the map until reach the target

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
	// VARIABLES
	int actualDist = gMap[money_found.x][money_found.y]->distance;
	Position actualCase;
	Position nextCase;
	LIFO_List* targetPath = LIFO_createList();
	int i, j;
	int nextCase_found;

	// The first case is money_found
	actualCase.x = money_found.x;
	actualCase.y = money_found.y;
	if (gMap[actualCase.x][actualCase.y]->item != HOME && perso->job != DEFENDER)
		gMap[actualCase.x][actualCase.y]->targeted = 1;
	LIFO_addElement (targetPath, actualCase);

	while (actualDist > 1)
	{
		// Look the neighbourg Cases and pick one
		for (i = 0; i < 8; i++)
		{
			switch (i)
			{
				case 0:
					nextCase.x = actualCase.x-1;
					nextCase.y = actualCase.y;
					break;
				case 1:
					nextCase.x = actualCase.x;
					nextCase.y = actualCase.y+1;
					break;
				case 2:
					nextCase.x = actualCase.x+1;
					nextCase.y = actualCase.y;
					break;
				case 3:
					nextCase.x = actualCase.x;
					nextCase.y = actualCase.y-1;
					break;
				case 4:
					nextCase.x = actualCase.x-1;
					nextCase.y = actualCase.y-1;
					break;
				case 5:
					nextCase.x = actualCase.x-1;
					nextCase.y = actualCase.y+1;
					break;
				case 6:
					nextCase.x = actualCase.x+1;
					nextCase.y = actualCase.y+1;
					break;
				case 7:
					nextCase.x = actualCase.x+1;
					nextCase.y = actualCase.y-1;
					break;
			}

			if ( INSIDE_MAP(nextCase.x, nextCase.y) && gMap[nextCase.x][nextCase.y]->distance == actualDist-1)
			{
				actualCase.x = nextCase.x;
				actualCase.y = nextCase.y;
				LIFO_addElement (targetPath, actualCase);
				actualDist--;
				break;
			}
		} // end : Look the neighbourg Cases and pick one

	}

	LIFO_freeList (perso->target);
	perso->target = targetPath;
}

void explore (GameElements *gElts, Personage* perso)
{
	// VARIABLES
	int i, j;
	Direction nextDirection;
	int hasMoved = 0; // Boolean

	// Pick up the nextDirection
	nextDirection = rand() % 8;

	// Change the Personage position depending on the nextDirection choosen
	switch (nextDirection)
	{
		case LEFT:
			if ( INSIDE_MAP(perso->pos.x-1, perso->pos.y) && gElts->gMap[perso->pos.x-1][perso->pos.y]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x-1][perso->pos.y]->explored) )
			{
				perso->pos.x--;
				hasMoved = 1;
			}
			break;
		case LEFT_UP:
			if ( INSIDE_MAP(perso->pos.x-1, perso->pos.y-1) && gElts->gMap[perso->pos.x-1][perso->pos.y-1]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x-1][perso->pos.y-1]->explored) )
			{
				perso->pos.x--;
				perso->pos.y--;
				hasMoved = 1;
			}
			break;
		case LEFT_DOWN:
			if ( INSIDE_MAP(perso->pos.x-1, perso->pos.y+1) && gElts->gMap[perso->pos.x-1][perso->pos.y+1]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x-1][perso->pos.y+1]->explored) )
			{
				perso->pos.x--;
				perso->pos.y++;
				hasMoved = 1;
			}
			break;
		case RIGHT_UP:
			if ( INSIDE_MAP(perso->pos.x+1, perso->pos.y-1) && gElts->gMap[perso->pos.x+1][perso->pos.y-1]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x+1][perso->pos.y-1]->explored) )
			{
				perso->pos.x++;
				perso->pos.y--;
				hasMoved = 1;
			}
			break;
		case RIGHT_DOWN:
			if ( INSIDE_MAP(perso->pos.x+1, perso->pos.y+1) && gElts->gMap[perso->pos.x+1][perso->pos.y+1]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x+1][perso->pos.y+1]->explored) )
			{
				perso->pos.x++;
				perso->pos.y++;
				hasMoved = 1;
			}
			break;
		case RIGHT:
			if ( INSIDE_MAP(perso->pos.x+1, perso->pos.y) && gElts->gMap[perso->pos.x+1][perso->pos.y]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x+1][perso->pos.y]->explored) )
			{
				perso->pos.x++;
				hasMoved = 1;
			}
			break;
		case UP:
			if ( INSIDE_MAP(perso->pos.x, perso->pos.y-1) && gElts->gMap[perso->pos.x][perso->pos.y-1]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x][perso->pos.y-1]->explored) )
			{
				perso->pos.y--;
				hasMoved = 1;
			}
			break;
		case DOWN:
			if ( INSIDE_MAP(perso->pos.x, perso->pos.y+1) && gElts->gMap[perso->pos.x][perso->pos.y+1]->crossable
				&& (perso->job == EXPLORER || gElts->gMap[perso->pos.x][perso->pos.y+1]->explored) )
			{
				perso->pos.y++;
				hasMoved = 1;
			}
			break;
	}

	// All the cases around the explorer get explored
	if (hasMoved && perso->job == EXPLORER)
	{
		for (i = perso->pos.x-2; i <= perso->pos.x+2; i++)
		{
			for (j = perso->pos.y-2; j <= perso->pos.y+2; j++)
			{
				if ( INSIDE_MAP(i,j) && ( ( (abs(perso->pos.x-i)) + (abs(perso->pos.y-j)) ) <= 2) )
					gElts->gMap[i][j]->explored = 1;
			}
		}
	}

	return;
}

void followPath (GameElements* gElts, Personage* perso)
{
	int goOnLeft = 0, goOnRight = 0, goOnDown = 0, goOnUp = 0;
	int moved = 0;
	int i, j;

	Position targetCase = LIFO_readElement(perso->target);

	if (targetCase.x < perso->pos.x)
		goOnLeft = 1;
	else if (targetCase.x > perso->pos.x)
		goOnRight = 1;
	if (targetCase.y < perso->pos.y)
		goOnUp = 1;
	else if (targetCase.y > perso->pos.y)
		goOnDown = 1;

	if (goOnLeft && goOnUp && gElts->gMap[perso->pos.x-1][perso->pos.y-1]->crossable)
	{
		perso->pos.x--;
		perso->pos.y--;
		moved = 1;
	}
	else if (goOnLeft && goOnDown && gElts->gMap[perso->pos.x-1][perso->pos.y+1]->crossable)
	{
		perso->pos.x--;
		perso->pos.y++;
		moved = 1;
	}
	else if (goOnRight && goOnUp && gElts->gMap[perso->pos.x+1][perso->pos.y-1]->crossable)
	{
		perso->pos.x++;
		perso->pos.y--;
		moved = 1;
	}
	else if (goOnRight && goOnDown && gElts->gMap[perso->pos.x+1][perso->pos.y+1]->crossable)
	{
		perso->pos.x++;
		perso->pos.y++;
		moved = 1;
	}
	else if (goOnLeft && gElts->gMap[perso->pos.x-1][perso->pos.y]->crossable)
	{
		perso->pos.x--;
		moved = 1;
	}
	else if (goOnUp && gElts->gMap[perso->pos.x][perso->pos.y-1]->crossable)
	{
		perso->pos.y--;
		moved = 1;
	}
	else if (goOnRight && gElts->gMap[perso->pos.x+1][perso->pos.y]->crossable)
	{
		perso->pos.x++;
		moved = 1;
	}
	else if (goOnDown && gElts->gMap[perso->pos.x][perso->pos.y+1]->crossable)
	{
		perso->pos.y++;
		moved = 1;
	}

	if ( ! moved)
		perso->state = WAITING;
	else
	{
		for (i = perso->pos.x-2; i <= perso->pos.x+2; i++)
		{
			for (j = perso->pos.y-2; j <= perso->pos.y+2; j++)
			{
				if ( INSIDE_MAP(i,j) && ( ( (abs(perso->pos.x-i)) + (abs(perso->pos.y-j)) ) <= 2) )
					gElts->gMap[i][j]->explored = 1;
			}
		}
	}
}