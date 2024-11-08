#include <iostream>
#include <random>
#include <vector>
#include "classes_h.h"

using namespace std;

int generateRandomNumber(int min, int max) {
    random_device rd;  
    mt19937 gen(rd()); // mt engine, seed is rd
    uniform_int_distribution<> distr(min, max);
return distr(gen);
}


int main() {
    Board gameBoard(5, 5);
    gameBoard.ShowBoard();

    vector<vector<int>> mines = {{1, 2}, {1, 1}, {3, 4},{0,0}};
    gameBoard.AddMines(mines);
    
    std::cout << "minas agregadas:\n";

    gameBoard.ShowBoard();


return 0;
}