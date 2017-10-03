//
// Created by en on 2017.10.02..
//

#ifndef AREASWEEPER_AREA_H
#define AREASWEEPER_AREA_H

#include <iostream>

namespace charRepresentations {
    static const char free = '-';
    static const char mined = '+';
    static const char flagged = '!';
    static const char boom = '@';
    static const std::string unrevealed = "\033[1;7m\u0002\033[1;0m";
}


class Area {
public:
    Area();

    void setMine();
    bool isMined();
    void setNeighbours(int);
    int getNeighbours();
    void setRevealed();
    bool isRevealed();
    void setFlag();
    void removeFlag();
    bool isFlagged();

private:
    bool mined = false;
    bool revealed = false;
    int nrOfNeighbours;
    bool flagged = false;
};


#endif //AREASWEEPER_AREA_H
