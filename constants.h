#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "basicTypes.h"
#include "LIFO.h"

// Size of the map
#define MAP_WIDTH			40
#define MAP_HEIGHT			20

// Period of a day in milliseconds
#define DAY_PERIOD			350

// Number of days per game
#define NB_DAYS 			365

#define TITLE_X				25
#define TITLE_Y				15

#define INSIDE_MAP(x,y)		(x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)


// Job
typedef enum {EXPLORER, COLLECTOR, ENNEMY, DEFENDER} Job;	

// State
typedef enum {WAITING, GETTING_MONEY, GOING_HOME, ATTACKING, STOP, RESTING, REACHING_HOME, DEFENDING, FOLLOWING_PATH} State;

// Personage
typedef struct
{
	int life;
	Job job;
	Position pos;
	Item bag;
	State state;
	LIFO_List* target;

	int index;
} Personage;

// Case
typedef struct Case
{
	// Booleans
	int explored;
	int crossable;

	// Item
	Item item;
	int targeted;

	// Dijkstra
	int visited;
	int distance;

	// Ennemy
	int ennemy; // Boolean
} Case;


// Game Elements
typedef struct
{
	// Timing
	int nb_hours;
	int nb_days;

	// New map
	Case* gMap[MAP_WIDTH][MAP_HEIGHT];

	// Position of home
	Position home;

	// List of Personages
	Personage* list_perso;
	int nb_perso;

	// Ennmies
	Position* attackPoint;
	int nb_attackPoints;
	int nb_ennemies;

	// amount of money
	int amount_money;

	int beingAttacked;
} GameElements;



#endif