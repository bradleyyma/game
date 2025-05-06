#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Player.h"
#include "entities/monsters/Monster.h"
#include "Platform.h"
#include <vector>
#include <memory>

class Game {
public:
    // Game states
    enum class GameState {
        MENU,
        PLAYING,
        GAME_OVER,
        LEADERBOARD
    };

    Game();
    ~Game();
    
    bool init();     // Initialize SDL and create window
    bool loadMedia(); // Load game assets
    void run();      // Main game loop
    void clean();    // Clean up resources
    void addBullet(std::unique_ptr<Bullet> bullet);
    
private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void spawnMonster();  // Spawn a new monster
    void checkCollisions(); // Check for collisions between player and monsters
    void resetGame();     // Reset game state for replay
    
    // Menu and end screen functions
    void renderMenu();
    void renderEndScreen();
    void handleMenuEvents(SDL_Event& event);
    void handleEndScreenEvents(SDL_Event& event);
    
    // Leaderboard
    std::vector<float> leaderboard;
    void loadLeaderboard();
    void saveLeaderboard();
    void updateLeaderboard(float score);
    void renderLeaderboard();
    void handleLeaderboardEvents(SDL_Event& event);
    SDL_Rect leaderboardButtonRect;
    SDL_Rect backButtonRect;
    bool isMouseOverLeaderboardButton;
    bool isMouseOverBackButton;
    std::vector<SDL_Texture*> leaderboardTextTextures;
    std::vector<SDL_Rect> leaderboardTextRects;

    SDL_Window* window;
    SDL_Renderer* renderer;
    GameState currentState;
    SDL_Texture* menuTexture;
    SDL_Rect startButtonRect;
    
    // End screen buttons
    SDL_Rect replayButtonRect;
    SDL_Rect menuButtonRect;
    bool isMouseOverReplayButton;
    bool isMouseOverMenuButton;
    bool isMouseOverButton;
    
    // Text rendering members
    TTF_Font* font;
    SDL_Texture* startButtonText;
    SDL_Texture* replayButtonText;
    SDL_Texture* menuButtonText;
    SDL_Texture* gameOverText;
    SDL_Rect startTextRect;
    SDL_Rect replayTextRect;
    SDL_Rect menuTextRect;
    SDL_Rect gameOverTextRect;
    
    // Timer for tracking time alive
    float timeAlive; // seconds
    float lastScore; // last round's score
    SDL_Texture* timeTextTexture;
    SDL_Rect timeTextRect;
    SDL_Texture* scoreTextTexture;
    SDL_Rect scoreTextRect;

    Player player;
    std::vector< std::unique_ptr<Monster> > monsters;
    std::vector< std::unique_ptr<Platform> > platforms;
    std::vector< std::unique_ptr<Bullet> > bullets;
    
    float monsterSpawnTimer;
    const float MONSTER_SPAWN_INTERVAL = 5.0f; // Spawn a monster every 5 seconds
    const int MAX_MONSTERS = 10; // Maximum number of monsters allowed
    
    bool isRunning;
    Uint32 frameStart;
    int frameTime;
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
    int WORLD_WIDTH;
    int WORLD_HEIGHT;
};