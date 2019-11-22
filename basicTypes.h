#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

// Position
typedef struct
{
	int x;
	int y;	
} Position;

// Direction
typedef enum {LEFT, LEFT_UP, LEFT_DOWN, RIGHT, RIGHT_UP, RIGHT_DOWN, UP, DOWN} Direction; 

// Item
typedef enum {NOTHING, MONEY, WALL, HOME} Item;


#endif // BASIC_TYPES_H