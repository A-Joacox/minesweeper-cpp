// celda.h
#ifndef CELDA_H
#define CELDA_H

#include <string>

using std::string;

class Celda {
protected:
    int x;
    int y;
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int adjacentMines = 0;

public:
    Celda(int x, int y) : x(x), y(y) {}
    virtual ~Celda() = default;

    int GetX() const { return x; }
    int GetY() const { return y; }

    void SetPosition(int newX, int newY) { x = newX; y = newY; }
    virtual void SetMine() { isMine = true; }
    bool IsMine() const { return isMine; }
    void Reveal() { isRevealed = true; }
    bool IsRevealed() const { return isRevealed; }
    void Flag() { isFlagged = !isFlagged; }
    void SetAdjacentMines(int count) { adjacentMines = count; }
    int GetAdjacentMines() const { return adjacentMines; }
    bool IsFlagged() const { return isFlagged; }
    virtual string ToString() const {
        if (isFlagged) return "| F |";
        if (!isRevealed) return "|   |";
        if (isMine) return "| X |";
        return "| " + std::to_string(adjacentMines) + " |";
    }
};

class Mina : public Celda {
public:
    Mina(int x, int y) : Celda(x, y) { SetMine(); }
    void SetMine() override { isMine = true; } 
    string ToString() const override {
        return "| X |";
    }
};

#endif