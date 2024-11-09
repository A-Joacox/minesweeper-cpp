#include <iostream>
#include <random>
#include <vector>
#include "classes_h.h"

using namespace std;

int main() {
    int boardW = 5, boardL = 5;
    int mineCount = 5;

    
    Board gameBoard(boardW, boardL);
    gameBoard.ShowBoard();

    //create the Mine vector with random positions
    vector<Mine> mines;
    for (int i = 0; i < mineCount; i++){
        mines.push_back(Mine(generateRandomNumber(0, boardW-1), generateRandomNumber(0, boardL-1)));
    }
    
    //check duplicates
    gameBoard.CheckDuplicates(mines); 

    //prepare the mines
    vector<vector<int>> minePositions; 
    for (const auto& mine : mines) {
        minePositions.push_back({ mine.GetX(), mine.GetY() });
    }

    //add them to the board
    gameBoard.AddMines(minePositions);
    cout << "\nminas agregadas:\n";
    gameBoard.ShowBoard();
    
return 0;
}