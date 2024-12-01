#ifndef board_h
#define board_h

#include "mines_h.h"
#include "functions.h"
#include <string>
#include <vector>
#include <set> //for pairs

class Board {
private:
    int len;
    int width;
    // mine original board
    std::string** board;
    // mine count
    std::vector<std::vector<int>> mineCount;
    // revealed cells
    std::vector<std::vector<std::string>> displayBoard; 
    // flags
    std::vector<std::vector<bool>> flagged; 
    // for flood-fill, so that there isn't infinite recursion
    std::vector<std::vector<bool>> visited; 


public: 
    Board(int width = 5, int length = 5) : width(width), len(length) {
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
        mineCount.resize(width, std::vector<int>(length, 0)); // init mine counts
        displayBoard.resize(width, std::vector<std::string>(length, "|   | ")); // init displayBoard
        flagged.resize(width, std::vector<bool>(length, false)); // init flagged cells as false
        visited.resize(width, std::vector<bool>(length, false)); // init visited cells as false
        for (int i = 0; i < width; ++i) {
            board[i] = new std::string[length];
            for (int j = 0; j < length; ++j) {
                board[i][j] = "|   | ";
            }
        }
    }

    int GetWidth() const{return width;}
    int GetLength() const{return len;}

    //TODO: fix this when resizing
    void ShowBoard(bool revealMines = false) {
        std::cout << "\n";
        for (int i = 0; i < width; i++) {
            if (i == 0) std::cout << "    " << i + 1 << "     ";
            else if (i == width - 1) std::cout << i + 1 << "  ";
            else std::cout << i + 1 << "     ";
        }
        std::cout << "\n";

        for (int i = 0; i < width; i++) {
            if (i == 0) std::cout << "  -----";
            else std::cout << "------";
        }
        std::cout << "\n";

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < len; ++j) {
                if (j == 0) std::cout << i + 1 << " ";
                if (revealMines) {
                    std::cout << board[i][j]; //solo si pierdes
                } else if (flagged[i][j]) {
                    std::cout << "| F | ";
                } else {
                    std::cout << displayBoard[i][j];
                }
            }
        std::cout << "\n";
        for (int i = 0; i < width; i++) {
            if (i == 0) std::cout << "  -----";
            else std::cout << "------";
            }
            std::cout << "\n";
        }
    }
    void AddMines(const std::vector<std::vector<int>>& minePositions) {
        for (const auto& ubi : minePositions) {
            int x = ubi[0];
            int y = ubi[1];
            if (x >= 0 && x < width && y >= 0 && y < len) {
                board[x][y] = "| X | ";
                //update neighboor cells
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if ((dx != 0 || dy != 0) && nx >= 0 && nx < width && ny >= 0 && ny < len) {
                        mineCount[nx][ny]++;
                        }
                    }
                }
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
                // if position is a duplicate, generate new random coordinates
                mine.NewPosition(generateRandomNumber(0, GetWidth() - 1), generateRandomNumber(0, GetLength() - 1));
            }
        }
    }

    void RevealCell(int x, int y) {
        // check boundaries and if already visited
        if (x < 0 || x >= width || y < 0 || y >= len || visited[x][y]) {
            return;
        }

        // mark as visited
        visited[x][y] = true;

        // reveal cell based on its minecount
        if (mineCount[x][y] > 0) {
            displayBoard[x][y] = "| " + std::to_string(mineCount[x][y]) + " | ";
            return;
        }

        // if minecount = 0, reveal empty
        displayBoard[x][y] = "| 0 | ";

        // flood-fill surrounding cells
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx != 0 || dy != 0) {
                    int nx = x + dx;
                    int ny = y + dy;
                    // reveals only if the cell hasn't been visited.
                    RevealCell(nx, ny);
                }
            }
        }
    }

    bool CheckPosition(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < len) {
            return board[x][y] == "| X | ";
            }
        return false;
        }  

    void FlagCell(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < len && displayBoard[x][y] == "|   | ") {
            flagged[x][y] = !flagged[x][y]; // toggle flag
        }
    }

    bool IsUserWin() const {
        for (int i = 0; i < width; ++i){
            for (int j = 0; j < len; ++j){
                if (board[i][j] != "| X | " && displayBoard[i][j] == "|   | ") {
                    return false;
                }
            }
        }
        return true;
    }
};

#endif