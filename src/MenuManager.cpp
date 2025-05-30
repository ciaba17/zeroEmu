#include "../include/MenuManager.hpp"
#include "../include/Globals.hpp"
#include "../include/InputManager.hpp"

Emulator::Emulator(SDL_Texture* logo, SDL_Texture* background, const std::string& emulatorPath, std::vector<Emulator>& emulators)
    : logo(logo), background(background), emulatorPath(emulatorPath) {        
        // Center the logo rectangle
        logoRect.y -= logoRect.h/2;

}

MenuManager::MenuManager(std::unordered_map<std::string, SDL_Texture*>* textures) {
    const int w = SCREEN_WIDTH;
    const int h = SCREEN_HEIGHT;

    // Consoles menu part
    const SDL_FRect emulatorLine = {w/2, h/1.5, w, h/7};
    const SDL_FRect previousEmulatorLogoBox = {w/2 - w/3, h/1.5, w/11, h/17};
    const SDL_FRect nextEmulatorLogoBox = {w/2 + w/3, h/1.5, w/11, h/17};

    addItem("emulatorLine", emulatorLine);

    // Add the emulators to the menu
    emulators.push_back(Emulator((*textures)["nes"], (*textures)["nesBG"], "nes", emulators));
    emulators.push_back(Emulator((*textures)["atari"], (*textures)["atariBG"], "atari2600", emulators));
    emulators.push_back(Emulator((*textures)["gameboy"], (*textures)["gameboyBG"], "gameboy", emulators));
    emulators.push_back(Emulator((*textures)["snes"], (*textures)["snesBG"], "snes", emulators));
    emulators.push_back(Emulator((*textures)["ps"], (*textures)["psBG"], "ps", emulators));
    emulators[0].selected = true; // Set the first emulator as selected

    // Games menu part
    const SDL_FRect gameLine = {w/2, h/1.5, w/2, h/7};

    updateTargetPositions();
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

    // Input took only if the key is released, this prevents the transition from being triggered multiple times
    if (leftPressed && !inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        emulatorIndex = (emulatorIndex - 1 + emulators.size()) % emulators.size();
        leftPressed = false;
        updateTargetPositions();
    }
    else if (rightPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        emulatorIndex = (emulatorIndex + 1) % emulators.size();
        rightPressed = false;
        updateTargetPositions();
    }
}


void MenuManager::updateTargetPositions()
{
    const int centerX = SCREEN_WIDTH / 2 - SCREEN_WIDTH / 8; // center position for the current emulator logo
    const int spacing = SCREEN_WIDTH / 3; // space between logos

    int total = emulators.size();


    for (int i = 0; i < total; ++i) {
        // Calculate the target position for each emulator logo
        // The target position is calculated based on the current index and the center index
        int offset = i - emulatorIndex;

        // Adjust the offset to ensure it wraps around correctly
        if (offset > total / 2) offset -= total;
        if (offset < -total / 2) offset += total;

        float newTargetX = centerX + offset * spacing;

        // If the distance to the new target position is greater than half the screen width, it teleport the logo
        if (std::abs(emulators[i].targetX - newTargetX) > SCREEN_WIDTH / 2) {
            emulators[i].shouldAnimate = false;
            emulators[i].logoRect.x = newTargetX; // Teleport
        } else {
            emulators[i].shouldAnimate = true; // Standard animation
        }

        emulators[i].targetX = centerX + offset * spacing;
    }
}



void MenuManager::menuRender(SDL_Renderer* renderer) {
    // Draw the background
    SDL_RenderCopy(renderer, emulators[emulatorIndex].background, nullptr, nullptr);

    // Draw the emulator line
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_RenderFillRect(renderer, &items["emulatorLine"]);
    
    // Draw the logo for the current emulator
    for (Emulator& emulator : emulators)
{
    // Linear interpolation for smooth movement
    const float speed = 0.2f;
    emulator.logoRect.x += (emulator.targetX - emulator.logoRect.x) * speed;

    if (emulator.onScreen()) {
        // Draw the logo
        SDL_RenderCopy(renderer, emulator.logo, nullptr, &emulator.logoRect);
    }
}

}


void MenuManager::gamesRender(SDL_Renderer* renderer) {

}

