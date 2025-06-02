#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include "../include/InputManager.hpp"
#include "../include/Globals.hpp"


class Emulator {
public:
    Emulator(SDL_Texture* logo, SDL_Texture* background, const std::string& emulatorPath);

    SDL_Texture* logo;
    SDL_Texture* background;
    std::string emulatorPath;
    bool rightmost = false;;
    bool leftmost = false;
    int targetX;
    bool shouldAnimate = true;
    bool selected = false;

    SDL_Rect logoRect = {
        int(SCREEN_WIDTH/2), 
        int(SCREEN_HEIGHT/1.5), 
        int(SCREEN_WIDTH/4.5),
        int(SCREEN_HEIGHT/8)
    };

    bool onScreen() {
        return logoRect.x >= 0 && logoRect.x<= SCREEN_WIDTH;
    }
};

class Game {
public:
    std::string name;
    std::string path;
    SDL_Texture* texture;

    SDL_Rect rect = {
        int(SCREEN_WIDTH/4.5), 
        int(SCREEN_HEIGHT/30), 
        int(SCREEN_WIDTH/2.5),
        int(SCREEN_HEIGHT/20)
    };

    Game(const std::string& name, const std::string& path, SDL_Texture* texture);
};

class MenuManager {
private:
    int emulatorIndex = 0; // Index of the current emulator
    int gameIndex = 0; // Index of the current game
    std::unordered_map<std::string, SDL_Rect> items; // List of menu items
    std::vector<Emulator> emulators; // List of emulators
    std::unordered_map<int, std::vector<Game>> games; // List of games

    bool rightPressed = false;
    bool leftPressed = false;
    bool enterPressed = false;

    const int logoWidth = SCREEN_WIDTH / 4.5;
    const int logoHeight = SCREEN_HEIGHT / 8;
    const int logoWidthScaled = SCREEN_WIDTH / 9;
    const int logoHeightScaled = SCREEN_HEIGHT / 16;

public:
    MenuManager(std::unordered_map<std::string, SDL_Texture*>* textures);
    void addItem(const std::string& itemName, SDL_Rect);
    void handleEmulatorsInput(InputManager& inputManager);
    void handleGamesInput(InputManager& inputManager);
    void updateTarget();

    void menuRender(SDL_Renderer* renderer);
    void gamesRender(SDL_Renderer* renderer, TTF_Font* font);
};
