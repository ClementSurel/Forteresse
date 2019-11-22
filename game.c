#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <ncurses.h>

#include "constants.h"
#include "game.h"
#include "animation.h"
#include "path_finder.h"

#include "LIFO.h"



int initGameElements (GameElements* gElts)
{
	// Variables
	int i, j;
	char fileData;

	// Open a file
	FILE* file = fopen("map.lvl", "r");
	if (file == NULL)
		return 0;

	// Initiliaze the map
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			gElts->gMap[i][j] = malloc(sizeof(Case));
			if (gElts->gMap[i][j] == NULL)
				return 0;
			gElts->gMap[i][j]->item = NOTHING;
			gElts->gMap[i][j]->explored = 0;
			gElts->gMap[i][j]->crossable = 1;
		}
	}

 	// Update the map from the file .lvl
 	gElts->nb_attackPoints = 0;
 	gElts->attackPoint = NULL;
	for (j = 0; j < MAP_HEIGHT; j++)
	{
		for (i = 0; i < MAP_WIDTH; i++)
		{
			fileData = fgetc(file);
			switch (fileData)
			{
				case '^':
					gElts->home.x = i;
					gElts->home.y = j;
					gElts->gMap[i][j]->item = HOME;
					gElts->gMap[i][j]->explored = 1;	
					gElts->gMap[gElts->home.x][gElts->home.y]->crossable = 0;
					break;
				case '#':
					gElts->gMap[i][j]->item = WALL;
					gElts->gMap[i][j]->crossable = 0;
					break;
				case 'A':
					gElts->nb_attackPoints++;
					gElts->attackPoint = realloc(gElts->attackPoint, gElts->nb_attackPoints*sizeof(Position));
					if (gElts->attackPoint == NULL)
						return 0;
					gElts->attackPoint[gElts->nb_attackPoints-1].x = i;
					gElts->attackPoint[gElts->nb_attackPoints-1].y = j;
					break;
				default:
					break;
			}
		}
		fgetc(file); // read the \n 
	}

	fclose(file);

	// Initiliaze the number of days
	gElts->nb_days = 0;
	gElts->nb_hours = 0;

	// Ennmies
	gElts->nb_ennemies = 0;

	// Initialize the list of Personages and create a first Personage
	gElts->nb_perso = 0;
	gElts->list_perso = NULL;
	addPersonage(gElts, EXPLORER);

	// Initialize the amount of money
	gElts->amount_money = 500;

	gElts->beingAttacked = 0;

	return 1;
}

void freeGameElements (GameElements* gElts)
{
	int i, j;

	// Free the map
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			free(gElts->gMap[i][j]);
		}
	}

	// Free the Personage's target path (LIFO_List)
	for (i = 0; i < gElts->nb_perso; i++)
	{
		LIFO_freeList(gElts->list_perso[i].target);
	}

	free(gElts->list_perso);

	free (gElts->attackPoint);
}

void addPersonage (GameElements* gElts, Job job)
{
	// Variables
	Personage* newPtr = NULL;
	int indice = 0;
	int nAttackPoint;

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

	indice = gElts->nb_perso-1;
	gElts->list_perso[indice].index = indice;

	// Initialize the new personage
	gElts->list_perso[indice].life = 100;
	gElts->list_perso[indice].job = job;

	if (job == ENNEMY)
	{
		gElts->nb_ennemies++;
		nAttackPoint = rand() % gElts->nb_attackPoints;
		gElts->list_perso[indice].pos.x = gElts->attackPoint[nAttackPoint].x;
		gElts->list_perso[indice].pos.y = gElts->attackPoint[nAttackPoint].y;
	}
	else
	{
		gElts->list_perso[indice].pos.x = gElts->home.x;
		gElts->list_perso[indice].pos.y = gElts->home.y - 1;
	}
		
	gElts->list_perso[indice].bag = NOTHING;
	gElts->list_perso[indice].state = WAITING;
	gElts->list_perso[indice].target = NULL;
}

int killPersonage (GameElements* gElts, Personage* perso)
{
	int i;
	Personage *newPtr = NULL;

	if (perso->job == ENNEMY)
		gElts->nb_ennemies--;

	LIFO_freeList(perso->target);

	for (i = perso->index; i+1 < gElts->nb_perso; i++)
	{
		gElts->list_perso[i] = gElts->list_perso[i+1];
		gElts->list_perso[i].index--;
	}

	gElts->nb_perso--;


	newPtr = realloc(gElts->list_perso, gElts->nb_perso*sizeof(Personage));
	if (newPtr == NULL)
		return 0;

	gElts->list_perso = newPtr;

	return 1;
}

void addMoney (GameElements* gElts)
{
	int x = rand() % MAP_WIDTH;
	int y = rand() % MAP_HEIGHT;

	if (gElts->gMap[x][y]->item == NOTHING)
		gElts->gMap[x][y]->item = MONEY;
}

