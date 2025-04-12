#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"
#include "entities/monsters/Monster.h"
#include "Platform.h"
#include <vector>
#include <memory>

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
    void spawnMonster();  // Spawn a new monster
    void checkCollisions(); // Check for collisions between player and monsters
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    std::vector< std::unique_ptr<Monster> > monsters;
    std::vector<Platform> platforms;
    
    float monsterSpawnTimer;
    const float MONSTER_SPAWN_INTERVAL = 5.0f; // Spawn a monster every 5 seconds
    const int MAX_MONSTERS = 10; // Maximum number of monsters allowed
    
    bool isRunning;
    Uint32 frameStart;
    int frameTime;
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
};