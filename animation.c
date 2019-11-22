#include <ncurses.h>
#include <ctype.h>

#include "constants.h"
#include "animation.h"



void printGame (GameElements* gElts)
{
	// Variables
	int i, j;
	int n;
	int being_attacked = 0;
	Personage *perso;

	// DISPLAY
	// Refresh the screen (i.s. clear the previous screen)
	clear();

	// Print stats
	printw("Day : %d\n", gElts->nb_days);
	printw("Amount of money : %d\n", gElts->amount_money);
	printw("Number of bernards : %d\n", gElts->nb_perso - gElts->nb_ennemies);
	printw("\n");
	printw("n : create a new bernard\n");
	printw("m : move an explorer\n");
	printw("p : pause\t");	printw("q : Quit\n");

	if (gElts->beingAttacked)
	{
		attron(COLOR_PAIR(4));
		printw("\nTHE HOME IS UNDER ATTACK !!!\n");
		attroff(COLOR_PAIR(4));
	}
	else
	{
		printw("\n\n");
	}

	// PRINT THE MAP
	for (i = 0; i < 2*MAP_WIDTH+2; i++)
		printw("-");
	printw("\n");

	for (j = 0; j < MAP_HEIGHT; j++)
	{
		printw("|");
		for (i = 0; i < MAP_WIDTH; i++)
		{
			if ( ! gElts->gMap[i][j]->explored )
			{
				attron(COLOR_PAIR(8));
				printw("  ");
				attroff(COLOR_PAIR(8));
			}
			else
			{
				switch (gElts->gMap[i][j]->item)
				{
					case WALL:
						attron(COLOR_PAIR(2));
						printw("# ");
						attroff(COLOR_PAIR(2));
						break;					
					case NOTHING:
						printw("  ");
						break;					
					case MONEY:
						attron(COLOR_PAIR(1));
						printw("$ ");
						attroff(COLOR_PAIR(1));
						break;
					default:
						printw("? ");
						break;
				}
			}
		}
		printw("|\n");
	}

	for (i = 0; i < 2*MAP_WIDTH+2; i++)
		printw("-");
	printw("\n\n");

	// Print the Personages
	for (j = 0; j < gElts->nb_perso; j++)
	{
		if (gElts->nb_hours%24 == 0)
			i = gElts->nb_perso-1 - j;
		else
			i = j;

		perso = &(gElts->list_perso[i]);
		if (gElts->gMap[perso->pos.x][perso->pos.y]->explored)
		{
			move(10+perso->pos.y, 1+2*perso->pos.x);
			switch (perso->job)
			{
				case EXPLORER:
					attron(COLOR_PAIR(3));
					printw("*e");
					attroff(COLOR_PAIR(3));
					break;
				case COLLECTOR:
					attron(COLOR_PAIR(5));
					printw("*c");
					attroff(COLOR_PAIR(5));
					break;
				case ENNEMY:
					if (perso->state == ATTACKING)
					{
						attron(COLOR_PAIR(4));
						printw("-*");
						attroff(COLOR_PAIR(4));	
					}
					else
					{
						attron(COLOR_PAIR(4));
						printw(" *");
						attroff(COLOR_PAIR(4));					
					}
					break;
				case DEFENDER:
					attron(COLOR_PAIR(7));
					if (perso->state == ATTACKING)
						printw("*-");
					else
						printw("*d");
					attroff(COLOR_PAIR(7));	
					break;
			}
		}
	}

	// Print the home
	move(10+gElts->home.y, 1+2*gElts->home.x);
	printw("^ ");

	move(10+MAP_HEIGHT+3, 0);

	refresh();
}

Job menu_newPerso ()
{
	// VARIABLES
	Job job;
	char saisie = 'a';
	int choiceMade = 0;

	nodelay(stdscr, 0);

	while ( !choiceMade )
	{
		// Print message
		move (4, 0);
		printw("Which kind of bernard do you want to create ?\n");
		printw("e : explorer\n");
		printw("c : collector\n");
		printw("d : defender\n");
		move(10+MAP_HEIGHT +2, 0);
		saisie = getch();
		saisie = toupper(saisie);
		switch (saisie)
		{
			case 'E':
				job = EXPLORER;
				choiceMade = 1;
				break;
			case 'C':
				job = COLLECTOR;
				choiceMade = 1;
				break;
			case 'D':
				job = DEFENDER;
				choiceMade = 1;
			default:
				break;
		}
	}

	nodelay(stdscr, 1);

	return job;
}

void movePersonage (GameElements* gElts)
{
	// VARIABLES
	int i, j;
	int input;
	int row, col;
	Position targetPos;

	nodelay(stdscr, 0);

	LIFO_List* targetCase = NULL;

	for (i = 0; i < gElts->nb_perso; i++)
	{
		if (gElts->list_perso[i].job == EXPLORER)
		{
			move(4, 0);
			printw("e : Select an explorer\n");
			printw("\n");
			printw("q : return to game\n");
			row = 10+gElts->list_perso[i].pos.y;
			col = 1+2*gElts->list_perso[i].pos.x;
			move(row, col);
			refresh();
			input = getch();
			input = toupper(input);
			switch (input)
			{
				case 'E':
					targetCase = LIFO_createList();
					targetPos = selectACase();
					if (targetPos.x != -1 && ! gElts->gMap[targetPos.x][targetPos.y]->explored)
					{
						LIFO_addElement(targetCase, targetPos);
						gElts->list_perso[i].target = targetCase;
						gElts->list_perso[i].state = FOLLOWING_PATH;
					}
					else if (targetPos.x != -1 && gElts->gMap[targetPos.x][targetPos.y]->explored)
					{
						LIFO_addElement(targetCase, targetPos);
						gElts->list_perso[i].target = targetCase;
						gElts->list_perso[i].state = FOLLOWING_PATH;
					}
					break;
				case KEY_LEFT:
					for (j = i-1; j >= 0; j--)
					{
						if (j >= 0 && gElts->list_perso[j].job == EXPLORER)
						{
							i = j-1;
							break;
						}
						else if (j == -1)
						{
							j = gElts->nb_perso;
						}
					}
					break;
				case 'Q':
					i = gElts->nb_perso;
					break;
				case KEY_RIGHT:
					break;
				default:
					i--;
					break;
			}

		}
		if (i == gElts->nb_perso-1)
			i = -1;
	}

	nodelay(stdscr, 1);

	move(10+MAP_HEIGHT+3, 0);
}

Position selectACase ()
{
	Position selectedCase = {-1, -1};
	int input = 'a';
	int row, col;

	getyx(stdscr, row, col);

	move(4, 0);
	printw("t : target the case\n");
	printw("\n");
	printw("q : return to previous menu\n");

	move(row, col);

	while (input != 'Q' && input != 'T')
	{
		input = getch();
		input = toupper(input);
		switch (input)
		{
			case 'T':
				selectedCase.x = col/2-1;
				selectedCase.y = row-10;
				break;
			case KEY_RIGHT:
				if (col+2 < 1+2*MAP_WIDTH)
				{
					col += 2;
					move (row, col);
				}
				break;
			case KEY_LEFT:
				if (col-2 >= 0)
				{
					col -= 2;
					move (row, col);
				}
				break;
			case KEY_UP:
				if (row-1 >= 10)
				{
					row--;
					move (row, col);
				}
				break;
			case KEY_DOWN:
				if (row+1 < 10+MAP_HEIGHT)
				{
					row++;
					move (row, col);
				}
				break;
		}
	}

	return selectedCase;
}
