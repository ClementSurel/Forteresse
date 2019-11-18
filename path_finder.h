#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include "constants.h"

int labelMap (Case* gMap[MAP_WIDTH][MAP_HEIGHT], Personage* perso, Item target);

void getTargetPath (Case* gMap[MAP_WIDTH][MAP_HEIGHT], Personage* perso, Position money_found);

#endif // PATH_FINDER_H