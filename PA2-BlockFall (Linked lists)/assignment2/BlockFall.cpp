#include "BlockFall.h"

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)

    ifstream file(input_file);
    // Check if the file is open
    if(!file.is_open()) {
        cerr << "Could not open the file: " << input_file << endl;
        return;
    }

    // Read the file line by line
    string line;
    string matrix;
    Block* temp = nullptr;
    // Initialize initial_block and active_rotation and construct the linked list
    while(getline(file, line)) {
        if(line[0] == '[') {
            if(line.back() == ']') {
                matrix += line.substr(1, line.length()-2);
            }
            else {
                matrix += line.substr(1) + "\n";
                while(getline(file, line)) {
                    if(line.back() == ']') {
                        matrix += line.substr(0, line.length()-1);
                        break;
                    }
                    matrix += line + "\n";
                }
            }

            if(initial_block == nullptr) {
                initial_block = getBlock(matrix);
                active_rotation = initial_block;
                temp = initial_block;
            }
            else {
                temp->next_block = getBlock(matrix);
                temp = temp->next_block;
            }
            matrix = "";
        }
    }

    // Get the power up shape and delete the power up block from linked list
    for(Block* temp3 = initial_block; temp3 != nullptr; temp3 = temp3->next_block) {
        if(temp3->next_block->next_block == nullptr) {
            power_up = temp3->next_block->shape;
            delete temp3->next_block;
            temp3->next_block = nullptr;
        }
    }

    // Make rotations
    for(Block* temp3 = initial_block; temp3 != nullptr; temp3 = temp3->next_block) {
        makeRotations(temp3);
    }
}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

    ifstream file(input_file);

    // Check if the file is open
    if(!file.is_open()) {
        cerr << "Could not open the file: " << input_file << endl;
        return;
    }

    // Initialize the grid
    string line;
    while(getline(file, line)) {
        vector<int> aRow;

        istringstream strStream(line);
        string element;

        while(getline(strStream, element, ' ')) {
            aRow.push_back(stoi(element));
        }

        grid.push_back(aRow);
    }
    // Close the file
    file.close();

    // Initialize rows and cols member variables
    rows = static_cast<int>(grid.size());
    cols = static_cast<int>(grid[0].size());
}

Block* BlockFall::getBlock(const std::string &strMatrix) {
    Block* pBlock = new Block;
    vector<vector<bool>> shape;

    stringstream strStream(strMatrix);
    string line;
    while(getline(strStream, line, '\n')) {
        vector<bool> aRow;
        for(int i = 0; i < line.length(); i++) {
            aRow.push_back((line[i] == '1'));
        }
        shape.push_back(aRow);
    }
    pBlock->shape = shape;
    return pBlock;
}

void BlockFall::makeRotations(Block *initialBlock) {
    initialBlock->right_rotation = initialBlock;
    initialBlock->left_rotation = initialBlock;
    Block* temp = initialBlock;
    Block* temp2 = temp;
    while(true) {
        Block* rotatedBlock = new Block;
        rotatedBlock->shape = turnRight(temp2->shape);

        bool equalFound = false;
        do {
            if(*temp == *rotatedBlock) {
                equalFound = true;
                break;
            }
            temp = temp->right_rotation;
        }
        while(temp != initialBlock);

        if(equalFound) {
            delete rotatedBlock;
            break;
        }
        else {
            temp2->right_rotation = rotatedBlock;
            rotatedBlock->left_rotation = temp2;

            initialBlock->left_rotation = rotatedBlock;
            rotatedBlock->right_rotation = initialBlock;

            rotatedBlock->next_block = initialBlock->next_block;

            temp2 = rotatedBlock;
        }
    }
}

vector<vector<bool>> BlockFall::turnRight(const vector<vector<bool>> &shape) {
    // Can be changed later to diagonal and horizontal flip
    vector<vector<bool>> turned2dVec;
    for(int i = 0; i < shape[0].size(); i++) {
        vector<bool> aRow;
        for(int j = 0; j < shape.size(); j++) {
            aRow.push_back(false);
        }
        turned2dVec.push_back(aRow);
    }

    for(int i = 0; i < shape.size(); i++) {
        for(int j = 0; j < shape[0].size(); j++) {
            turned2dVec[j][i] = shape[i][j];
        }
    }

    for (int i = 0; i < turned2dVec.size(); i++) {
        std::reverse(turned2dVec[i].begin(), turned2dVec[i].end());
    }

    return turned2dVec;
}

