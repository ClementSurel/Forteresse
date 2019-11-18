#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H



typedef struct
{
	int x;
	int y;	
} Position;


typedef enum {NOTHING, MONEY, WALL, HOME, PERSO} Item;

typedef struct
{
	Item item;
	int targeted;
	int distance;
} Case;

#endif // BASIC_TYPES_H