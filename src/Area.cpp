//
// Created by en on 2017.10.02..
//

#include <regex.h>
#include "Area.h"


Area::Area() {}

void Area::setMine() { mined = true; }
bool Area::isMined() { return mined; }
void Area::setNeighbours(int n) { nrOfNeighbours = n; };
int Area::getNeighbours() { return nrOfNeighbours; };
void Area::setRevealed() {revealed = true; };
bool Area::isRevealed() { return revealed; };


