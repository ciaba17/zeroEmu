#include "../include/MenuManager.hpp"

Emulator::Emulator(SDL_Texture* logo, SDL_Texture* background, const std::string& emulatorPath)
    : logo(logo), background(background), emulatorPath(emulatorPath) {        
        // Center the logo rectangle
        logoRect.y -= logoRect.h/2;
}

Game::Game(const std::string& name, const std::string& path, SDL_Texture* texture)
    : name(name), path(path), texture(texture) {
        // Center the game rectangle
        rect.x -= rect.w/2;
        rect.y -= rect.h/2;
    }

MenuManager::MenuManager(std::unordered_map<std::string, SDL_Texture*>* textures) {
    const int w = SCREEN_WIDTH;
    const int h = SCREEN_HEIGHT;

    // --- CONSOLE MENU PART ---
    const SDL_Rect emulatorLine = {int(w/2), int(h/1.5), int(w), int(h/7)};
    const SDL_Rect selectionBox = {int(w/2), int(h/1.5), int(w/4.3),int(h/7.8)};

    addItem("emulatorLine", emulatorLine); // Line for the emulators
    addItem("selectionBox", selectionBox); // Selection box for the current emulator

    // Add the emulators to the menu
    emulators.push_back(Emulator((*textures)["nes"], (*textures)["nesBG"], "nes"));
    emulators.push_back(Emulator((*textures)["atari"], (*textures)["atariBG"], "atari2600"));
    emulators.push_back(Emulator((*textures)["gameboy"], (*textures)["gameboyBG"], "gameboy"));
    emulators.push_back(Emulator((*textures)["snes"], (*textures)["snesBG"], "snes"));
    emulators.push_back(Emulator((*textures)["ps"], (*textures)["psBG"], "ps"));
    emulators[0].selected = true; // Set the first emulator as selected
    updateTarget(); // Initialize the target positions for the emulators



    // --- GAMES MENU PART ---
    games[0] = { // NES games
        Game("Super Mario Bros.", "roms/nes/super_mario_bros.nes", (*textures)["superMario"]),
        Game("The Legend of Zelda", "roms/nes/zelda.nes", (*textures)["zelda"]),
        Game("Metroid", "roms/nes/metroid.nes", (*textures)["metroid"])
    };
}

void MenuManager::addItem(const std::string& itemName, SDL_Rect rect) {
    // Center the rectangle
    rect.x -= rect.w/2;
    rect.y -= rect.h/2;
    
    items[itemName] = rect; 
}

void MenuManager::handleEmulatorsInput(InputManager& inputManager) {
    if (inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        rightPressed = true;
    }
    else if (inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        leftPressed = true;
    }
    else if (inputManager.isKeyPressed(SDL_SCANCODE_RETURN)) {
        enterPressed = true;
    }


    // Input took only if the key is released, this prevents the transition from being triggered multiple times
    if (leftPressed && !inputManager.isKeyPressed(SDL_SCANCODE_LEFT)) {
        emulatorIndex = (emulatorIndex - 1 + emulators.size()) % emulators.size();
        leftPressed = false;
        updateTarget();
    }
    else if (rightPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        emulatorIndex = (emulatorIndex + 1) % emulators.size();
        rightPressed = false;
        updateTarget();
    }
    else if (enterPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RETURN)) {
        // Transition to the games menu
        emulatorsMenu = false;
        gamesMenu = true;
        enterPressed = false;
    }
}


void MenuManager::handleGamesInput(InputManager& inputManager) {
    if (inputManager.isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        // Transition back to the emulators menu
        gamesMenu = false;
        emulatorsMenu = true;
    }
}


void MenuManager::updateTarget()
{
    // Set the selected state for the current emulator
    for (int i = 0; i < emulators.size(); ++i) {
        emulators[i].selected = (i == emulatorIndex); 
    }


    const int centerX = SCREEN_WIDTH / 2 - SCREEN_WIDTH / 9; // center position for the current emulator logo
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
    for (Emulator& emulator : emulators) {
        // Linear interpolation for smooth movement
        const float speed = 0.2f;
        emulator.logoRect.x += (emulator.targetX - emulator.logoRect.x) * speed;

        if (emulator.onScreen()) {
            // Draw the logo
            SDL_RenderCopy(renderer, emulator.logo, nullptr, &emulator.logoRect);
        }
    }
}


void MenuManager::gamesRender(SDL_Renderer* renderer, TTF_Font* font) {
    for (int i = 0; i < games[emulatorIndex].size(); ++i) {
        Game& game = games[emulatorIndex][i];

        SDL_Rect renderRect = game.rect; // Create a copy of the rectangle
        renderRect.y += i * (renderRect.h + 10); // Add spacing between games
        
        // Draw the game rectangle
        SDL_SetRenderDrawColor(renderer, 0, 0, 100, 180);
        SDL_RenderDrawRect(renderer, &renderRect);

        // Draw the game name
        SDL_Surface* surface = TTF_RenderText_Solid(font, game.name.c_str(), {255, 255, 255});
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dst = {renderRect.x + 10, renderRect.y + 10, surface->w, surface->h};
            SDL_FreeSurface(surface);
            if (texture) {
                SDL_RenderCopy(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
        
    }
}

