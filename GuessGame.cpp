#include <iostream>
#include <cstring>
#include <ctime>
#include <cctype>

#define MAX_WORDS 5
#define MAX_ATTEMPTS 6

using namespace std;

void displayWord(const char word[], const int guessed[]) {
    for (size_t i = 0; i < strlen(word); i++) {
        if (guessed[i])
            cout << word[i] << " ";
        else
            cout << "_ ";
    }
    cout << endl;
}

bool isWordGuessed(const int guessed[], size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (!guessed[i])
            return false;
    }
    return true;
}

void getWord(char word[]) {
    const char* wordList[MAX_WORDS] = {"teacher", "programming", "hangman", "computer", "puzzle"};
    strcpy(word, wordList[rand() % MAX_WORDS]);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    char word[50];
    getWord(word);
    size_t wordLength = strlen(word);
    int guessed[wordLength] = {0};

    char incorrectGuesses[MAX_ATTEMPTS + 1] = {0};
    int incorrectCount = 0;

    cout << "Welcome to Hangman!" << endl;

    while (incorrectCount < MAX_ATTEMPTS) {
        cout << "\nWord: ";
        displayWord(word, guessed);
        cout << "Incorrect Guesses: ";
        for (int i = 0; i < incorrectCount; i++) {
            cout << incorrectGuesses[i] << " ";
        }
        cout << "\nGuesses Left: " << MAX_ATTEMPTS - incorrectCount << endl;

        cout << "Guess a letter: ";
        char guess;
        cin >> guess;

        // Validate input
        guess = tolower(guess);
        if (!isalpha(guess)) {
            cout << "Invalid input. Please enter a single letter." << endl;
            continue;
        }

        bool alreadyGuessed = false;
        for (int i = 0; i < incorrectCount; i++) {
            if (incorrectGuesses[i] == guess) {
                alreadyGuessed = true;
                break;
            }
        }
        if (alreadyGuessed) {
            cout << "You already guessed that letter. Try again." << endl;
            continue;
        }

        // Check if the guess is correct
        bool correct = false;
        for (size_t i = 0; i < wordLength; i++) {
            if (word[i] == guess) {
                guessed[i] = 1;
                correct = true;
            }
        }

        if (!correct) {
            incorrectGuesses[incorrectCount++] = guess;
        }

        // Check if the word is fully guessed
        if (isWordGuessed(guessed, wordLength)) {
            cout << "\nCongratulations! You guessed the word \"" << word << "\" correctly!" << endl;
            break;
        }
    }

    if (incorrectCount == MAX_ATTEMPTS) {
        cout << "\nYou ran out of attempts! The correct word was \"" << word << "\". Better luck next time!" << endl;
    }

    return 0;
}
