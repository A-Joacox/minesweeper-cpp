// board.h
#ifndef BOARD_H
#define BOARD_H

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
    //usamos Celda*** porque Celda** solo seria un array de punteros a Celda
    //basicamente la primera dimension Celda** es un array de punteros a filas
    //la segunda dimension Celda* es un array de punteros a celdas
    std::vector<std::vector<int>> mineCount;

public:
    Board(int width = 5, int length = 5) : width(width), len(length) {
        CreateBoard(width, length);
    }

    ~Board() {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < len; j++) {
                delete board[i][j];
            }
            delete[] board[i];
        }
        delete[] board;
    }
    
    //creamos un tablero de celdas
    //cada celda es un puntero a una clase celda
    void CreateBoard(int width, int length) {
        board = new Celda**[width];
        mineCount.resize(width, std::vector<int>(length, 0));
        for (int i = 0; i < width; i++) {
            board[i] = new Celda*[length];
            for (int j = 0; j < length; j++) {
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

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < len; j++) {
                if (j == 0) std::cout << i + 1 << " ";

                //si la celda esta revelada y es una mina (finalizo el juego)
                if (revealMines && board[i][j]->IsMine()) {
                    std::cout << "| X | ";
                
                //si la celda esta revelada y no es una mina (finalizo el juego)
                } else if (revealMines) {
                    std::cout << "|   | ";

                //si la celda es un flag
                } else if (board[i][j]->IsFlagged()) {
                    std::cout << "| F | ";

                //si la celda es revelada, ver vecinos
                } else if (board[i][j]->IsRevealed()) {
                    std::cout << board[i][j]->ToString();

                //sino no mostrar nada
                } else {
                    std::cout << "|   | ";
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
                delete board[x][y]; // eliminar la celda existente
                board[x][y] = new Mina(x, y); // reemplazar con una mina

                // incrementar el contador de minas adyacentes
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

        // seteamos las minas adyacentes de cada mina
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < len; j++) {
                board[i][j]->SetAdjacentMines(mineCount[i][j]);
            }
        }
    }

    //verificar que no hayan dos o mas minas en un mismo lugar
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

    //añadir minas al tablero
    void AddRandomMines(int mineCount) {
        std::vector<Mina> mines;
        for (int i = 0; i < mineCount; i++) {
            mines.push_back(Mina(generateRandomNumber(0, width - 1), generateRandomNumber(0, len - 1)));
        }
        CheckDuplicates(mines);
        AddMines(mines);
    }

    //revelar celda
    void RevealCell(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= len || board[x][y]->IsRevealed()) {
            return;
        }
        board[x][y]->Reveal();
        if (board[x][y]->GetAdjacentMines() > 0) {
            return;
        }
        //flood-fill, revelar celdas adyacentes
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

    //verificar si hay una mina en la posicion x, y
    bool CheckPosition(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < len) {
            return board[x][y]->IsMine();
        }
        return false;
    }

    //marcar una celda con una flag
    void FlagCell(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < len && !board[x][y]->IsRevealed()) {
            board[x][y]->Flag();
        }
    }

    //verificar si el usuario gano
    bool IsUserWin() const {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < len; j++) {
                if (!board[i][j]->IsMine() && !board[i][j]->IsRevealed()) {
                    return false;
                }
            }
        }
        return true;
    }
};

#endif