// celda.h
#ifndef CELDA_H
#define CELDA_H

class Celda {
protected:
    int x;
    int y;
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int adjacentMines = 0; // Nuevo miembro para contar minas adyacentes

public:
    Celda(int x, int y) : x(x), y(y) {}

    int GetX() const { return x; }
    int GetY() const { return y; }

    void SetPosition(int newX, int newY) { x = newX; y = newY; }
    void SetMine() {isMine = true;}
    bool IsMine() const { return isMine; }
    void Reveal() { isRevealed = true; }
    bool IsRevealed() const { return isRevealed; }
    void Flag() { isFlagged = !isFlagged; }
    void SetAdjacentMines(int count) { adjacentMines = count; } // Nuevo método para establecer minas adyacentes
    int GetAdjacentMines() const { return adjacentMines; } // Nuevo método para obtener minas adyacentes
    bool IsFlagged() const { return isFlagged; }
    std::string ToString() const {
        if (isFlagged) return "| F |";
        if (!isRevealed) return "|   |";
        if (isMine) return "| X |";
            return "| " + std::to_string(adjacentMines) + " |"; // Mostrar el número de minas adyacentes

    }
};

class Mina : public Celda {
public:
    Mina(int x, int y) : Celda(x, y) {}
};

#endif