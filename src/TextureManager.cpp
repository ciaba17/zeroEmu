#include "../include/TextureManager.hpp"

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer(renderer) {
    // Load all textures at initialization
    loadSingleTexture("NES", "assets/nesLogo.png");
    loadSingleTexture("NESWP", "assets/nesWP.png");

    loadSingleTexture("atari", "assets/atariLogo.png");
    loadSingleTexture("atariWP", "assets/atariWP.png");

}

void TextureManager::loadSingleTexture(std::string name, std::string path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());

    if (!texture) {
        std::cerr << "Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
        return;
    }

    textures[name] = texture;
}