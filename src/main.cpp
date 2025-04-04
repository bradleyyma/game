#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;
    
    // Initialize the game
    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }
    
    // Load game media
    if (!game.loadMedia()) {
        std::cerr << "Failed to load game media!" << std::endl;
        return 1;
    }
    
    // Run the game loop
    game.run();
    
    // Clean up
    game.clean();
    
    return 0;
}