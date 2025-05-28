#pragma once
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <iostream>

class WindowManager {
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool init(const char* title);
};