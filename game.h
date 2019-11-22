#ifndef GAME_H
#define GAME_H

#include "constants.h"

/* Manage the game elements */
int initGameElements (GameElements*);

void freeGameElements (GameElements*);

/* Add a personage */
void addPersonage (GameElements*, Job);

int killPersonage (GameElements* gElts, Personage* perso);

/* Add money */
void addMoney (GameElements*);

void new_turn (GameElements* gElts);

void lookingForMoney (GameElements*, int indice);

void gettingMoney (GameElements*, int indice);

void bringBackItem (GameElements*, int indice);

/****************************************
return a boolean:
	1 : The Personage reached the target
****************************************/
int reachTarget (GameElements*, Personage*);

void readInput (GameElements* gElts);

void ennemyAttack (GameElements* gElts, Personage* perso);

int defend (GameElements*, Personage* perso);

void defenderAttack (GameElements*, Personage*);


#endif // include GAME_H