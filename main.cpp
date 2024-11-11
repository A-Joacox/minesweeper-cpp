#include "classes_h.h"

using namespace std;

int main() {
    int boardW = 8, boardL = 8;
    int mineCount = 8;


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
    
    int x, y;
    bool gameOver = false;
    char action = '\0';

    while (!gameOver) {
        cout << "enter row and column to reveal (e.g., '1 2') or '1 2 f' to flag/unflag: ";
        cin >> x >> y;
        x--; 
        y--; // adjust for 0,0 indexing

        if (cin.peek() == ' '){
            cin >> action;
        } else {
            action = '\0';
        }

        if (x < 0 || x >= boardW || y < 0 || y >= boardL) {
            cout << "Invalid input.\n";
            cin.clear();
            // discard invalid input, this from some stack overflow code
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
            continue;
        }

        if (action == 'f') {
            gameBoard.FlagCell(x, y);
            cout << "flag at (" << x + 1 << ", " << y + 1 << ").\n";
            gameBoard.ShowBoard(false);

        } else if (gameBoard.CheckPosition(x, y)) {
            cout << "Boom! You hit a mine. Game Over.\n";
            gameBoard.ShowBoard(true); // show all mines on game over
            gameOver = true;
            
        }else {
            gameBoard.RevealCell(x, y);
            cout << "Safe! Keep going.\n";
            gameBoard.ShowBoard(false);
        }
    }
return 0;
}