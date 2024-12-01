// persona.h
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdint> //Para uint16_t
#include <vector>
#include <ctime>
#include <chrono> //Para medir el tiempo
#include "board.h"

using std::string;

class Player {
protected:
    string name;
    bool IsWin = false;
    unsigned int PlayedSeconds = 0;
    //uso uint16_t para el día, mes y año porque no necesito un rango tan grande (menos memoria)
    uint16_t day;
    uint16_t month;
    uint16_t year;

public:
    Player(const string& name) : name(name) {}

    string GetName() const { return name; }

    void SetWin(bool win) { IsWin = win; }

    void SetSeconds(unsigned int sec) { PlayedSeconds = sec; }

    void SetDate(uint16_t d, uint16_t m, uint16_t y) { day = d; month = m; year = y; }

    void SaveToFile(const string& filename){
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "Name: " << name << "\n";
            file << "Win: " << (IsWin ? "Yes" : "No") << "\n";
            file << "Played Seconds: " << PlayedSeconds << "\n";
            file << "Date: " << day << "/" << month << "/" << year << "\n";
            file << "--------------------------\n";
            file.close();
        } else {
            std::cerr << "Error trying to open file\n";
        }
    }
    virtual void Play(Board& gameBoard){};
};

class ActualPlayer : public Player {
public:
    ActualPlayer(const std::string& name) : Player(name) {}

    void Play(Board& gameBoard) override {
        int boardW = gameBoard.GetWidth();
        int boardL = gameBoard.GetLength();
        int x, y;
        bool gameOver = false;
        char action = '\0';

        auto start = std::chrono::high_resolution_clock::now(); // Iniciar el tiempo

        while (!gameOver) {
            std::cout << "enter row and column to reveal (e.g., '1 2') or '1 2 f' to flag/unflag: ";
            std::cin >> x >> y;
            x--;
            y--;

            if (std::cin.peek() == ' ') {
                std::cin >> action;
            } else {
                action = '\0';
            }

            if (x < 0 || x >= boardW || y < 0 || y >= boardL) {
                std::cout << "Invalid input.\n";
                std::cin.clear(); // Limpiar el buffer (cin)
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar el resto de la línea
                continue;
            }

            if (action == 'f') {
                gameBoard.FlagCell(x, y);
                std::cout << "flag at (" << x + 1 << ", " << y + 1 << ").\n";
                gameBoard.ShowBoard(false);
            } else if (gameBoard.CheckPosition(x, y)) {
                gameBoard.ShowBoard(true);
                gameOver = true;
                std::cout << "Boom! You hit a mine. Game Over.\n";
                SetWin(false);
            } else {
                gameBoard.RevealCell(x, y);
                std::cout << "Safe! Keep going.\n";
                gameBoard.ShowBoard(false);

                if (gameBoard.IsUserWin()) {
                    gameOver = true;
                    SetWin(true);
                    std::cout << "Congratulations! You have cleared all safe cells!\n";
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        unsigned int duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        SetSeconds(duration);

        time_t now = time(0);
        tm* ltm = localtime(&now);
        uint16_t day = ltm->tm_mday;
        uint16_t month = 1 + ltm->tm_mon;
        //es un formato de chrono para el año (1900 + ltm->tm_year)
        uint16_t year = 1900 + ltm->tm_year; 
        SetDate(day, month, year);

        SaveToFile("players.txt");
    }
};

class Robot : public Player {
public:
    Robot() : Player("Robot") {}

    void Play(Board& gameBoard) override {
        srand(time(0));
        int width = gameBoard.GetWidth();
        int length = gameBoard.GetLength();
        bool gameOver = false;

        auto start = std::chrono::high_resolution_clock::now();

        while (!gameOver) {
            // Generar números aleatorios para x e y es el algoritmo del robot
            int x = rand() % width;
            int y = rand() % length;

            if (!gameBoard.CheckPosition(x, y) && !gameBoard.IsUserWin()) {
                gameBoard.RevealCell(x, y);
                gameBoard.ShowBoard(false);
            } else if (gameBoard.CheckPosition(x, y)) {
                gameBoard.ShowBoard(true);
                gameOver = true;
                std::cout << "Boom! Robot hit a mine. Game Over.\n";
                SetWin(false);
            }

            if (gameBoard.IsUserWin()) {
                gameOver = true;
                SetWin(true);
                std::cout << "Robot has cleared all safe cells!\n";
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        unsigned int duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        SetSeconds(duration);

        time_t now = time(0);
        tm* ltm = localtime(&now);
        uint16_t day = ltm->tm_mday;
        uint16_t month = 1 + ltm->tm_mon;
        uint16_t year = 1900 + ltm->tm_year;
        SetDate(day, month, year);
        SaveToFile("players.txt");
    }
};

#endif