#include "Board.h"
constexpr int THIRDHEART = 17; // Position of the third heart
constexpr int SECONDHEART = 13; // Position of the second heart
constexpr int FIRSTHEART = 9; // Position of the first heart

// Reset the board to its initial state
bool Board::reset(const char* filename, int currentLives) {
    gotoxy(0, 0);
    system("cls");
    int countMario = 0;
    int countPrincess = 0;
    int countDonkey = 0;
    int countLegend = 0;
    bool goToNextScreen = false;
    bool isLastLineValid = true;
    countLives = currentLives;
    clearGhostLocations();
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file " << filename << '\n';
        return false;
    }
    boardState.resize(MAX_Y, std::vector<char>(MAX_X, ' ')); // Initialize boardState
    int i, j;
    for (i = 0; i < MAX_Y; i++) {
        for (j = 0; j < MAX_X; j++) {
            char ch = file.get();
            if (ch == EOF) {
                gotoxy(5, 12);
                std::cout << "Error: There are missing lines in the board file " << filename << "\n";
                goToNextScreen = true;
                break;
            }
            if (i == MAX_Y - 1) { // Last line
                if (ch == ' ')
                    isLastLineValid = false;
            }
            currentBoard[i][j] = ch;
            boardState[i][j] = ch; // Update boardState
            if (ch == '@') {
                countMario++;
                if (countMario == 1) { // If Mario is found, set his starting position (only once)
                    MarioStartPoint.setX(j);
                    MarioStartPoint.setY(i);
                }
                currentBoard[i][j] = ' '; // Replace Mario character with empty space
                boardState[i][j] = ' '; // Update boardState
            }
            else if (ch == '$') {
                countPrincess++;
                if (countPrincess == 1) { // If Princess is found, set her starting position (only once)
                    PrincessStartPoint.setX(j);
                    PrincessStartPoint.setY(i);
                }
                else if (countPrincess >= 2) {
                    currentBoard[i][j] = ' '; // Replace Princess character with empty space
                    boardState[i][j] = ' '; // Update boardState
                }
            }
            else if (ch == '&') {
                countDonkey++;
                if (countDonkey == 1) { // If Donkey Kong is found, set his starting position (only once)
                    DonkeyStartPoint.setX(j);
                    DonkeyStartPoint.setY(i);
                }
                else if (countDonkey >= 2) {
                    currentBoard[i][j] = ' '; // Replace Donkey Kong character with empty space
                    boardState[i][j] = ' '; // Update boardState
                }
            }
            else if (ch == 'L') {
                countLegend++;
                if (countLegend == 1) { // If the legend is found, set the position of the legend (only once)
                    LPosition.setX(j);
                    LPosition.setY(i);
                    currentBoard[i][j + 1] = 'i'; currentBoard[i][j + 2] = 'v'; currentBoard[i][j + 3] = 'e'; currentBoard[i][j + 4] = 's'; currentBoard[i][j + 5] = ':';
                    boardState[i][j + 1] = 'i'; boardState[i][j + 2] = 'v'; boardState[i][j + 3] = 'e'; boardState[i][j + 4] = 's'; boardState[i][j + 5] = ':';
                    THIRDSTAR = LPosition.getX() + 11; // Position of the third star
                    SECONDSTAR = LPosition.getX() + 9; // Position of the second star
                    FIRSTSTAR = LPosition.getX() + 7; // Position of the first star
                    std::streamoff index = file.tellg();
                    file.seekg(index + 5); // Skip the rest of the "Lives:" string
                    j += 5;
                }
                else if (countLegend >= 2) {
                    currentBoard[i][j] = ' '; // Replace the number of lives with an empty space
                    boardState[i][j] = ' '; // Update boardState
                }
            }
            else if (ch == 'x') {
                currentBoard[i][j] = ' ';
                addGhostLocation(j, i, false);
            }
            else if (ch == 'X') {
                currentBoard[i][j] = ' ';
                addGhostLocation(j, i, true);
            }
        }
    }
    char underDonkey = currentBoard[DonkeyStartPoint.getY() + 1][DonkeyStartPoint.getX()];
    char underPrincess = currentBoard[PrincessStartPoint.getY() + 1][PrincessStartPoint.getX()];
    char underMario = currentBoard[MarioStartPoint.getY() + 1][MarioStartPoint.getX()];
    bool AllCharactersOnFloor = true;
    if (underDonkey != '=' && underDonkey != '>' && underDonkey != '<' && underDonkey != 'Q')
        AllCharactersOnFloor = false;
    if (underPrincess != '=' && underPrincess != '>' && underPrincess != '<' && underPrincess != 'Q')
        AllCharactersOnFloor = false;
    if (underMario != '=' && underMario != '>' && underMario != '<' && underMario != 'Q')
        AllCharactersOnFloor = false;

    // Check if all required characters were found
    if (countMario == 0 || countPrincess == 0 || countDonkey == 0 || countLegend == 0) {
        gotoxy(5, 12);
        std::cout << "Error: There are missing characters in the board file " << filename << "\n";
        goToNextScreen = true;
    }
    if (!isLastLineValid) {
        gotoxy(5, 12);
        std::cout << "Error: The last line is empty in the board file " << filename << "\n";
        goToNextScreen = true;
    }
    if (!AllCharactersOnFloor) {
        gotoxy(5, 11);
        std::cout << "Error: There are characters in invalid placement " << filename << "\n";
        gotoxy(5, 12);
        std::cout << "       in the board file " << filename << "\n";
        goToNextScreen = true;
    }
    if (goToNextScreen) { // Current file is not valid
        Sleep(2000);
        gotoxy(5, 14);
        std::cout << "Loading the next screen...\n";
        Sleep(3000);
        file.close();
        return false; // Goes to the next file
    }

    // Explicitly set the position for the score
    int scoreX  = LPosition.getX();
    ScorePosition.setX(LPosition.getX());
    int scoreY = LPosition.getY() + 2;
    ScorePosition.setY(LPosition.getY() + 2);

    P_ActivePosition.setX(ScorePosition.getX() + 15);
    P_ActivePosition.setY(ScorePosition.getY());
    currentBoard[scoreY][scoreX] = 'S'; currentBoard[scoreY][scoreX + 1] = 'c'; currentBoard[scoreY][scoreX + 2] = 'o'; currentBoard[scoreY][scoreX + 3] = 'r'; currentBoard[scoreY][scoreX + 4] = 'e'; currentBoard[scoreY][scoreX + 5] = ':'; currentBoard[scoreY][scoreX + 6] = ' ';
    boardState[scoreY][scoreX] = 'S'; boardState[scoreY][scoreX + 1] = 'c'; boardState[scoreY][scoreX + 2] = 'o'; boardState[scoreY][scoreX + 3] = 'r'; boardState[scoreY][scoreX + 4] = 'e'; boardState[scoreY][scoreX + 5] = ':'; boardState[scoreY][scoreX + 6] = ' ';
    ScorePosition.setX(scoreX + 6);
    ScorePosition.setY(scoreY);
    int LPositionY = LPosition.getY();
    if (countLives == 3) {
        setChar(FIRSTSTAR, LPositionY, '*'); // Draw the first star
        setChar(SECONDSTAR, LPositionY, '*'); // Draw the second star
        setChar(THIRDSTAR, LPositionY, '*'); // Draw the third star
    }
    else if (countLives == 2) {
        setChar(FIRSTSTAR, LPositionY, '*'); // Draw the first star
        setChar(SECONDSTAR, LPositionY, '*'); // Draw the second star
    }
    else {
        setChar(FIRSTSTAR, LPositionY, '*'); // Draw the first star
    }
    setScore(score);
    file.close();
    return true;
}

