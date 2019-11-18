#ifndef LIFO_H_INCLUDED
#define LIFO_H_INCLUDED

typedef struct Element Element;
typedef struct LIFO_List LIFO_List;

#include "basicTypes.h"

struct Element
{
    Position value;

    Element *next;
};

struct LIFO_List
{
	int nb_elms;

    Element* firstElm;
};


LIFO_List* LIFO_createList ();

void LIFO_freeList (LIFO_List* LIFO_list);

void LIFO_addElement (LIFO_List* LIFO_list, Position val);

Position LIFO_readElement (LIFO_List* LIFO_list);

Position LIFO_pop (LIFO_List* LIFO_list);

/* Check whether or not a case is already in the list.
	Return a boolean */
int LIFO_exists (LIFO_List* LIFO_list, Position val);

#endif // LIFO_H_INCLUDED
