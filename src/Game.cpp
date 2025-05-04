#include "Game.h"
#include "entities/monsters/Slime.h"
#include "entities/monsters/MonsterFactory.h"
#include "GameUtils.h"
#include <iostream>
#include <random>

Game::Game()
    : window(nullptr), renderer(nullptr), isRunning(false), frameStart(0), frameTime(0),
      monsterSpawnTimer(0.0f), currentState(GameState::MENU), menuTexture(nullptr),
      isMouseOverButton(false), isMouseOverReplayButton(false), isMouseOverMenuButton(false),
      font(nullptr), startButtonText(nullptr), replayButtonText(nullptr), 
      menuButtonText(nullptr), gameOverText(nullptr) {
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
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
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
    
    // Load font
    font = TTF_OpenFont("../assets/fonts/Arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Set renderer color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // Get window size
    SDL_GetWindowSize(window, &WORLD_WIDTH, &WORLD_HEIGHT);

    // Initialize button positions (centered)
    startButtonRect = {
        WORLD_WIDTH / 2 - 100,  // x
        WORLD_HEIGHT / 2 - 30,  // y
        200,                    // width
        60                      // height
    };
    
    replayButtonRect = {
        WORLD_WIDTH / 2 - 100,
        WORLD_HEIGHT / 2 - 30,
        200,
        60
    };
    
    menuButtonRect = {
        WORLD_WIDTH / 2 - 100,
        WORLD_HEIGHT / 2 + 50,
        200,
        60
    };

    // Create text textures
    SDL_Color textColor = {255, 255, 255, 255}; // White text
    
    // Start button text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Start Game", textColor);
    if (textSurface) {
        startButtonText = SDL_CreateTextureFromSurface(renderer, textSurface);
        startTextRect.w = textSurface->w;
        startTextRect.h = textSurface->h;
        startTextRect.x = startButtonRect.x + (startButtonRect.w - startTextRect.w) / 2;
        startTextRect.y = startButtonRect.y + (startButtonRect.h - startTextRect.h) / 2;
        SDL_FreeSurface(textSurface);
    }
    
    // Replay button text
    textSurface = TTF_RenderText_Solid(font, "Play Again", textColor);
    if (textSurface) {
        replayButtonText = SDL_CreateTextureFromSurface(renderer, textSurface);
        replayTextRect.w = textSurface->w;
        replayTextRect.h = textSurface->h;
        replayTextRect.x = replayButtonRect.x + (replayButtonRect.w - replayTextRect.w) / 2;
        replayTextRect.y = replayButtonRect.y + (replayButtonRect.h - replayTextRect.h) / 2;
        SDL_FreeSurface(textSurface);
    }
    
    // Menu button text
    textSurface = TTF_RenderText_Solid(font, "Return to Menu", textColor);
    if (textSurface) {
        menuButtonText = SDL_CreateTextureFromSurface(renderer, textSurface);
        menuTextRect.w = textSurface->w;
        menuTextRect.h = textSurface->h;
        menuTextRect.x = menuButtonRect.x + (menuButtonRect.w - menuTextRect.w) / 2;
        menuTextRect.y = menuButtonRect.y + (menuButtonRect.h - menuTextRect.h) / 2;
        SDL_FreeSurface(textSurface);
    }
    
    // Game Over text
    textSurface = TTF_RenderText_Solid(font, "Game Over!", textColor);
    if (textSurface) {
        gameOverText = SDL_CreateTextureFromSurface(renderer, textSurface);
        gameOverTextRect.w = textSurface->w;
        gameOverTextRect.h = textSurface->h;
        gameOverTextRect.x = WORLD_WIDTH / 2 - textSurface->w / 2;
        gameOverTextRect.y = WORLD_HEIGHT / 3;
        SDL_FreeSurface(textSurface);
    }

    // Initialize player position
    player.setPosition(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);

    // Initialize platform
    std::unique_ptr<Platform> p1 = std::make_unique<Platform>(100, WORLD_HEIGHT / 1.1, 200, 50, 100.0f, 0);
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

    if (!Platform::loadSharedTexture(renderer, "../assets/images/platform.png")) {
        std::cerr << "Failed to load platform texture!" << std::endl;
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

    // Check if player is dead
    if (player.getHealth() <= 0) {
        currentState = GameState::GAME_OVER;
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
        
        if (currentState == GameState::MENU) {
            handleMenuEvents(event);
        } 
        else if (currentState == GameState::GAME_OVER) {
            handleEndScreenEvents(event);
        }
        else {
            // Pass the event to the player for handling
            player.handleEvent(event);
        }
    }
}

void Game::handleMenuEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;
        
        // Check if mouse is over the start button
        isMouseOverButton = (mouseX >= startButtonRect.x && 
                           mouseX <= startButtonRect.x + startButtonRect.w &&
                           mouseY >= startButtonRect.y && 
                           mouseY <= startButtonRect.y + startButtonRect.h);
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            // Check if click is on the start button
            if (mouseX >= startButtonRect.x && 
                mouseX <= startButtonRect.x + startButtonRect.w &&
                mouseY >= startButtonRect.y && 
                mouseY <= startButtonRect.y + startButtonRect.h) {
                currentState = GameState::PLAYING;
            }
        }
    }
}