bool BlockFall::collisionDetected() {
    int activeBlockRowSize = (int)active_rotation->shape.size();
    int activeBlockColumnSize = (int)active_rotation->shape[0].size();
    //  Detect if the block is going out of grid
    if(blockXPos+activeBlockRowSize > rows || blockYPos+activeBlockColumnSize > cols || blockXPos < 0 || blockYPos < 0) {
        return true;
    }
    // Detect if block is colliding with another block
    for(int i = 0; i < activeBlockRowSize; i++) {
        for(int j = 0; j < activeBlockColumnSize; j++) {
            if(active_rotation->shape[i][j] == 1 && grid[blockXPos+i][blockYPos+j] == 1) {
                return true;
            }
        }
    }
    // Otherwise collision is not detected
    return false;
}

void BlockFall::moveRight() {
    blockYPos++;
    if(collisionDetected()) {
        blockYPos--;
    }
}

void BlockFall::moveLeft() {
    blockYPos--;
    if(collisionDetected()) {
        blockYPos++;
    }
}

void BlockFall::rotateRight() {
    active_rotation = active_rotation->right_rotation;
    if(collisionDetected()) {
        active_rotation = active_rotation->left_rotation;
    }
}

void BlockFall::rotateLeft() {
    active_rotation = active_rotation->left_rotation;
    if(collisionDetected()) {
        active_rotation = active_rotation->right_rotation;
    }
}

void BlockFall::drop() {
    // Drop the block until collision
    while(!collisionDetected()) {
        blockXPos++;
    }
    blockXPos--;

    // Add the score of the block
    int occupiedCellsInShape = 0;
    for(int i = 0; i < active_rotation->shape.size(); i++) {
        for(int j = 0; j < active_rotation->shape[0].size(); j++) {
            if(active_rotation->shape[i][j] == 1) {
                occupiedCellsInShape += 1;
            }
        }
    }
    current_score += blockXPos * occupiedCellsInShape;

    // Print the block to the grid
    insertBlockToGrid();

    // If gravity is on, drop all blocks
    if(gravity_mode_on) {
        dropBlocks();
    }
    // Check for power up, if power up found clear the grid and update the score accordingly
    if(powerUpFound()) {
        cout << "Before clearing:" << endl;
        printGrid(true);
        cout << endl;
        int occCellsInGrid =  0;
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if(grid[i][j] == 1) {
                    occCellsInGrid += 1;
                }
                grid[i][j] = 0;
            }
        }
        current_score += (1000 + occCellsInGrid);
    }
    // Clear rows and update score
    bool beforeClearing = true;
    while(clearFullRows(beforeClearing)) {
        beforeClearing = false;
        current_score += cols;
    }

    active_rotation = active_rotation->next_block;
    blockXPos = 0;
    blockYPos = 0;

}

bool BlockFall::powerUpFound() {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            bool found = true;
            if((i + power_up.size()) <= rows && (j + power_up[0].size()) <= cols) {
                for(int c = 0; c < power_up.size(); c++) {
                    for(int d = 0; d < power_up[0].size(); d++) {
                        if(grid[i+c][j+d] != power_up[c][d]) {
                            found = false;
                            break;
                        }
                    }
                    if(!found) {
                        break;
                    }
                }
            }
            else {
                found = false;
            }

            if(found) {
                return true;
            }
        }
    }
    return false;
}

bool BlockFall::clearFullRows(bool flag) {
    for(int i = 0; i < rows; i++) {
        bool found = true;
        for(int j = 0; j < cols; j++) {
            if(grid[i][j] == 0) {
                found = false;
                break;
            }
        }
        if(found) {
            if(flag) {
                cout << "Before clearing:" << endl;
                printGrid(true);
                cout << endl;
            }
            for(int idx = i; idx > 0; idx--) {
                std::copy(grid[idx-1].begin(), grid[idx-1].end(), grid[idx].begin());
            }
            std::fill(grid[0].begin(), grid[0].end(), 0);
            return true;
        }
    }
    return false;
}

