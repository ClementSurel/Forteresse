#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "basicTypes.h"
#include "LIFO.h"

// Size of the map
#define MAP_WIDTH		18
#define MAP_HEIGHT		11

// Period of a day in milliseconds
#define DAY_PERIOD		350

// Number of days per game
#define NB_DAYS 		365


typedef enum {NOTHING, MONEY} Item;
typedef enum {WAITING, GETTING_MONEY, GOING_HOME} State;
	

typedef struct
{
	Position pos;
	Item bag;
	State state;
	LIFO_List* target;
} Personage;


typedef struct
{
	int nb_days;
	char gMap[MAP_WIDTH][MAP_HEIGHT]; // map of the game

	// The position of the differents elements
	// Position of the money
	Position* list_money;
	int money_visible;
	int money_units;

	// Position of home
	Position home;

	// List of Personages
	Personage* list_perso;
	int nb_perso;

	int amount_money;

} GameElements;



#endif