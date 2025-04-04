#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"

class Game {
public:
    Game();
    ~Game();
    
    bool init();     // Initialize SDL and create window
    bool loadMedia(); // Load game assets
    void run();      // Main game loop
    void clean();    // Clean up resources
    
private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    
    bool isRunning;
    Uint32 frameStart;
    int frameTime;
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
};