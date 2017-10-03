//
// Created by en on 2017.10.02..
//

#ifndef AREASWEEPER_AREA_H
#define AREASWEEPER_AREA_H


namespace charRepresentations {
    static const char free = '-';
    static const char mined = '+';
    static const char flagged = 'f';
    static const char boom = '@';
    static const char unrevealed = 'u';
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
