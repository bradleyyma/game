#include "Game.h"
#include "entities/monsters/Slime.h"
#include "entities/monsters/MonsterFactory.h"
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

    // Set the world dimensions, for now we'll use the window size
    WORLD_WIDTH = w;
    WORLD_HEIGHT = h;

    // Initialize player in the center of the screen

    player.setPosition(w / 2, h / 2);


    std::unique_ptr<Platform> p1 = std::make_unique<Platform>(100, h / 1.1, 200, 50, 100.0f, 0);
    
    platforms.emplace_back(std::move(p1));

    isRunning = true;
    return true;
}

bool Game::loadMedia() {
    if (!player.loadTexture(renderer, "../assets/images/hero.png")) {
        std::cerr << "Failed to load player texture!" << std::endl;
    }

    if (!Bullet::loadSharedTexture(renderer, "../assets/images/bullet.png")) {
        std::cerr << "Failed to load bullet texture!" << std::endl;
    }

    if (!Slime::loadSharedTexture(renderer, "../assets/images/new_slime.png")) {
        std::cerr << "Failed to load slime texture!" << std::endl;
    }

    if (!Hopper::loadSharedTexture(renderer, "../assets/images/hopper.png")) {
        std::cerr << "Failed to load hopper texture!" << std::endl;
    }

    for (auto& platform : platforms) {
        if (!platform->loadTexture(renderer, "../assets/images/platform.png")) {
            std::cerr << "Failed to load platform texture!" << std::endl;
            return false;
        }
    }
    
    return true;
}

void Game::spawnMonster() {
    if (monsters.size() >= MAX_MONSTERS) return;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    auto monsterType = dis(gen) == 0 ? MonsterFactory::MonsterType::SLIME : MonsterFactory::MonsterType::HOPPER;
    // Create new slime using MonsterFactory
    auto monster = MonsterFactory::createMonster(monsterType, renderer, WORLD_WIDTH, WORLD_HEIGHT);
    if (monster) {
        monsters.push_back(std::move(monster));
    }
}

void Game::checkCollisions() {

    // Check bullet-monster collisions first
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
        bool bulletHit = false;
        SDL_Rect bulletRect = GameUtils::makeRect((*bulletIt)->getX(), (*bulletIt)->getY(), 
                                                Bullet::WIDTH, Bullet::HEIGHT);
        
        // Check if bullet is out of bounds
        if (!GameUtils::isInScreen(bulletRect, WORLD_WIDTH, WORLD_HEIGHT)) {
            bulletIt = bullets.erase(bulletIt);
            continue;
        }

        // Check collision with monsters
        for (auto monsterIt = monsters.begin(); monsterIt != monsters.end() && !bulletHit;) {
            SDL_Rect monsterRect = GameUtils::makeRect((*monsterIt)->getX(), (*monsterIt)->getY(), 
                                                     (*monsterIt)->getWidth(), (*monsterIt)->getHeight());
            
            if (GameUtils::checkCollision(bulletRect, monsterRect)) {
                // Handle bullet hit on monster
                (*monsterIt)->takeDamage((*bulletIt)->getDamage());
                if ((*monsterIt)->getHealth() <= 0) {
                    monsterIt = monsters.erase(monsterIt);
                } else {
                    ++monsterIt;
                }
                bulletIt = bullets.erase(bulletIt);
                bulletHit = true;
            } else {
                ++monsterIt;
            }
        }
        
        if (!bulletHit) {
            ++bulletIt;
        }
    }

    // Check monster-player collisions and monster bounds
    SDL_Rect playerRect = GameUtils::makeRect(player.getX(), player.getY(), 
                                            Player::WIDTH, Player::HEIGHT);
    
    for (auto monsterIt = monsters.begin(); monsterIt != monsters.end();) {
        SDL_Rect monsterRect = GameUtils::makeRect((*monsterIt)->getX(), (*monsterIt)->getY(), 
                                                 (*monsterIt)->getWidth(), (*monsterIt)->getHeight());
        
        // Check if monster is out of bounds
        if (!GameUtils::isInScreen(monsterRect, WORLD_WIDTH, WORLD_HEIGHT)) {
            monsterIt = monsters.erase(monsterIt);
            continue;
        }

        // Check collision with player
        if (GameUtils::checkCollision(playerRect, monsterRect)) {
            player.takeDamage((*monsterIt)->getDamage());
            monsterIt = monsters.erase(monsterIt);
        } else {
            ++monsterIt;
        }
    }

    // Check player bounds
    if (player.getX() < 0) player.setPosition(0, player.getY());
    if (player.getX() > WORLD_WIDTH - Player::WIDTH) player.setPosition(WORLD_WIDTH - Player::WIDTH, player.getY());
    if (player.getY() < 0) player.setPosition(player.getX(), 0);
    if (player.getY() > WORLD_HEIGHT - Player::HEIGHT) {
        player.setPosition(player.getX(), WORLD_HEIGHT - Player::HEIGHT);
        player.resetJump(); // Stop jumping when hitting ground
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
        if (bullets.size() > 0) {
            std::cout << "Bullets: " << bullets.size() << std::endl;
        }
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
    // Update platforms
    for (auto & platform : platforms) {
        platform->update(deltaTime);

        SDL_Rect rect = platform->getCollider();
        if (rect.x <= 0 || rect.x + rect.w >= WORLD_WIDTH) {
            platform->reverseX();
        }
    }
    
    // Update player
    player.update(deltaTime, * this);
    player.getGun()->update(deltaTime);  // Update bullets
    
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

    // Update all bullets and remove any that are no longer active
    for (auto it = bullets.begin(); it != bullets.end();) {
        if ((*it)->move(deltaTime)) {
            ++it; // Bullet is still active
        } else {
            it = bullets.erase(it); // Remove inactive bullet
        }
    }
    
}

void Game::render() {
    // Clear screen with white color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    // Render platform
    for (auto & platform : platforms) {
        platform->render(renderer);
    }

    // Render player
    player.render(renderer);
    player.getGun()->render(renderer);  // Render bullets
    
    // Render monsters
    for (auto& monster : monsters) {
        monster->render(renderer);
    }

    // Render all active bullets
    for (const auto& bullet : bullets) {
        bullet->render(renderer);
    }
    
    // Update screen
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    // Clean up shared bullet texture
    Bullet::cleanupSharedTexture();
    
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

void Game::addBullet(std::unique_ptr<Bullet> bullet) {
    bullets.push_back(std::move(bullet));
}