// Get the current board state
const std::vector<std::vector<char>>& Board::getCurrentBoard() const {
    return boardState;
}

// Print the current state of the board
void Board::print() const { // Taken from amirs exercise
    gotoxy(0, 0); // Move cursor to the top-left corner
    for (int i = 0; i < MAX_Y - 1; i++) {
        std::cout << currentBoard[i]; // Print each row of the board
    }
    std::cout << currentBoard[MAX_Y - 1]; // Print the last row without a newline
}

// Set the score
void Board::setScore(int Score) {
    score = Score;
    if (score < 10)
        setChar(ScorePosition.getX(), ScorePosition.getY(), score + '0'); // Draw the score
    else {
        setChar(ScorePosition.getX(), ScorePosition.getY(), '1'); // Draw the score
        setChar(ScorePosition.getX() + 1, ScorePosition.getY(), score % 10 + '0'); // Draw the score
    }
    gotoxy(ScorePosition.getX(), ScorePosition.getY());
    std::cout << currentBoard[ScorePosition.getY()][ScorePosition.getX()] << currentBoard[ScorePosition.getY()][ScorePosition.getX() + 1];
}

// Update the score
void Board::updateScore() {
    score++;
    if (score < 10) {
        setChar(ScorePosition.getX(), ScorePosition.getY(), score + '0'); // Draw the score
    }
    else {
        setChar(ScorePosition.getX(), ScorePosition.getY(), '1'); // Draw the score
        setChar(ScorePosition.getX() + 1, ScorePosition.getY(), score % 10 + '0'); // Draw the score
    }
    gotoxy(ScorePosition.getX(), ScorePosition.getY());
    std::cout << currentBoard[ScorePosition.getY()][ScorePosition.getX()] << currentBoard[ScorePosition.getY()][ScorePosition.getX() + 1];
}

