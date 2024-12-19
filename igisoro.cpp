#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

// Function to display the board
void displayBoard(const char board[]) {
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        cout << " " << board[i] << " ";
        if ((i + 1) % 3 == 0) {
            cout << "\n";
            if (i < 8) cout << "---------\n";
        } else {
            cout << "|";
        }
    }
    cout << "\n";
}

// Function to check if a player has won
bool checkWinner(const char board[], char player) {
    // Check rows, columns, and diagonals
    int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };
    for (auto &pattern : winPatterns) {
        if (board[pattern[0]] == player &&
            board[pattern[1]] == player &&
            board[pattern[2]] == player) {
            return true;
        }
    }
    return false;
}

// Function to check for a draw
bool checkDraw(const char board[]) {
    for (int i = 0; i < 9; i++) {
        if (board[i] >= '1' && board[i] <= '9') return false; // Unplayed cell exists
    }
    return true;
}

// Main game function
void playGame() {
    char board[9];
    for (int i = 0; i < 9; i++) {
        board[i] = '1' + i; // Fill board with 1-9
    }

    char currentPlayer = 'T'; // Player X starts
    bool gameOver = false;

    while (!gameOver) {
        displayBoard(board);

        // Input move
        int move;
        cout << "Player " << currentPlayer << ", enter your move (1-9): ";
        cin >> move;

        // Validate move
        if (move < 1 || move > 9 || board[move - 1] == 'T' || board[move - 1] == 'R') {
            cout << "Invalid move! Try again.\n";
            continue;
        }

        // Update board
        board[move - 1] = currentPlayer;

        // Check for winner or draw
        if (checkWinner(board, currentPlayer)) {
            displayBoard(board);
            cout << "Player " << currentPlayer << " wins! Congratulations!\n";
            gameOver = true;
        } else if (checkDraw(board)) {
            displayBoard(board);
            cout << "It's a draw! Well played both players!\n";
            gameOver = true;
        } else {
            // Switch player
            currentPlayer = (currentPlayer == 'T') ? 'R' : 'T';
        }
    }
}

int main() {
    char playAgain;

    do {
        cout << "Welcome to Tic-Tac-Toe!\n";
        playGame();

        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
    } while (tolower(playAgain) == 'y');

    cout << "Thank you for playing Tic-Tac-Toe. Goodbye!\n";
    return 0;
}