#pragma once
#include <cstring>
#include <iostream>
#include "utils.h"
#include <fstream>
#include <vector>
#include "Point.h"

class Board {
private:
    int THIRDSTAR; // Score threshold for the third star
    int SECONDSTAR; // Score threshold for the second star
    int FIRSTSTAR; // Score threshold for the first star
    int score; // Current score
    Point MarioStartPoint; // Starting position of Mario
    Point PrincessStartPoint; // Starting position of the Princess
    Point DonkeyStartPoint; // Starting position of Donkey Kong
    Point LPosition; // Position of the L character
    Point ScorePosition; // Position of the score display
    Point HammerPosition; // Position of the hammer
    Point P_ActivePosition; // Position of the active P character
    bool IsGameColored; // Flag to determine if the game is colored
    int countLives; // Number of lives remaining
    std::vector<std::vector<char>> boardState; // 2D vector to store the board state
    Board(const Board&) = delete; // Delete copy constructor
    bool isHeart(int x, int y) const; // Check if a position contains a heart

    static constexpr int MAX_X = 80; // Maximum width of the board
    static constexpr int MAX_Y = 25; // Maximum height of the board

    char currentBoard[MAX_Y][MAX_X + 1]; // Current state of the board
    std::vector<std::tuple<int, int,bool>> ghostLocations; // Vector to store ghost locations

public:
    const std::vector<std::vector<char>>& getCurrentBoard() const; // Get the current board state
    // Constructor to initialize the board
    Board() : countLives(3),
        score(0),
        IsGameColored(false),
        THIRDSTAR(-1),
        SECONDSTAR(-1),
        FIRSTSTAR(-1),
        MarioStartPoint(-1, -1),
        PrincessStartPoint(-1, -1),
        DonkeyStartPoint(-1, -1),
        LPosition(-1, -1),
        ScorePosition(-1, -1)
    {
        // Initialize currentBoard with spaces
        for (int i = 0; i < MAX_Y; ++i) {
            for (int j = 0; j < MAX_X; ++j) {
                currentBoard[i][j] = ' ';
            }
            currentBoard[i][MAX_X] = '\0'; // Null-terminate each row
        }
    }

    // Set the number of lives
    void setLives(int lives);
    // Set the score
    void setScore(int Score);
    // Update the score
    void updateScore();

    // Getters for various positions and states
    int getThirdStar() const { return THIRDSTAR; }
    int getSecondStar() const { return SECONDSTAR; }
    int getFirstStar() const { return FIRSTSTAR; }
    Point getMarioStartPoint() const { return MarioStartPoint; }
    Point getPrincessStartPoint() const { return PrincessStartPoint; }
    Point getDonkeyStartPoint() const { return DonkeyStartPoint; }
    Point getLPosition() const { return LPosition; }
    Point getScorePosition() const { return ScorePosition; }
    Point getHammerPosition() const { return HammerPosition; }
    Point getPActivePosition() const { return P_ActivePosition; }
    int getScore() const { return score; }
    bool getIsGameColored() const { return IsGameColored; }
    void setIsGameColored(bool value) { IsGameColored = value; }

    // Reset the board to its initial state
    bool reset(const char* filename, int currentLives);
    // Print the current state of the board
    void print() const;
    // Get the character at a specific position on the board
    char getChar(int x, int y) const;
    // Set a character at a specific position on the board
    void setChar(int x, int y, char c);
    // Set a string at a specific position on the board
    void setString(int x, int y, const std::string& str);
    // Remove a heart from the board
    void removeHeart();
    // Determine how many hearts are left on the board
    int HowManyHeart() const;

    // Manage ghost locations
    void addGhostLocation(int x, int y, bool Super);
    const std::vector<std::tuple<int, int,bool>>& getGhostLocations() const;
    void clearGhostLocations();
    int getLives() const { return countLives; }
};
