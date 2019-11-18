#ifndef GAME_H
#define GAME_H

#include "constants.h"

/* Manage the game elements */
void initGameElements (GameElements*);

void freeGameElements (GameElements*);

/* Manage the list of the Personage */
void addPersonage (GameElements*);

/* Manage the list of money */
void addMoney (GameElements*);




void new_turn (GameElements* gElts);

void lookingForMoney (GameElements*, int indice);

void gettingMoney (GameElements*, int indice);

void bringBackItem (GameElements*, int indice);

/*
return a boolean:
	1 : The Personage reached the target
*/
int reachTarget (GameElements*, int indice);

void readInput (GameElements* gElts);

#endif // include GAME_H