void Game::handleEndScreenEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;
        
        // Check if mouse is over the replay button
        isMouseOverReplayButton = (mouseX >= replayButtonRect.x && 
                                 mouseX <= replayButtonRect.x + replayButtonRect.w &&
                                 mouseY >= replayButtonRect.y && 
                                 mouseY <= replayButtonRect.y + replayButtonRect.h);
        
        // Check if mouse is over the menu button
        isMouseOverMenuButton = (mouseX >= menuButtonRect.x && 
                               mouseX <= menuButtonRect.x + menuButtonRect.w &&
                               mouseY >= menuButtonRect.y && 
                               mouseY <= menuButtonRect.y + menuButtonRect.h);
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            // Check if click is on the replay button
            if (mouseX >= replayButtonRect.x && 
                mouseX <= replayButtonRect.x + replayButtonRect.w &&
                mouseY >= replayButtonRect.y && 
                mouseY <= replayButtonRect.y + replayButtonRect.h) {
                resetGame();
                currentState = GameState::PLAYING;
            }
            
            // Check if click is on the menu button
            if (mouseX >= menuButtonRect.x && 
                mouseX <= menuButtonRect.x + menuButtonRect.w &&
                mouseY >= menuButtonRect.y && 
                mouseY <= menuButtonRect.y + menuButtonRect.h) {
                resetGame();
                currentState = GameState::MENU;
            }
        }
    }
}

void Game::resetGame() {
    // Clear all game objects
    monsters.clear();
    bullets.clear();
    
    player.reset(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);
    
    // Reset monster spawn timer
    monsterSpawnTimer = 0.0f;
    
    // Reset platforms to initial positions
    platforms.clear();
    std::unique_ptr<Platform> p1 = std::make_unique<Platform>(100, WORLD_HEIGHT / 1.1, 200, 50, 100.0f, 0);
    platforms.emplace_back(std::move(p1));
}

void Game::update(float deltaTime) {
    if (currentState == GameState::PLAYING) {
        // Update platforms
        for (auto & platform : platforms) {
            platform->update(deltaTime);

            SDL_Rect rect = platform->getCollider();
            if (rect.x <= 0 || rect.x + rect.w >= WORLD_WIDTH) {
                platform->reverseX();
            }
        }
        
        // Update player
        player.update(deltaTime, *this);
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
}

void Game::render() {
    // Clear screen with white color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    switch (currentState) {
        case GameState::MENU:
            renderMenu();
            break;
        case GameState::GAME_OVER:
            renderEndScreen();
            break;
        case GameState::PLAYING:
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
            break;
    }
    
    // Update screen
    SDL_RenderPresent(renderer);
}

void Game::renderMenu() {
    // Set the color for the button
    if (isMouseOverButton) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Light blue when hovered
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for normal state
    }
    
    // Render the start button
    SDL_RenderFillRect(renderer, &startButtonRect);
    
    // Render the text on the button
    if (startButtonText) {
        SDL_RenderCopy(renderer, startButtonText, nullptr, &startTextRect);
    }
}

void Game::renderEndScreen() {
    // Render "Game Over" text
    if (gameOverText) {
        SDL_RenderCopy(renderer, gameOverText, nullptr, &gameOverTextRect);
    }

    // Render replay button
    if (isMouseOverReplayButton) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Light blue when hovered
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for normal state
    }
    SDL_RenderFillRect(renderer, &replayButtonRect);
    
    // Render replay button text
    if (replayButtonText) {
        SDL_RenderCopy(renderer, replayButtonText, nullptr, &replayTextRect);
    }

    // Render menu button
    if (isMouseOverMenuButton) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Light blue when hovered
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for normal state
    }
    SDL_RenderFillRect(renderer, &menuButtonRect);
    
    // Render menu button text
    if (menuButtonText) {
        SDL_RenderCopy(renderer, menuButtonText, nullptr, &menuTextRect);
    }
}

void Game::clean() {
    // Clean up shared bullet texture
    Bullet::cleanupSharedTexture();
    
    if (startButtonText) {
        SDL_DestroyTexture(startButtonText);
        startButtonText = nullptr;
    }

    if (replayButtonText) {
        SDL_DestroyTexture(replayButtonText);
        replayButtonText = nullptr;
    }

    if (menuButtonText) {
        SDL_DestroyTexture(menuButtonText);
        menuButtonText = nullptr;
    }

    if (gameOverText) {
        SDL_DestroyTexture(gameOverText);
        gameOverText = nullptr;
    }

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::addBullet(std::unique_ptr<Bullet> bullet) {
    bullets.push_back(std::move(bullet));
}