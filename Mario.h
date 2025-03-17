#pragma once
#include "Board.h"
#include "Point.h"
class BarrelArmy;
class GhostManager;

class Mario {
    Mario(const Mario&) = delete; // Delete copy constructor

    // Enum class for movement keys
    enum class MoveKey {
        UP = 'w',
        LEFT = 'a',
        DOWN = 'x',
        RIGHT = 'd',
        STAY = 's',
        PUNCH = 'p' // for hammer
    };

    // Number of movement keys
    static constexpr size_t numKeys = 6; // UP, LEFT, DOWN, RIGHT, STAY, PUNCH

    // Structure to represent directions
    struct Direction { int x, y; }; // Taken from amirs exercise

    // Directions corresponding to keys (w, a, x, d, s)
    static constexpr Direction directions[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0} }; // Taken from amirs exercise

    // Mario's initial position
    Point p;

    BarrelArmy* pBarrelArmy = nullptr; // Pointer to the BarrelArmy object
    GhostManager* pGhostManager = nullptr; // Pointer to the GhostManager object
    Point StartPoint; // Starting position of Mario
    Point PrincessPoint; // Position of the Princess
    bool IsHammer; // Flag to indicate if Mario has a hammer
    bool IsColored; // Flag to indicate if Mario is colored
    int IsKilling; // Counter for killing state
    bool IsSilent = false;
    int iterationwin = 0;
    // Counter for falling
    int countFall = 0;

    // Current direction Mario is facing
    Direction dir{ 0, 0 };

    // Character representation of Mario
    char ch = '@';

    // Jump counter and state
    int jump = 0;
    bool isJumping = false;

    // Ladder interaction state
    bool isClimb = false;

    // Pointer to the game board
    Board* pBoard = nullptr;

    // Function to handle Mario's damage (not implemented here)
    void marioDamage();

    // Previous direction (for undoing movement if needed)
    Direction prevDir = { 0, 0 };
    Direction prevDirHammer = { 0, 0 };

public:
    // Set the starting position of Mario
    void setStartPosition(Point p) {
        StartPoint.setX(p.getX());
        StartPoint.setY(p.getY());
        this->p.setX(StartPoint.getX());
        this->p.setY(StartPoint.getY());
    }


    // Constructor to initialize Mario
    Mario() : p(p.getX(), p.getY()), IsHammer(false), IsColored(false), IsKilling(0), Back(false), LevelComplete(false) {}

    // Checks if Mario is standing on the floor
    bool isFloor() const;

    // Checks if Mario is on a ladder
    bool isLadder() const;

    // Checks if Mario is under a ceiling
    bool isCeiling() const;

    // Flag indicating if Mario needs to be reset
    bool needsReset = false;

    // Getter for Mario's X position
    int getX() const { return p.getX(); }

    // Getter for Mario's Y position
    int getY() const { return p.getY(); }

    // Getter for Mario's direction on X axis
    int getdirX() const { return dir.x; }

    // Getter for Mario's hammer direction on X axis
    int getDirHammerX() const { return prevDirHammer.x; }

    // Getter for Mario's direction on Y axis
    int getdirY() const { return dir.y; }

    // Function to draw Mario (with default '@' character)
    void draw() const {
        draw(ch);
    }

    // Function to draw Mario with a specific character
    void draw(char c) const;
    // Function to erase Mario
    void erase() const;
    // Function to handle killing state
    void kill();
    // Function to show the score note
    void showScoreNote();
    // Function to print a digit at a specific position
    void printDigit(int digit, Point p);
    // Function to set the game board
    void setBoard(Board& board);

    // Function to set the BarrelArmy object
    void setBarrelArmy(BarrelArmy& barrelArmy) {
        pBarrelArmy = &barrelArmy;
    }

    // Function to set the GhostManager object
    void setGhostManager(GhostManager& ghostManager) {
        pGhostManager = &ghostManager;
    }

    bool Back; // Flag to indicate if Mario is returning to the menu
    bool LevelComplete; // Flag to indicate if the level is complete

    // Handles key press events to move Mario
    void keyPressed(char key);

    // Function to move Mario based on direction
    void move();

    // Getter for Mario's starting point
    Point getStartPoint() const { return StartPoint; }
    // Setter for Mario's starting point
    void setStartPoint(Point p) { StartPoint = p; }
    // Getter for the Princess's point
    Point getPrincessPoint() const { return PrincessPoint; }
    // Setter for the Princess's point
    void setPrincessPoint(Point p) { PrincessPoint = p; }
    // Getter for the hammer flag
    bool getIsHammer() const { return IsHammer; }
    // Setter for the hammer flag
    void setIsHammer(bool value) { IsHammer = value; }
    // Getter for the colored flag
    bool getIsColored() const { return IsColored; }
    // Setter for the colored flag
    void setIsColored(bool value) { IsColored = value; }
    // Getter for the killing state
    int getIsKilling() const { return IsKilling; }
    // Setter for the killing state
    void setIsKilling(int value) { IsKilling = value; }

    // Function to check if Mario is returning to the menu
    bool ToMenu() const;

    // Function to check if the level is updated
    bool updateLevel() const;

    // Function to handle the winning state (not implemented here)
    void Win();

    // Function to handle collision with a barrel
    void handleBarrel(int barrelX, int barrelY);
    // Function to handle collision with a ghost
    void handleGhost(int barrelX, int barrelY);

    // Function to show the lose note
    void showLoseNote();

    // Function to show the win note
    void showWinNote();

    // Function to show the game finish note
    void showGameFinishNote();

    // Function to print Mario's photo
    void printMarioPhoto(Point p);

    // Function to print Mario's photo in color
    void printMarioPhotoColored(Point p);

    // Function to print the monkey's photo
    void printMonkeyPhoto(Point p);

    // Function to print the monkey's photo in color
    void printMonkeyPhotoColored(Point p);
	void setIsSilent(bool silent) { IsSilent = silent; }

};

