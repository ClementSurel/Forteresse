#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <ncurses.h>

#include "constants.h"
#include "game.h"
#include "animation.h"
#include "LIFO.h"

int main (int argc, char *argv[])
{
	/* Variables */
	GameElements gElts;
	char input;
	clock_t last_t, cur_t;

	// Initialize srand
	srand(time(NULL));

	// Initialize ncruses
	initscr();
	nodelay(stdscr, 1);
	noecho();
	start_color();
	// Color definition
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);

	initGameElements (&gElts);

	last_t = clock()/(CLOCKS_PER_SEC/1000);

	while (gElts.nb_days <= NB_DAYS)
	{
		cur_t = clock()/(CLOCKS_PER_SEC/1000);
		if (cur_t - last_t >= DAY_PERIOD)
		{
			new_turn(&gElts);
			readInput(&gElts);
			printGame(&gElts);
			
			gElts.amount_money -= 5;
			gElts.nb_days++;
			last_t = cur_t;

		}
	}

	nodelay(stdscr, 0);
	printw("END OF THE GAME\n");
	printw("Press a key to quit");
	getch();

/* TEST LIFO
	printw("Test : Create a list\n");
	LIFO_List* list = LIFO_createList();
	if (list == NULL)
	{
		printw("Failed to create a list\n");
		refresh();
		getch();
		endwin();
		freeGameElements (&gElts);
		return 1;
	}

	printw("Add the element (x = 4, y = 5) in the list\n");
	Position val = {89, 12};
	LIFO_addElement (list, val);

	printw("Add the element (x = 89, y = 12) in the list\n");
	val.x = 4;
	val.y = 5;
	LIFO_addElement (list, val);

	printw("Test : check if the element (x=4, y=5) exists : \n");
	if (LIFO_exists(list, val))
		printw("The element exists :)\n");
	else
		printw("The element doesn't exists :(\n");

	printw("Test : check if the element (x = 2, y = 3) exists in the list\n");
	val.x = 2;
	val.y = 3;
	if (LIFO_exists(list, val))
		printw("The element exists :(\n");
	else
		printw("The element does not exists :)\n");

	printw("Delete the list\n");
	LIFO_freeList (list);

	refresh();
	getch();
*/

	// close ncurses
	endwin();

	freeGameElements (&gElts);

	return 0;
}

