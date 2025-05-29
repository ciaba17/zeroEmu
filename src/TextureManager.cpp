#include "../include/TextureManager.hpp"

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer(renderer) {
    // Load all textures at initialization
    loadSingleTexture("NESWP", "assets/nesWP.png");
    loadSingleTexture("NES", "assets/nesLogo.png");

    loadSingleTexture("atariWP", "assets/atariWP.png");
    loadSingleTexture("atari", "assets/atariLogo.png");

    loadSingleTexture("gameboyWP", "assets/gameboyWP.png");
    loadSingleTexture("gameboy", "assets/gameboyLogo.png");

    for (const auto& pair : textures) {
        SDL_SetTextureBlendMode(pair.second, SDL_BLENDMODE_BLEND); // activare blending
    }
}

void TextureManager::loadSingleTexture(std::string name, std::string path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());

    if (!texture) {
        std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
        return;
    }

    textures[name] = texture;
}