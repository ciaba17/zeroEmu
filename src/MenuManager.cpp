#include "../include/MenuManager.hpp"

Emulator::Emulator(SDL_Texture* logo, SDL_Texture* background, const std::string& name)
    : logo(logo), background(background), name(name) {        
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
    emulators.push_back(Emulator((*textures)["nes"], (*textures)["nesBG"], "nes-emu"));
    emulators.push_back(Emulator((*textures)["snes"], (*textures)["snesBG"], "snesemu"));
    emulators.push_back(Emulator((*textures)["atari"], (*textures)["atariBG"], "atari2600"));
    emulators.push_back(Emulator((*textures)["gameboy"], (*textures)["gameboyBG"], "gameboy"));
    emulators.push_back(Emulator((*textures)["ps"], (*textures)["psBG"], "ps"));
    emulators[0].selected = true; // Set the first emulator as selected
    updateEmulatorTarget(); // Initialize the target positions for the emulators



    // --- GAMES MENU PART ---
    for (const auto& game : std::filesystem::directory_iterator("roms/nes")) {
        if (game.path().extension() == ".nes") {
            // Add the game to the nes games list
            games[0].push_back(Game(game.path().stem().string(), game.path().string(), (*textures)["superMario"]));
        }
    }
    games[1] = { // snes games
        Game("Super Metroid", "/home/Cai/Desktop/programmazione/zeroEmu/roms/snes/super_metroid.sfc", (*textures)["superMetroid"]),
    };

    for (auto& [key, gameList] : games) {
        for (int i = 0; i < gameList.size(); ++i) {
            gameList[i].rect.y += i * (gameList[i].rect.h + 10); // Add some spacing between games
        }
    }
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
        updateEmulatorTarget();
    }
    else if (rightPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RIGHT)) {
        emulatorIndex = (emulatorIndex + 1) % emulators.size();
        rightPressed = false;
        updateEmulatorTarget();
    }
    else if (enterPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RETURN)) {
        // Transition to the games menu
        emulatorsMenu = false;
        gamesMenu = true;
        enterPressed = false;
        updateGameTarget(); // Initialize the target positions for the games
    }
}


void MenuManager::handleGamesInput(InputManager& inputManager) {
    if (inputManager.isKeyPressed(SDL_SCANCODE_ESCAPE)) {
        // Transition back to the emulators menu
        gamesMenu = false;
        emulatorsMenu = true;
    }
    else if (inputManager.isKeyPressed(SDL_SCANCODE_UP)) {
        upPressed = true;
    }
    else if (inputManager.isKeyPressed(SDL_SCANCODE_DOWN)) {
        downPressed = true;
    }
    else if (inputManager.isKeyPressed(SDL_SCANCODE_RETURN)) {
        enterPressed = true;
    }

    if (upPressed && !inputManager.isKeyPressed(SDL_SCANCODE_UP)) {
        if (gameIndex > 0) gameIndex--; // Check the boundaries

        // If the gameIndex is less than 0 move down all the games
        if (!games[emulatorIndex][gameIndex].onScreen()) {
            for (Game& game : games[emulatorIndex]) {
                game.rect.y += (game.rect.h + 10); // Move the game rectangle down
            }
        }
        
        upPressed = false;
        updateGameTarget();
    }
    else if (downPressed && !inputManager.isKeyPressed(SDL_SCANCODE_DOWN)) {
        if (gameIndex + 1 < games[emulatorIndex].size()) gameIndex++; // Check the boundaries

        // If the gameIndex exceeds the number of games on screen move up all the games
        if (!games[emulatorIndex][gameIndex].onScreen()) {
            for (Game& game : games[emulatorIndex]) {
                game.rect.y -= (game.rect.h + 10); // Move the game rectangle up
            }
        }

        downPressed = false;
        updateGameTarget();
    }
    else if (enterPressed && !inputManager.isKeyPressed(SDL_SCANCODE_RETURN)) {
        enterPressed = false;
        std::string romPath = games[emulatorIndex][gameIndex].path;
        std::string command = emulators[emulatorIndex].name + " \"" + romPath + "\"";
        system(command.c_str()); // Execute the command to run the game
        
    }

}


void MenuManager::updateEmulatorTarget()
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


void MenuManager::updateGameTarget() {
    for (int i = 0; i < games[emulatorIndex].size(); ++i) {
        games[emulatorIndex][i].selected = (i == gameIndex); 
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
        
        // Draw the game rectangle
        if (game.selected) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for selected game
        } 
        else {
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Gray for unselected games
        }
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

