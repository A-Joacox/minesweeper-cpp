
#ifndef classes_h
#define classes_h
#include <iostream>
#include <string>

class Mine{
private:
    int x;
    int y;
    bool explode;
public: 
    Mine(int x, int y){
        this->x = x;
        this->y = y;
    }
    int GetX(){return x;}
    int GetY(){return y;}
};

class Board {
private:
    int len;
    int width;
    bool active;
    std::string** board;

public: 
    Board(int width = 5, int length = 5) : width(width), len(length), active(true) {
        CreateBoard(width, length);
    }

    ~Board(){
        for (int i = 0; i < width; ++i) {
            delete[] board[i];
        }
        delete[] board;
    }

    void CreateBoard(int width, int length) {
        board = new std::string*[width];
        for (int i = 0; i < width; ++i) {
            board[i] = new std::string[length];
            for (int j = 0; j < length; ++j) {
                board[i][j] = "|  |";
            }
        }
    }

    void ShowBoard() {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < len; ++j) {
                std::cout << board[i][j];
            }
            std::cout << std::endl;
        }
    }
};

#endif