void BlockFall::dropBlocks() {
    for(int j = 0; j < cols; j++) {
        for(int i = rows-1; i > 0; i--) {
            for(int idx = i; idx >= 0; idx--) {
                if(grid[idx][j] == 1) {
                    grid[i][j] = grid[idx][j];
                    if(idx != i) {
                        grid[idx][j] = 0;
                    }
                    break;
                }
            }
        }
    }
}

void BlockFall::insertBlockToGrid() {
    for(int i = 0; i < active_rotation->shape.size(); i++) {
        for(int j = 0; j < active_rotation->shape[0].size(); j++) {
            if(active_rotation->shape[i][j] == 1) {
                grid[blockXPos+i][blockYPos+j] = 1;
            }
        }
    }
}

void BlockFall::deleteBlockFromGrid() {
    for(int i = 0; i < active_rotation->shape.size(); i++) {
        for(int j = 0; j < active_rotation->shape[0].size(); j++) {
            if(active_rotation->shape[i][j] == 1) {
                grid[blockXPos+i][blockYPos+j] = 0;
            }
        }
    }
}

void BlockFall::addEntry() {
    LeaderboardEntry* entry = new LeaderboardEntry(current_score, time(nullptr), player_name);
    leaderboard.insert_new_entry(entry);
    leaderboard.write_to_file(leaderboard_file_name);
}

void BlockFall::printGrid(bool inserted) {
    if(!inserted) {
        insertBlockToGrid();
    }
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << (grid[i][j] == 1 ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << endl;
    }
    cout << endl;
    if(!inserted) {
        deleteBlockFromGrid();
    }
}

void BlockFall::printGridWithScore(bool inserted) {
    unsigned long highScore;
    if(leaderboard.head_leaderboard_entry == nullptr) {
        highScore = current_score;
    }
    else {
        highScore = leaderboard.head_leaderboard_entry->score;
    }

    cout << "Score: " << current_score << endl;
    cout << "High Score: " << (current_score > highScore ? current_score : highScore) << endl;
    printGrid(inserted);
}

void BlockFall::stdoutGameOver() {
    cout << "GAME OVER!" << endl;
    cout << "Next block that couldn't fit:" << endl;
    for(int i = 0; i < active_rotation->shape.size(); i++) {
        for(int j = 0; j < active_rotation->shape[0].size(); j++) {
            cout << (active_rotation->shape[i][j] == 1 ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << endl;
    }
    cout << endl;

    cout << "Final grid and score:" << endl;
    cout << endl;
    printGridWithScore(true);
    leaderboard.print_leaderboard();
    cout << endl << endl;
}

void BlockFall::stdoutOutOfBlocks() {
    cout << "YOU WIN!" << endl;
    cout << "No more blocks." << endl;
    cout << "Final grid and score:" << endl;
    cout << endl;
    printGridWithScore(true);
    leaderboard.print_leaderboard();
    cout << endl << endl;
}

void BlockFall::stdoutNoMoreCommands() {
    cout << "GAME FINISHED!" << endl;
    cout << "No more commands." << endl;
    cout << "Final grid and score:" << endl;
    cout << endl;
    printGridWithScore(true);
    leaderboard.print_leaderboard();
    cout << endl << endl;
}

bool BlockFall::isGameOver() {
    return collisionDetected();
}

bool BlockFall::isOutOfBlocks() {
    return active_rotation == nullptr;
}

BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks,

    // Free rotations
    for(Block* temp = initial_block; temp != nullptr; temp = temp->next_block) {
        while(temp != temp->right_rotation) {
            Block* temp2 = temp->right_rotation;
            temp->right_rotation = temp2->right_rotation;
            delete temp2;
        }
    }

    // Free main blocks
    while(initial_block->next_block != nullptr) {
        Block* temp = initial_block->next_block;
        initial_block->next_block = temp->next_block;
        delete temp;
    }
    delete initial_block;
}
