#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../include/InputManager.hpp"

class MenuManager {
public:
    std::unordered_map<std::string, SDL_Rect> items; // List of menu items

    MenuManager();
    void addItem(const std::string& itemName, SDL_Rect);
    void handleInput(InputManager& inputManager);
    void render(SDL_Renderer* renderer, std::unordered_map<std::string, SDL_Texture*>* textures);
};