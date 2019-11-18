#include <stdlib.h>
#include <stdio.h>

#include "LIFO.h"

#include <ncurses.h>


LIFO_List* LIFO_createList ()
{
    LIFO_List *LIFO_list = malloc(sizeof(*LIFO_list));

    if (LIFO_list == NULL)
    {
        printw("Failed to create a new list\n");
        refresh();
        nodelay(stdscr, 0);
        getch();
        nodelay(stdscr, 1);
        return NULL;
    }

    LIFO_list->nb_elms = 0;
    LIFO_list->firstElm = NULL;

    return LIFO_list;
}

void LIFO_freeList (LIFO_List* LIFO_list)
{
    Element *actualElm = NULL;
    Element *nextElm = NULL;

    if (LIFO_list == NULL)
        return;

    if (LIFO_list->firstElm == NULL)
    {
        free(LIFO_list);
        return;
    }

    nextElm = LIFO_list->firstElm;
    free(LIFO_list);

    while (nextElm != NULL)
    {
        actualElm = nextElm;
        nextElm = nextElm->next;
        free (actualElm);
    }
}

void LIFO_addElement (LIFO_List* LIFO_list, Position val)
{
    Element *newElm = malloc(sizeof(*newElm));

    if (newElm == NULL)
    {
        printw("An error occured during memory allocation for the new element\n");
        refresh();
        nodelay(stdscr, 0);
        getch();
        nodelay(stdscr, 1);
        return;
    }

    if (LIFO_list == NULL)
    {
        printw("In function LIFO_addElement : No list\n");
        refresh();
        nodelay(stdscr, 0);
        getch();
        nodelay(stdscr, 1);
        return;
    }

    newElm->value.x = val.x;
    newElm->value.y = val.y;

    newElm->next = LIFO_list->firstElm;
    LIFO_list->firstElm = newElm;
    LIFO_list->nb_elms++;
}

Position LIFO_readElement (LIFO_List* LIFO_list)
{
    Position element_value = {-1, -1};

    if (LIFO_list == NULL)
        return element_value;

    if (LIFO_list->firstElm == NULL)
        return element_value;

    element_value.x = LIFO_list->firstElm->value.x;
    element_value.y = LIFO_list->firstElm->value.y;

    return element_value;
}       

Position LIFO_pop (LIFO_List* LIFO_list)
{
    Position val = {-1, -1};
    Element *actualElm = NULL;

    if (LIFO_list == NULL)
    {
        printw("In function LIFO_getElm : No list\n");
        refresh();
        nodelay(stdscr, 0);
        getch();
        nodelay(stdscr, 1);
        return val;
    }

    if (LIFO_list->firstElm == NULL)
    {
        printw("In function LIFO_getElement : None element found in the list\n");
        refresh();
        nodelay(stdscr, 0);
        getch();
        nodelay(stdscr, 1);
        return val;
    }

    actualElm = LIFO_list->firstElm;

    // Take the element's value
    val.x = actualElm->value.x;
    val.y = actualElm->value.y;

    // Delete the element
    LIFO_list->firstElm = actualElm->next;
    LIFO_list->nb_elms--;
    free(actualElm);

    return val;
}

int LIFO_exists (LIFO_List* LIFO_list, Position val)
{
    Element *actualElm = NULL;

    if (LIFO_list == NULL)
        return 0;

    if (LIFO_list->firstElm == NULL)
        return 0;

    actualElm = LIFO_list->firstElm;

    while (actualElm != NULL)
    {
        if (actualElm->value.x == val.x && actualElm->value.y == val.y)
            return 1;
        actualElm = actualElm->next;
    }

    return 0;
}