// Set the number of lives
void Board::setLives(int lives) {
    countLives = lives;
}

// Add a ghost location to the board
void Board::addGhostLocation(int x, int y, bool Super) {
    ghostLocations.emplace_back(x, y, Super);
}

// Get the ghost locations on the board
const std::vector<std::tuple<int, int, bool>>& Board::getGhostLocations() const {
    return ghostLocations;
}

// Clear the ghost locations on the board
void Board::clearGhostLocations() {
    ghostLocations.clear();
}

// Set a character at a specific position on the board
void Board::setChar(int x, int y, char c) {
    if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) {
        currentBoard[y][x] = c; // Update the character at the specified position
    }
}

// Set a string at a specific position on the board
void Board::setString(int x, int y, const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (x + i < MAX_X) {
            currentBoard[y][x + i] = str[i]; // Update the board with the string
        }
        else {
            break; // Stop if the string exceeds the board width
        }
    }
}

// Remove a heart from the board
void Board::removeHeart() {
    int LPositionY = LPosition.getY();
    countLives--;
    if (getChar(THIRDSTAR, LPositionY) == '*') {
        setChar(THIRDSTAR, LPositionY, ' '); // Remove the third star
    }
    else if (getChar(SECONDSTAR, LPositionY) == '*') {
        setChar(SECONDSTAR, LPositionY, ' '); // Remove the second star
    }
    else {
        setChar(FIRSTSTAR, LPositionY, ' '); // Remove the first star
    }
}

// Determine how many hearts are left on the board
int Board::HowManyHeart() const {
    int LPositionY = LPosition.getY();
    if (getChar(THIRDSTAR, LPositionY) == '*') {
        return 3;
    }
    else if (getChar(SECONDSTAR, LPositionY) == '*') {
        return 2;
    }
    else {
        return 1;
    }
}

// Check if a position contains a heart
bool Board::isHeart(int x, int y) const {
    if (IsGameColored)
        return getChar(x, y) == '\xE2' &&
        getChar(x + 1, y) == '\x9D' &&
        getChar(x + 2, y) == '\xA4'; // Check for heart characters
    else
        return getChar(x, y) == '*'; // Check for star character
}

// Get the character at a specific position on the board
char Board::getChar(int x, int y) const { // Taken from amirs exercise
    return currentBoard[y][x];
}

