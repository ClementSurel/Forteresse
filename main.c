#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

#include <ncurses.h>

#include "constants.h"
#include "game.h"
#include "animation.h"
#include "LIFO.h"
#include "level_editor.h"

int main (int argc, char *argv[])
{
	/* Variables */
	GameElements gElts;
	int input;
	clock_t last_t, cur_t;
	int enterGame = 0;
	int continueProg = 1;

	// Initialize srand
	srand(time(NULL));

	// Initialize ncruses
	setlocale(LC_ALL,"");
	initscr();
	noecho();
	keypad(stdscr, 1);

	// Color definition
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_WHITE);
	init_pair(7, COLOR_CYAN, COLOR_BLACK);
	init_pair(8, COLOR_BLACK, COLOR_WHITE);

	while (continueProg)
	{
		// MAIN TITLE
		while ( ! enterGame )
		{
			clear();
			move(TITLE_Y, TITLE_X);
			attron(COLOR_PAIR(4));
			printw("===  THE FORTRESS  ===");
			attroff(COLOR_PAIR(4));
			move(TITLE_Y+5, TITLE_X);
			printw("Press any key to start");
			refresh();

			input = getch();
			input = toupper(input);
			if (input == 'E')
			{
				createLevel();
			}
			else if (input == 27 || input == 'Q')
			{
				endwin();
				return 0;
			}
			else
			{
				enterGame = 1;
			}
		}

		nodelay(stdscr, 1);

		if ( ! initGameElements (&gElts) )
		{
			freeGameElements(&gElts);
			nodelay(stdscr, 0);
			printw("END OF THE GAME\n");
			printw("Press a key to quit");
			getch();
			endwin();
			return 1;
		}

		last_t = clock()/(CLOCKS_PER_SEC/1000);

		while (gElts.nb_days <= NB_DAYS)
		{
			cur_t = clock()/(CLOCKS_PER_SEC/1000);
			if (cur_t - last_t >= DAY_PERIOD)
			{
				new_turn(&gElts);
				printGame(&gElts);
				readInput(&gElts);
				
				gElts.nb_hours += 12;
				if (gElts.nb_hours >= 24)
				{
					gElts.nb_days++;
					gElts.nb_hours = 0;
				}

				last_t = cur_t;
			}
		}

		nodelay(stdscr, 0);
		printw("END OF THE GAME\n");
		printw("Press any key");
		getch();

		enterGame = 0;
	} // end while (continueProg)

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

