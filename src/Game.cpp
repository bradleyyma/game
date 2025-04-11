#include "Game.h"
#include "Slime.h"
#include "MonsterFactory.h"
#include "GameUtils.h"
#include <iostream>
#include <random>

Game::Game()
    : window(nullptr), renderer(nullptr), isRunning(false), frameStart(0), frameTime(0),
      monsterSpawnTimer(0.0f) {
}

Game::~Game() {
    clean();
}

bool Game::init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Set renderer color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Get window size for player positioning
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Initialize player in the center of the screen

    player.init(w / 2, h / 2);
    
    isRunning = true;
    return true;
}

bool Game::loadMedia() {
    // Load player texture
    if (!player.loadTexture(renderer, "../assets/images/dot.bmp")) {
        std::cerr << "Failed to load player texture!" << std::endl;
        return false;
    }
    
    return true;
}

void Game::spawnMonster() {
    if (monsters.size() >= MAX_MONSTERS) return;
    
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    
    // Create new slime using MonsterFactory
    auto monster = MonsterFactory::createMonster(MonsterFactory::MonsterType::SLIME, renderer, w, h);
    if (monster) {
        monsters.push_back(std::move(monster));
    }
}

void Game::checkCollisions() {
    // Check for collisions between player and monsters
    for (auto it = monsters.begin(); it != monsters.end();) {
        if (player.checkCollision(**it)) {
            // Player takes damage
            player.takeDamage((*it)->getDamage());
            
            // Remove the monster
            it = monsters.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    frameStart = SDL_GetTicks();
    
    while (isRunning) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        
        frameStart = SDL_GetTicks();
        
        handleEvents();
        update(deltaTime);
        checkCollisions();
        render();
        
        // Cap the frame rate
        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            isRunning = false;
        }
        
        // Pass the event to the player for handling
        player.handleEvent(event);
    }
}

void Game::update(float deltaTime) {
    // Update player
    player.update(deltaTime);
    
    // Update monster spawn timer
    monsterSpawnTimer += deltaTime;
    if (monsterSpawnTimer >= MONSTER_SPAWN_INTERVAL) {
        spawnMonster();
        monsterSpawnTimer = 0.0f;
    }
    
    // Update monsters
    for (auto& monster : monsters) {
        monster->update(deltaTime);
    }
    
}

void Game::render() {
    // Clear screen with white color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    // Render player
    player.render(renderer);
    
    // Render monsters
    for (auto& monster : monsters) {
        monster->render(renderer);
    }
    
    // Update screen
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    IMG_Quit();
    SDL_Quit();
}