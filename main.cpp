#include <iostream>
#include <ctime>
#include <limits>
#include "src/Area.h"

class AreaSweeper {
public:
    AreaSweeper(int w, int h) {
        this->width=w;
        this->height=h;
        area = new Area[width*height];
    };

    void game() {
        initArea();
        printArea();
        countNeighbours();
        printNeighbours();
        int x, y;
        char option;
        do {
            do {
                std::cout << "Enter x ";
                std::cin >> x;
            } while (!isNumericInput() || x < 0 || x >= width);
            do {
                std::cout << "Enter y ";
                std::cin >> y;
            } while (!isNumericInput() || y < 0 || y >= height);

            do {
                std::cout << "Enter option: reveal/flag/unfalg (r/f/u) ";
                std::cin >> option;
            } while (option != 'r' && option != 'f' && option != 'u');

            Area *place = area + y * width + x;
            if (option == 'r') {
                if ((place->isRevealed())) {
                    std::cout << "Already revealed'" << std::endl;
                    continue;
                } else if ((place->isMined())) {
                    std::cout << ":-(" << std::endl;
                    printEnd();
                    return;
                } else {
                    reveal(x, y);
                    printReveal();
                    continue;
                }
            }
            if (option == 'f') {
                if (place->isRevealed()) {
                    std::cout << "Already revealed'" << std::endl;
                    continue;
                } else if (place->isFlagged()) {
                    std::cout << "Already flagged'" << std::endl;
                    continue;
                } else {
                    place->setFlag();
                    ++nrOfFlags;
                    if (place->isMined()) {
                        ++foundMines;
                        if (foundMines == nrOfMines && isAllRevealed()) {
                            std::cout<<":-)"<<std::endl;
                            printEnd();
                            return;
                        }
                    }
                    printReveal();
                    continue;
                }
            }
            if (option=='u') {
                if (place->isRevealed()) {
                    std::cout << "Already revealed'" << std::endl;
                    continue;
                } else if (!place->isFlagged()) {
                    std::cout << "Not flagged'" << std::endl;
                    continue;
                } else {
                    place->removeFlag();
                    --nrOfFlags;
                    if (place->isMined()) {
                        --foundMines;
                    }
                    printReveal();
                    continue;
                }
            }
        } while (true);
    }

private:
    int width, height;
    Area* area;
    int nrOfMines = 0;
    int foundMines = 0;
    int nrOfFlags = 0;

    void initArea() {
        //TODO make sure taht at least one mine is set
        int random;
        std::srand(std::time(NULL));
        for(int i = 0; i < width*height; i++) {
            random = std::rand()%5+1;
            if (random > 4) {
                ++nrOfMines;
                (area+i)->setMine();
            }
        }
    }

    void printArea() {
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                if ((area+h*width+w)->isMined()) {
                    std::cout << '+';
                } else {
                    std::cout << '-';
                }
            }
            std::cout<<std::endl;
        }
    }

    void countNeighbours() {
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                (area+h*width+w)->setNeighbours(countNeighbourOf(w, h));
            }
        }
    }

    int countNeighbourOf(int x, int y) {
        int place = y*width+x;
        int result = 0;
        //directions
        bool n, e, s, w;
        n = e = s = w = true;
        if (y == 0) n = false;
        else if (y == height - 1) s = false;
        if (x % width == 0) w = false;
        else if ( (x+1) % width == 0) e = false;

        //n
        if (n) result += (area+place-width)->isMined();
        //ne
        if (n && e) result += (area+place-width+1)->isMined();
        //e
        if (e) result += (area+place+1)->isMined();
        //se
        if (s && e) result += (area+place+width+1)->isMined();
        //s
        if (s) result += (area+place+width)->isMined();
        //sw
        if (s && w) result += (area+place+width-1)->isMined();
        //w
        if (w) result += (area+place-1)->isMined();
        //nw
        if (n && w) result += (area+place-width-1)->isMined();

        return result;
    }

    void printNeighbours() {
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                std::cout<<(area+h*width+w)->getNeighbours()<<" ";
            }
            std::cout<<std::endl;
        }
    }

    void reveal(int x, int y) {
        Area* place = area+y*width + x;
        if ( !(place)->isRevealed() ) {
            (place)->setRevealed();
            if ( (place)->getNeighbours() == 0 ) revealNeighboursOf(x, y);
        }
    }

    void revealNeighboursOf(int x, int y) {
        Area* place = area + y*width+x;
        //directions
        bool n, e, s, w;
        n = e = s = w = true;
        if (y == 0) n = false;
        else if (y == height - 1) s = false;
        if (x % width == 0) w = false;
        else if ( (x+1) % width == 0) e = false;

        //n
        if ( n && !(place-width)->isRevealed() ) {
            reveal(x, y-1);
        }
        //ne
        if ( n && e  && !(place-width+1)->isRevealed() ) {
            reveal(x+1, y-1);
        }
        //e
        if ( e  && !(place+1)->isRevealed() ) {
            reveal(x+1, y);
        }
        //se
        if ( s && e  && !(place+width+1)->isRevealed() ) {
            reveal(x+1, y+1);
        }
        //s
        if ( s  && !(place+width)->isRevealed() ) {
            reveal(x, y+1);
        }
        //sw
        if ( s && w  && !(place+width-1)->isRevealed() ) {
            reveal(x-1, y+1);
        }
        //w
        if ( w  && !(place-1)->isRevealed() ) {
            reveal(x-1, y);
        }
        //nw
        if ( n && w  && !(place-width-1)->isRevealed() ) {
            reveal(x-1, y-1);
        }
    }

    void printReveal() {
        std::cout<<"Number of mines: "<<nrOfMines<<", number of flags: "<<nrOfFlags<<std::endl;
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                if ( (area+h*width+w)->isFlagged() ) std::cout<<'f';
                else if ( (area+h*width+w)->isRevealed() ) std::cout<<(area+h*width+w)->getNeighbours();
                else std::cout<<'u';
            }
            std::cout<<std::endl;
        }
    }

    void printEnd() {
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                if ( (area+h*width+w)->isRevealed() ) std::cout<<(area+h*width+w)->getNeighbours();
                else if ( (area+h*width+w)->isMined() ) std::cout<<'@';
                else std::cout<<'u';
            }
            std::cout<<std::endl;
        }
    }

    bool isAllRevealed() {
        for (int i = 0; i < width*height; i++) {
            if ( !(area+i)->isRevealed() && !(area+i)->isFlagged() ) return false;
        }
        return true;
    }

    bool isNumericInput() {
        if(!std::cin) {
            std::cin.clear(); // reset failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        return true;
    }
    
};


int main() {
    AreaSweeper as(4, 3);
    as.game();
    return 0;
}