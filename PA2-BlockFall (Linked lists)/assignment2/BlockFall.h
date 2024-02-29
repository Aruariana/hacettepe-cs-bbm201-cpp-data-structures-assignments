#ifndef BLOCKFALL_H
#define BLOCKFALL_H

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"
//#define occupiedCellChar "⬜"
//#define unoccupiedCellChar "⬛"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Block.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"

using namespace std;

class BlockFall {
public:

    BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name,
              const string &player_name);
    virtual ~BlockFall();

    int rows;  // Number of rows in the grid
    int cols;  // Number of columns in the grid
    vector<vector<int> > grid;  // 2D game grid
    vector<vector<bool>> power_up; // 2D matrix of the power-up shape"
    Block * initial_block = nullptr; // Head of the list of game blocks. Must be filled up and initialized after a call to read_blocks()
    Block * active_rotation = nullptr; // Currently active rotation of the active block. Must start with the initial_block
    bool gravity_mode_on = false; // Gravity mode of the game
    unsigned long current_score = 0; // Current score of the game
    string leaderboard_file_name; // Leaderboard file name, taken from the command-line argument 5 in main
    string player_name; // Player name, taken from the command-line argument 6 in main
    Leaderboard leaderboard;
    // Helper variables
    int blockXPos = 0;
    int blockYPos = 0;

    void initialize_grid(const string & input_file); // Initializes the grid using the command-line argument 1 in main
    void read_blocks(const string & input_file); // Reads the input file and calls the read_block() function for each block;

    // Helper functions
    Block* getBlock(const string & strMatrix); // Generates a block dynamically from a string
    void makeRotations(Block *initialBlock); // Generates possible rotations for a block
    vector<vector<bool>> turnRight(const vector<vector<bool>> &shape); // Returns the given matrix's clockwise orientation
    bool collisionDetected(); // Detect collision for active block

    void moveRight(); // Moves the active block right if possible
    void moveLeft(); // Moves the active block left if possible
    void rotateRight(); // Rotate the active block right if possible
    void rotateLeft(); // Rotate the active block left if possible
    void drop(); // Drops the block and gets the next block as active rotation, resets blockXPos and blockYPos
    bool powerUpFound(); // Searches for a power up in the grid
    bool clearFullRows(bool flag); // Clears the first full row it finds and returns true, if it doesn't find returns false
    void dropBlocks(); // When gravity is on, drop all the blocks accordingly

    void insertBlockToGrid();
    void deleteBlockFromGrid();

    bool isGameOver();
    bool isOutOfBlocks();

    void addEntry();

    void printGrid(bool inserted);
    void printGridWithScore(bool inserted);

    void stdoutGameOver();
    void stdoutOutOfBlocks();
    void stdoutNoMoreCommands();
};



#endif // BLOCKFALL_H