void new_turn (GameElements* gElts)
{
	// Variables
	int i;
	int r;
	Personage *perso = NULL;

	gElts->beingAttacked = 0;

	for (i = 0; i < gElts->nb_perso; i++)
	{
		perso = &(gElts->list_perso[i]);
		switch (perso->job)
		{
			case EXPLORER:
				switch (perso->state)
				{
					case WAITING:
						explore(gElts, perso);
						break;
					case FOLLOWING_PATH:
						followPath (gElts, perso);
						break;
				}
				break;
			case COLLECTOR:
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
				break;
			case ENNEMY:
				if (perso->life < 100)
					perso->state = STOP;
				switch (perso->state)
				{
					case WAITING:
						gElts->beingAttacked = 0;
						if (labelMap(gElts->gMap, perso, HOME))
						{
							reachTarget(gElts, perso);
							if (perso->target->nb_elms > 1)
								perso->state = REACHING_HOME;
						}
						break;
					case REACHING_HOME:
						reachTarget(gElts, perso);
						if (perso->target->nb_elms == 1)
							perso->state = ATTACKING;
						break;
					case ATTACKING:
						gElts->beingAttacked = 1;
						ennemyAttack(gElts, perso);
						break;
					case RESTING:
						perso->state = ATTACKING;
						break;
				}
				break;
			case DEFENDER:
				switch (perso->state)
				{
					case WAITING:
						if ( ! defend(gElts, perso) )
							explore(gElts, perso);
						break;
					case ATTACKING:
						perso->state = WAITING;
						break;
				}
				break;
		}
	}

	// may add money
	r = rand() % 5;
	if (r == 0)
		addMoney(gElts);

	if (gElts->nb_attackPoints != 0)
	{
		r = rand() % 20;
		if (r == 0)
			addPersonage (gElts, ENNEMY);
	}

	gElts->amount_money -= 2*gElts->nb_perso;
}

void lookingForMoney (GameElements* gElts, int indice)
{
	// Get the pointer on the Personage
	Personage* perso = &(gElts->list_perso[indice]);

	if ( labelMap (gElts->gMap, perso, MONEY) )
	{
		perso->state = GETTING_MONEY;
		gettingMoney (gElts, indice);	
	}
}

void gettingMoney (GameElements* gElts, int indice)
{
	// Variables
	Personage *perso = &(gElts->list_perso[indice]);

	if ( reachTarget(gElts, perso) )
	{
		perso->bag = MONEY;
		perso->state = GOING_HOME;
		if ( ! labelMap(gElts->gMap, perso, HOME))
		{
			printw("Failed to find home\n");
			refresh();
			nodelay(stdscr, 0);
			getch();
			nodelay(stdscr, 1);
			perso->state = WAITING;
		}
	}
}

void bringBackItem (GameElements* gElts, int indice)
{
	// Variables
	Personage *perso = &(gElts->list_perso[indice]);

	if (reachTarget(gElts, perso))
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

int reachTarget (GameElements* gElts, Personage* perso)
{
	// Variables
	Position actualPos = {perso->pos.x, perso->pos.y};
	Position nextCase = LIFO_pop (perso->target);
	if (nextCase.x == -1 && nextCase.y == -1)
	{
		printw("No path to follow\n");
		refresh();
		nodelay(stdscr, 0);
		getch();
		nodelay(stdscr, 1);
		return 1;
	}


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
	{
		gElts->gMap[perso->pos.x][perso->pos.y]->targeted = 0;
		if (gElts->gMap[perso->pos.x][perso->pos.y]->item == MONEY)
			gElts->gMap[perso->pos.x][perso->pos.y]->item = NOTHING;
		return 1;
	}
	else
		return 0;
}

void readInput (GameElements *gElts)
{
	// VARIABLES
	char input = getch();
	Job job;

	input = toupper(input);
	switch (input)
	{
		case 'N':
			job = menu_newPerso(gElts);
			addPersonage (gElts, job);
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
		case 'M':
			movePersonage(gElts);
			break;
	}
}

void ennemyAttack (GameElements* gElts, Personage* perso)
{
	// Variables
	int i, j;

	for (j = perso->pos.y-1; j <= perso->pos.y+1; j++)
	{
		for (i = perso->pos.x-1; i <= perso->pos.x+1; i++)
		{
			if ( INSIDE_MAP(i, j) && gElts->gMap[i][j]->item == HOME )
			{
				gElts->amount_money -= 10;	
			}
		}
	}

	perso->state = RESTING;

	return;
}

int defend (GameElements* gElts, Personage* perso)
{
	// VARIABLES
	int i, j;
	Personage* p;

	// Clean up the map
	for (j = 0; j < MAP_HEIGHT; j++)
	{
		for (i = 0; i < MAP_WIDTH; i++)
		{
			gElts->gMap[i][j]->ennemy = 0;
		}
	}

	// Label cases invaded by the ennemies
	for (i = 0; i < gElts->nb_perso; i++)
	{
		p = &(gElts->list_perso[i]);
		if (p->job == ENNEMY)
		{
			gElts->gMap[p->pos.x][p->pos.y]->ennemy = 1;
		}
	}

	if ( labelMap(gElts->gMap, perso, NOTHING) )
	{
		//perso->state = DEFENDING;
		if ( perso->target->nb_elms > 1)
		{
			reachTarget(gElts, perso);
		}
		else
		{
			perso->state = ATTACKING;
			defenderAttack(gElts, perso);
		}

		return 1;
	}

	return 0;
} 

void defenderAttack (GameElements* gElts, Personage* perso)
{
	// Variables
	int i, j;

	for (i = 0; i < gElts->nb_perso; i++)
	{
		if ( gElts->list_perso[i].job == ENNEMY
			&& gElts->list_perso[i].pos.x <= perso->pos.x+1 && gElts->list_perso[i].pos.x >= perso->pos.x-1
			&& gElts->list_perso[i].pos.y <= perso->pos.y+1 && gElts->list_perso[i].pos.y >= perso->pos.y-1 )
		{
			gElts->list_perso[i].life -= 20;
			if (gElts->list_perso[i].life <= 0)
			{
				killPersonage (gElts, &(gElts->list_perso[i]));
				//perso->state = WAITING;
			}
			break;
		}
	}

	return;
}


