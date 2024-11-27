// board.h
#ifndef Board_H
#define Board_H

#include "celda.h"
#include "functions.h"
#include <string>
#include <vector>
#include <set>
#include <iostream>

class Board {
private:
    int len;
    int width;
    Celda*** board;
    std::vector<std::vector<int>> mineCount;

public:
    Board(int width = 5, int length = 5) : width(width), len(length) {
        CreateBoard(width, length);
    }

    ~Board() {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < len; ++j) {
                delete board[i][j];
            }
            delete[] board[i];
        }
        delete[] board;
    }

    void CreateBoard(int width, int length) {
        board = new Celda**[width];
        mineCount.resize(width, std::vector<int>(length, 0));
        for (int i = 0; i < width; ++i) {
            board[i] = new Celda*[length];
            for (int j = 0; j < length; ++j) {
                board[i][j] = new Celda(i, j);
            }
        }
    }

    int GetWidth() const { return width; }
    int GetLength() const { return len; }

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
                if (revealMines && board[i][j]->IsMine()) {
                    std::cout << "| X | ";
                } else if (revealMines) {
                    std::cout << "|   | ";
                } else if (board[i][j]->IsFlagged()) {
                    std::cout << "| F | ";
                } else {
                    std::cout << board[i][j]->ToString();
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

    void AddMines(const std::vector<Mina>& mines) {
        for (const auto& mine : mines) {
            int x = mine.GetX();
            int y = mine.GetY();
            if (x >= 0 && x < width && y >= 0 && y < len) {
                board[x][y]->SetMine();
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

        // Set the adjacent mine counts for each cell
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < len; ++j) {
                board[i][j]->SetAdjacentMines(mineCount[i][j]);
            }
        }
    }

    void CheckDuplicates(std::vector<Mina>& mines) {
        std::set<std::pair<int, int>> uniquePositions;
        for (auto& mine : mines) {
            while (true) {
                auto pos = std::make_pair(mine.GetX(), mine.GetY());
                if (uniquePositions.find(pos) == uniquePositions.end()) {
                    uniquePositions.insert(pos);
                    break;
                }
                mine.SetPosition(generateRandomNumber(0, GetWidth() - 1), generateRandomNumber(0, GetLength() - 1));
            }
        }
    }

    void RevealCell(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= len || board[x][y]->IsRevealed()) {
            return;
        }
        board[x][y]->Reveal();
        if (board[x][y]->GetAdjacentMines() > 0) {
            return;
        }
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx != 0 || dy != 0) {
                    int nx = x + dx;
                    int ny = y + dy;
                    RevealCell(nx, ny);
                }
            }
        }
    }

    bool CheckPosition(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < len) {
            return board[x][y]->IsMine();
        }
        return false;
    }

    void FlagCell(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < len && !board[x][y]->IsRevealed()) {
            board[x][y]->Flag();
        }
    }

    bool IsUserWin() const {
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < len; ++j) {
                if (!board[i][j]->IsMine() && !board[i][j]->IsRevealed()) {
                    return false;
                }
            }
        }
    return true;
    }
};

#endif