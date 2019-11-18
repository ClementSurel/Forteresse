#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include "constants.h"

int labelMap (char gMap[MAP_WIDTH][MAP_HEIGHT], Personage* perso, char target);

void getTargetPath (char gMap[MAP_WIDTH][MAP_HEIGHT], Personage* perso, Position money_found);

#endif // PATH_FINDER_H