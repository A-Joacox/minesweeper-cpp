#include "person.h"
#include "celda.h"
#include "tablero.h"
#include "functions.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdint>
using namespace std;

void ShowPlayers(const string& filename) {
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

int main() {
    string name;
    int boardW = 8, boardL = 8;
    int mineCount = 8;
    int val;
    int x, y;
    bool gameOver = false;
    char action = '\0';

    cout << "Welcome to Minesweeper!\nSelect an option:\n";
    cout << "1. Play\n";
    cout << "2. Show players\n";
    cout << "3. Robot\n";
    cout << "4. Exit\n";
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

        Tablero gameBoard(boardW, boardL);
        gameBoard.ShowBoard();

        vector<Mina> mines;
        for (int i = 0; i < mineCount; i++) {
            mines.push_back(Mina(generateRandomNumber(0, boardW - 1), generateRandomNumber(0, boardL - 1)));
        }

        gameBoard.CheckDuplicates(mines);
        gameBoard.AddMines(mines);

        auto start = chrono::high_resolution_clock::now();

        while (!gameOver) {
            cout << "enter row and column to reveal (e.g., '1 2') or '1 2 f' to flag/unflag: ";
            cin >> x >> y;
            x--;
            y--;

            if (cin.peek() == ' ') {
                cin >> action;
            } else {
                action = '\0';
            }

            if (x < 0 || x >= boardW || y < 0 || y >= boardL) {
                cout << "Invalid input.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (action == 'f') {
                gameBoard.FlagCell(x, y);
                cout << "flag at (" << x + 1 << ", " << y + 1 << ").\n";
                gameBoard.ShowBoard(false);
            } else if (gameBoard.CheckPosition(x, y)) {
                gameBoard.ShowBoard(true);
                gameOver = true;
                cout << "Boom! You hit a mine. Game Over.\n";
                player.SetWin(false);
            } else {
                gameBoard.RevealCell(x, y);
                cout << "Safe! Keep going.\n";
                gameBoard.ShowBoard(false);

                if (gameBoard.IsUserWin()) {
                    gameOver = true;
                    player.SetWin(true);
                    cout << "Congratulations! You have cleared all safe cells!\n";
                }
            }
        }

        auto end = chrono::high_resolution_clock::now();
        unsigned int duration = chrono::duration_cast<chrono::seconds>(end - start).count();

        player.SetSeconds(duration);

        time_t now = time(0);
        tm* ltm = localtime(&now);
        uint16_t day = ltm->tm_mday;
        uint16_t month = 1 + ltm->tm_mon;
        uint16_t year = 1900 + ltm->tm_year;
        player.SetDate(day, month, year);

        player.SaveToFile("players.txt");

        break;
    }
    case 2: {
        ShowPlayers("players.txt");
        break;
    }
    case 3: {
        // Robot logic
        break;
    }
    case 4: {
        cout << "Goodbye!\n";
        gameOver = true;
        return 0;
    }
    default: {
        cout << "Invalid option. Exiting.\n";
        return 0;
    }
    }

return 0;
}
