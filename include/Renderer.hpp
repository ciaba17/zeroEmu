#pragma once
#include <SDL2/SDL.h>
#include <iostream>

class Renderer{
private:
SDL_Renderer* renderer;
public:
    Renderer(SDL_Renderer* renderer);

    void render();
};