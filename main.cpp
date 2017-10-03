#include <iostream>
#include <ctime>
#include <limits>
#include "src/Area.h"

class AreaSweeper {
public:
    AreaSweeper() {};

    void initGame() {
        char level = getLevel();
        setLevel(level);
        initArea();
        countNeighbours();
        if (getDebugMode()) {
            printArea();
            printNeighbours();
        }
    }

    char getLevel() {
        char level;
        do {
            std::cout << "Enter difficulty level beginner/intermediate/expert (b/i/e) ";
            std::cin >> level;
        } while (level != 'b' && level != 'i' && level != 'e');
        return level;
    }

    void setLevel(char l) {
        switch (l) {
            case 'b' : {
                this->width=9;
                this->height=9;
                this->nrOfMines = 10;
                area = new Area[width*height];
                break;
            };
            case 'i' : {
                this->width=16;
                this->height=16;
                this->nrOfMines = 40;
                area = new Area[width*height];
                break;
            };
            case 'e' : {
                this->width=30;
                this->height=16;
                this->nrOfMines = 99;
                area = new Area[width*height];
                break;
            };
        }
    }

    bool getDebugMode() {
        char answer;
        do {
            std::cout << "Do you want to see the table and the number of beighbours (debug mode) y/n ";
            std::cin >> answer;
        } while (answer != 'y' && answer != 'n');
        if (answer == 'n') return false;
        return true;
    }

    void run() {
        int x, y;
        char option;
        do {
            do {
                std::cout << "Enter x (between 0 and "<<width-1<<") ";
                std::cin >> x;
            } while (!isNumericInput() || x < 0 || x >= width);
            do {
                std::cout << "Enter y (between 0 and "<<height-1<<") ";
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
                    if (foundMines == nrOfMines && isAllRevealed()) {
                        std::cout<<":-)"<<std::endl;
                        printEnd();
                        return;
                    } else {
                        printReveal();
                        continue;
                    }
                }
            }
            if (option == 'f') {
                if (place->isRevealed()) {
                    std::cout << "Already revealed!" << std::endl;
                    continue;
                } else if (place->isFlagged()) {
                    std::cout << "Already flagged!" << std::endl;
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
                    std::cout << "Already revealed!" << std::endl;
                    continue;
                } else if (!place->isFlagged()) {
                    std::cout << "Not flagged!" << std::endl;
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
                    std::cout << charRepresentations::mined;
                } else {
                    std::cout << charRepresentations::free;
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

        if ( n ) reveal(x, y-1);
        if ( n && e ) reveal(x+1, y-1);
        if ( e ) reveal(x+1, y);
        if ( s && e ) reveal(x+1, y+1);
        if ( s ) reveal(x, y+1);
        if ( s && w ) reveal(x-1, y+1);
        if ( w ) reveal(x-1, y);
        if ( n && w ) reveal(x-1, y-1);
    }

    void printReveal() {
        std::cout<<"Number of mines: "<<nrOfMines<<", number of flags: "<<nrOfFlags<<std::endl;
        std::cout<<"   "; for (int i = 0; i < width; i++) { std::cout<<i%10;std::cout<<" "; } std::cout<<std::endl;
        drawBlueLine();
        for (int h = 0; h < height; h++) {
            std::cout<<h%10<<" \033[1;34m|\033[0m";
            for (int w = 0; w < width; w++) {
                if ( (area+h*width+w)->isFlagged() ) std::cout<<charRepresentations::flagged;
                else if ( (area+h*width+w)->isRevealed() ) std::cout<<charRepresentationOf((area+h*width+w)->getNeighbours());
                else std::cout<<charRepresentations::unrevealed;
                std::cout<<"\033[1;34m|\033[0m";
            }
            std::cout<<std::endl;
        }
        drawBlueLine();
    }

    void printEnd() {
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                if ( (area+h*width+w)->isRevealed() ) std::cout<<(area+h*width+w)->getNeighbours();
                else if ( (area+h*width+w)->isMined() ) std::cout<<charRepresentations::boom;
                else std::cout<<charRepresentations::unrevealed;
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
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        return true;
    }

    void drawBlueLine() {
        std::cout<<"  ";
        std::cout<<"\033[1;34m";
        for (int i = 0; i <= width*2; i++) std::cout<<"-";
        std::cout<<std::endl;
        std::cout<<"\033[0m";
    }

    std::string charRepresentationOf(int neighbours) {
        switch (neighbours) {
            case 0: return "\u0002"; //
            case 1: return "\033[34m1\033[1;0m"; //blue 1
            case 2: return "\033[32m2\033[1;0m"; //green2
            case 3: return "\033[31m3\033[1;0m"; //red3
            case 4: return "\033[33m4\033[1;0m"; //yellow4
            case 5: return "\033[36m5\033[1;0m"; //cyan5
            case 6: return "\033[35m6\033[1;0m"; //magenta6
            case 7: return "7"; //black7
            case 8: return "\033[1m\033[30m8\033[1;0m"; //bold black8
        }
    }
};


int main() {
    AreaSweeper as;
    as.initGame();
    as.run();
    return 0;
}