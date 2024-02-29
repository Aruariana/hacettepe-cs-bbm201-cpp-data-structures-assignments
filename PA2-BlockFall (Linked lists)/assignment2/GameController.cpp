#include "GameController.h"

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    ifstream file(commands_file);
    if(!file.is_open()) {
        cerr << "Could not open the file: " << commands_file << endl;
    }
    string line;

    while(getline(file, line)) {
        // Check the game ending conditions
        if(game.isOutOfBlocks()) {
            file.close();
            game.addEntry();
            game.stdoutOutOfBlocks();
            return true;
        }
        if(game.isGameOver()) {
            file.close();
            game.addEntry();
            game.stdoutGameOver();
            return false;
        }

        // Execute commands
        if(line == "MOVE_RIGHT") {
            game.moveRight();
        }
        else if(line == "MOVE_LEFT") {
            game.moveLeft();
        }
        else if(line == "ROTATE_RIGHT") {
            game.rotateRight();
        }
        else if(line == "ROTATE_LEFT") {
            game.rotateLeft();
        }
        else if(line == "DROP") {
            game.drop();
        }
        else if(line == "GRAVITY_SWITCH") {
            game.gravity_mode_on = !game.gravity_mode_on;
            if(game.gravity_mode_on) {
                game.dropBlocks();
                while(game.clearFullRows(false)) {
                    game.current_score += game.cols;
                }
            }
        }
        else if(line == "PRINT_GRID") {
            game.printGridWithScore(false);
            cout << endl;
        }
        else {
            cout << "Unknown command: " << line << endl;
        }
    }
    file.close();
    game.addEntry();
    game.stdoutNoMoreCommands();
    return true;
}



