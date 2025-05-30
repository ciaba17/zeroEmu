#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include "../include/InputManager.hpp"
#include "../include/Globals.hpp"

class Emulator {
public:
    Emulator(SDL_Texture* logo, SDL_Texture* background, const std::string& emulatorPath, std::vector<Emulator>& emulators);

    SDL_Texture* logo;
    SDL_Texture* background;
    std::string emulatorPath;
    bool rightmost = false;;
    bool leftmost = false;
    int targetX;

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

    Game(const std::string& name, const std::string& path, SDL_Texture* texture)
        : name(name), path(path), texture(texture) {}
};

class MenuManager {
private:
    int emulatorIndex = 0; // Index of the current emulator
    std::unordered_map<std::string, SDL_Rect> items; // List of menu items
    std::vector<Emulator> emulators; // List of emulators

    bool rightPressed = false;
    bool leftPressed = false;

    int initialPosition;
    bool isTransitioning = false;
    float transitionProgress = 0.0; // da 0.0 a 1.0
    const float transitionSpeed = 0.3; // più piccolo = più lento
    const float transitionDistance = 8; // Distanza di transizione in pixel
    int WPTransparency = 255; // Opacità del menu

public:
    MenuManager(std::unordered_map<std::string, SDL_Texture*>* textures);
    void addItem(const std::string& itemName, SDL_FRect);
    void handleInput(InputManager& inputManager);
    void transition();
    void updateTargetPositions();

    void menuRender(SDL_Renderer* renderer);
    void gamesRender(SDL_Renderer* renderer);
};
