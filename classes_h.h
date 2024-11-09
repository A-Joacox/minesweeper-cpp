
#ifndef classes_h
#define classes_h
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <set>

int generateRandomNumber(int min, int max) {
    std::random_device rd;  
    std::mt19937 gen(rd()); // mt engine, seed is rd
    std::uniform_int_distribution<> distr(min, max);
return distr(gen);
}

class Mine{
private:
    int x;
    int y;
    bool explode;
public: 
    Mine(int x, int y){
        this->x = x;
        this->y = y;
        explode = false;
    }

    int GetX() const {return x;}
    int GetY() const {return y;}

    void NewPosition(int newX, int newY){
        x = newX;
        y = newY;
    }
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
                board[i][j] = "|   | ";
            }
        }
    }

    int GetWidth() const{return width;}
    int GetLength() const{return len;}

    void ShowBoard() {
        std::cout << "\n-----------------------------\n";
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < len; ++j) {
                std::cout << board[i][j];
            }
            std::cout << "\n-----------------------------\n";
        }
    }
    void AddMines(const std::vector<std::vector<int>>& minePositions) {
        for (const auto& ubi : minePositions) {
            int x = ubi[0];
            int y = ubi[1];
            if (x >= 0 && x < width && y >= 0 && y < len) {
                board[x][y] = "| X | ";
            } else {
                std::cout << "invalid mine position: (" << x << ", " << y << ")\n";
            }
        }
    }
     void CheckDuplicates(std::vector<Mine>& mines) {
        std::set<std::pair<int, int>> uniquePositions;

        for (auto& mine : mines) {
            while (true) {
                auto pos = std::make_pair(mine.GetX(), mine.GetY());
                if (uniquePositions.find(pos) == uniquePositions.end()) {
                    uniquePositions.insert(pos);
                    break;
                }
                // If position is a duplicate, generate new random coordinates
                mine.NewPosition(generateRandomNumber(0, GetWidth() - 1), generateRandomNumber(0, GetLength() - 1));
            }
        }
    }
};






#endif