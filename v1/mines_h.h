#ifndef mines_h
#define mines_h

#include <string>
#include <vector>
#include <set> //for pairs

class Mine{
private:
    int x;
    int y;
public: 
    Mine(int x, int y){
        this->x = x;
        this->y = y;
    }

    int GetX() const {return x;}
    int GetY() const {return y;}

    void NewPosition(int newX, int newY){
        x = newX;
        y = newY;
    }
};

#endif