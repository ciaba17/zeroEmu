#include "../include/TextureManager.hpp"

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer(renderer) {
    // Load all textures at initialization
    loadSingleTexture("nesBG", "assets/nesBG.png");
    loadSingleTexture("nes", "assets/nesLogo.png");

    loadSingleTexture("atariBG", "assets/atariBG.png");
    loadSingleTexture("atari", "assets/atariLogo.png");

    loadSingleTexture("gameboyBG", "assets/gameboyBG.png");
    loadSingleTexture("gameboy", "assets/gameboyLogo.png");

    loadSingleTexture("snesBG", "assets/snesBG.png");
    loadSingleTexture("snes", "assets/snesLogo.png");

    loadSingleTexture("psBG", "assets/psBG.png");
    loadSingleTexture("ps", "assets/psLogo.png");

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