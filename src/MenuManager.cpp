#include "../include/MenuManager.hpp"
#include "../include/Globals.hpp"
#include "../include/InputManager.hpp"

MenuManager::MenuManager(std::unordered_map<std::string, SDL_Texture*>* textures) {
    const int w = SCREEN_WIDTH;
    const int h = SCREEN_HEIGHT;

    // Consoles menu part
    const SDL_FRect emulatorLine = {w/2, h/1.5, w, h/7};
    const SDL_FRect previousEmulatorLogoBox = {w/2 - w/3, h/1.5, w/11, h/17};
    const SDL_FRect nextEmulatorLogoBox = {w/2 + w/3, h/1.5, w/11, h/17};

    addItem("emulatorLine", emulatorLine);
    addItem("emulatorLogoBox", emulatorLogoBox);
    addItem("previousEmulatorLogoBox", previousEmulatorLogoBox);
    addItem("nextEmulatorLogoBox", nextEmulatorLogoBox);

    // Set the initial position of the central emulator logo box
    initialPosition = items["emulatorLogoBox"].x;

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

    emulatorTextures.push_back(
        std::make_pair(
            (*textures)["gameboyWP"], 
            (*textures)["gameboy"]
        )
    );


    // Games menu part
    const SDL_FRect gameLine = {w/2, h/1.5, w/2, h/7};
}

void MenuManager::addItem(const std::string& itemName, SDL_FRect rect) {
    // Center the rectangle
    rect.x -= rect.w/2;
    rect.y -= rect.h/2;

    // Convert SDL_FRect to SDL_Rect
    SDL_Rect intRect;
    intRect.x = static_cast<int>(rect.x);
    intRect.y = static_cast<int>(rect.y);
    intRect.w = static_cast<int>(rect.w);
    intRect.h = static_cast<int>(rect.h);
    
    items[itemName] = intRect; 
}

void MenuManager::handleInput(InputManager& inputManager) {
    if (inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        rightPressed = true;
    }
    else if (inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        leftPressed = true;
    }

    if (rightPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        if (emulatorIndex == emulatorTextures.size() - 1) {
            emulatorIndex = 0; // Wrap around to the first emulator
        } 
        else {
            emulatorIndex++;
        }
        isTransitioning = true;
        rightPressed = false;
    }
    else if (leftPressed && !inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        if (emulatorIndex == 0) {
            emulatorIndex = emulatorTextures.size() - 1; // Wrap around to the last emulator
        } 
        else {
            emulatorIndex--;
        }
        isTransitioning = true;
        leftPressed = false;
    }

    //transition();
}

void MenuManager::transition() {
    if (isTransitioning) {
        transitionProgress += transitionSpeed;
        items["emulatorLogoBox"].x += transitionDistance;
        WPTransparency-= 20;
        if (transitionProgress >= 2.0f) {
            transitionProgress = 0.0f;
            items["emulatorLogoBox"].x = initialPosition;
            WPTransparency = 255;
            isTransitioning = false;
        }
    }
}

void MenuManager::menuRender(SDL_Renderer* renderer) {
    // Draw the background for the current emulator
    SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex].first, nullptr, nullptr);
    
    // Draw the emulator line
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_RenderFillRect(renderer, &items["emulatorLine"]);
    
    // Draw the logo for the current emulator
    SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex].second, nullptr, &items["emulatorLogoBox"]);

    if (emulatorIndex == 0) { 
        // If it's the first emulator, wrap around to the last one
        SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex+1].second, nullptr, &items["nextEmulatorLogoBox"]);
        SDL_RenderCopy(renderer, emulatorTextures[emulatorTextures.size()-1].second, nullptr, &items["previousEmulatorLogoBox"]);
    }
    else if (emulatorIndex == emulatorTextures.size() - 1) { 
        // If it's the last emulator, wrap around to the first one
        SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex-1].second, nullptr, &items["previousEmulatorLogoBox"]);
        SDL_RenderCopy(renderer, emulatorTextures[0].second, nullptr, &items["nextEmulatorLogoBox"]);
    } 
    else { 
        // For all other emulators, render the previous and next ones
        SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex-1].second, nullptr, &items["previousEmulatorLogoBox"]);
        SDL_RenderCopy(renderer, emulatorTextures[emulatorIndex+1].second, nullptr, &items["nextEmulatorLogoBox"]);
    }
}


void MenuManager::gamesRender(SDL_Renderer* renderer) {

}


Emulator::Emulator(const int index, SDL_Texture* logo, SDL_Texture* background, const std::string& emulatorPath)
    : index(index), logo(logo), background(background), emulatorPath(emulatorPath) {
        
    const int w = SCREEN_WIDTH;
    const int h = SCREEN_HEIGHT;
        
    // Set the logo rectangle to a default size
    logoRect = {w/2, h/1.5, w/3.8, h/9.8};   
    secondaryLogoRect = {w/2, h/1.5, w/11, h/17};
    

}