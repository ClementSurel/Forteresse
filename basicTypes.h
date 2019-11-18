#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H



typedef struct
{
	int x;
	int y;	
} Position;


typedef enum {NOTHING, INEXPLORED, MONEY, WALL, HOME, PERSO} Item;

typedef struct Case
{
	// explored
	int explored;

	// Item
	Item item;
	int targeted;
	int distance;
} Case;

#endif // BASIC_TYPES_H