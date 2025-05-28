#include "../include/MenuManager.hpp"
#include "../include/Globals.hpp"
#include "../include/InputManager.hpp"

MenuManager::MenuManager() {
    const int w = SCREEN_WIDTH;
    const int h = SCREEN_HEIGHT;

    SDL_Rect emulatorLine = {w/2, int(h/1.5), w, h/9};
    SDL_Rect emulatorLogoBox = {w/2, int(h/1.5), w/3, h/9};

    addItem("emulatorLine", emulatorLine);
    addItem("emulatorLogoBox", emulatorLogoBox);}

void MenuManager::addItem(const std::string& itemName, SDL_Rect rect) {
    rect.x -= rect.w/2;
    rect.y -= rect.h/2;
    items[itemName] = rect; 
}

void MenuManager::handleInput(InputManager& inputManager) {
    if (inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        std::cout << "Right key pressed" << std::endl;
    }
    if (inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        std::cout << "Left key pressed" << std::endl;
    }
}

void MenuManager::render(SDL_Renderer* renderer, std::unordered_map<std::string, SDL_Texture*>* textures) {
    // Draw the background
    SDL_RenderCopy(renderer, (*textures)["NESWP"], nullptr, nullptr);
    // Draw the emulator line
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 210);
    SDL_RenderFillRect(renderer, &items["emulatorLine"]); 

    // Draw the NES logo on the emulator logo box
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &items["emulatorLogoBox"]);
    SDL_RenderCopy(renderer, (*textures)["NES"], nullptr, &items["emulatorLogoBox"]);
}