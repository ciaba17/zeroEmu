#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <unordered_map>

class TextureManager {
public:
    SDL_Renderer* renderer = nullptr;

    TextureManager(SDL_Renderer* renderer);

    std::unordered_map<std::string, SDL_Texture*> textures;

    void loadSingleTexture(std::string name, std::string path);
    void loadAllTextures();
};