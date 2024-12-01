// menu.h
#ifndef MENU_H
#define MENU_H

#include "person.h"
#include "celda.h"
#include "board.h"
#include "functions.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdint>
using namespace std;

class Menu {
public:
    void ShowPlayers(const string& filename) { //Mostrar los jugadores
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        } else {
            cout << "Unable to open file" << endl;
        }
    }

    void ShowBestPlayers(const string & filename){
        ifstream file(filename);
        if (file.is_open()) {
            string line; //Imprimir linea por linea del archivo
            string name; //para mostrar el nombre del jugador
            bool showPlayer = false;
            while (getline(file,line)) {
                //npos es lo que se retorna cuando .find no encuentra nada
                if (line.find("Win: Yes") != string::npos) {
                    showPlayer = true;
                    cout << name << endl;
                }
                if (showPlayer) {
                    cout << line << endl;
                if (line.find("--------------------------") != string::npos) {
                    showPlayer = false;
                    }
                }
            name = line;
            }
            file.close();
        }else{
            cout << "Unable to open file" << endl;
        }
    }

    void Run() {    //Correr el juego
        string name;
        int boardW = 8, boardL = 8;
        int mineCount = 8;
        int val;
        bool gameOver = false;

        cout << "Welcome to Minesweeper!\nSelect an option:\n";
        cout << "1. Play\n";
        cout << "2. Show all players\n";
        cout << "3. Show Best Players\n";
        cout << "4. Robot\n";
        cout << "5. Exit\n";
        cin >> val;
        switch (val) {
        case 1: {
            cout << "Enter your name: ";
            cin >> name;
            ActualPlayer player(name);
            cout << "Select Difficulty:\n";
            cout << "1. Easy (5x5)\n";
            cout << "2. Medium (8x8)\n";
            cout << "3. Hard (10x10)\n";
            cin >> val;
            switch (val) {
            case 1: {
                boardW = 5;
                boardL = 5;
                mineCount = 5;
                break;
            }
            case 2: {
                boardW = 8;
                boardL = 8;
                mineCount = 8;
                break;
            }
            case 3: {
                boardW = 10;
                boardL = 10;
                mineCount = 10;
                break;
            }
            default: {
                cout << "\nSelecting default difficulty: Medium\n";
                boardW = 8;
                boardL = 8;
                mineCount = 8;
                break;
            }
            }
            Board gameBoard(boardW, boardL);
            gameBoard.ShowBoard();
            gameBoard.AddRandomMines(mineCount);
            player.Play(gameBoard);
            break;
        }
        case 2: {
            ShowPlayers("players.txt");
            break;
        }
        case 3: {
            ShowBestPlayers("players.txt");
            break;
        }
        case 4: {
            Robot robot;
            cout << "Select Difficulty:\n";
            cout << "1. Easy (5x5)\n";
            cout << "2. Medium (8x8)\n";
            cout << "3. Hard (10x10)\n";
            cin >> val;
            switch (val) {
            case 1: {
                boardW = 5;
                boardL = 5;
                mineCount = 5;
                break;
            }
            case 2: {
                boardW = 8;
                boardL = 8;
                mineCount = 8;
                break;
            }
            case 3: {
                boardW = 10;
                boardL = 10;
                mineCount = 10;
                break;
            }
            default: {
                cout << "\nSelecting default difficulty: Medium\n";
                boardW = 8;
                boardL = 8;
                mineCount = 8;
                break;
            }
        }
            Board gameBoard(boardW, boardL);
            gameBoard.ShowBoard();
            gameBoard.AddRandomMines(mineCount);
            robot.Play(gameBoard);
            break;
        }
        case 5: {
            cout << "Goodbye!\n";
            gameOver = true;
            return;
        }
        default: {
            cout << "Invalid option. Exiting.\n";
            return;
        }
        }
    }
};

#endif