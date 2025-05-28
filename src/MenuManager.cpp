#include "../include/MenuManager.hpp"
#include "../include/Globals.hpp"
#include "../include/InputManager.hpp"

MenuManager::MenuManager(std::unordered_map<std::string, SDL_Texture*>* textures) {
    const int w = SCREEN_WIDTH;
    const int h = SCREEN_HEIGHT;

    SDL_Rect emulatorLine = {w/2, int(h/1.5), w, h/9};
    SDL_Rect emulatorLogoBox = {w/2, int(h/1.5), w/3, h/9};

    addItem("emulatorLine", emulatorLine);
    addItem("emulatorLogoBox", emulatorLogoBox);

    emulatorTextures.push_back(
        std::make_pair(
            (*textures)["NESWP"], 
            (*textures)["NES"]
        )
    );

    emulatorTextures.push_back(
        std::make_pair(
            (*textures)["atariWP"], 
            (*textures)["atari"]
        )
    );
}

void MenuManager::addItem(const std::string& itemName, SDL_Rect rect) {
    rect.x -= rect.w/2;
    rect.y -= rect.h/2;
    items[itemName] = rect; 
}

void MenuManager::handleInput(InputManager& inputManager) {
    if (inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        rightPressed = true;
    }
    else if (inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        leftPressed = true;
    }

    if (rightPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        if (emulatorIndex < emulatorTextures.size() - 1) {
            emulatorIndex++;
            isTransitioning = true;
        }
        rightPressed = false;
    }
    else if (leftPressed && !inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        if (emulatorIndex > 0) {
            emulatorIndex--;
        }
        leftPressed = false;
    }

    transition();
}

void MenuManager::transition() {
    if (!isTransitioning) {
        initialPosition = 100;
    }
    else {
        transitionProgress += transitionSpeed;
        items["emulatorLogoBox"].x += transitionDistance;
        
        if (transitionProgress >= 1000.0f) {
            transitionProgress = 0.0f;
            items["emulatorLogoBox"].x = initialPosition;
            isTransitioning = false;
        }
    }
}

void MenuManager::render(SDL_Renderer* renderer) {
    // Draw the background
    SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex].first, nullptr, nullptr);
    std::cout << "Rendering emulator: " << emulatorIndex << std::endl;
    // Draw the emulator line
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 210);
    SDL_RenderFillRect(renderer, &items["emulatorLine"]); 

    // Draw the NES logo on the emulator logo box
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &items["emulatorLogoBox"]);
    SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex].second, nullptr, &items["emulatorLogoBox"]);
}