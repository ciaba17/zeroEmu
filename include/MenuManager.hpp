#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include "../include/InputManager.hpp"

class MenuManager {
private:
    int emulatorIndex = 0; // Index of the current emulator
    std::unordered_map<std::string, SDL_Rect> items; // List of menu items
    std::vector<std::pair<SDL_Texture*, SDL_Texture*>> emulatorTextures; // Pair of emulator logo and wallpaper textures

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
    void render(SDL_Renderer* renderer);
};